/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2009 The EVEmu Team
    For the latest information visit http://evemu.mmoforge.org
    ------------------------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any later
    version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License along with
    this program; if not, write to the Free Software Foundation, Inc., 59 Temple
    Place - Suite 330, Boston, MA 02111-1307, USA, or go to
    http://www.gnu.org/copyleft/lesser.txt.
    ------------------------------------------------------------------------------------
    Author:     Captnoord
*/

#include "EvemuPCH.h"
#include "ByteBuffer.h"
#include "PasswordModule.h"
#include "BinAsciiModule.h"
#include "EVEVersion.h"

#ifdef OBJECT_DUMPER_SUPPORT
#  include "PyObjectDumper.h"
#endif//OBJECT_DUMPER_SUPPORT

// Dummy authorization handshake function
static const uint8 handShakeInitialFunctionBlob[] = {
    0x74, 0x04, 0x00, 0x00, 0x00, 0x4E, 0x6F, 0x6E, 0x65    //marshaled Python string "None"
};
static const uint8 handShakeInitialFunctionBlobSize = 9;

/* defining debug messages which can be disabled */
#ifdef _DEBUG
#  define dlog(x,...) printf(x"\n",  __VA_ARGS__)
#else
#  define dlog(x,...) /*x*/
#endif

EveClientSocket::EveClientSocket(SOCKET fd) : Socket(fd, CLIENTSOCKET_SENDBUF_SIZE, CLIENTOCKET_RECVBUF_SIZE),
    mCurrentStateMachine(&EveClientSocket::_authStateHandshake), mSession(NULL), mAccountInfo(NULL), mQueued(false),
        mAuthed(false), mNagleEanbled(false), mUserName(""), mRemaining(0)
{
    /* we don't support authorization queue for now
     * we don't enable 'nagle' for now
     */
}

EveClientSocket::~EveClientSocket()
{
    /* delete the buffers in the send buffer queue */
    // disabled for now as we don't have a send queue yet
    //WorldPacket * pck;
    //while((pck = _queue.Pop()))
    //  delete pck;

    /* delete the 'auth' request packet if it still exists */
    //if (mRequest != NULL)
    //  delete mRequest;

    /* remove the link between the current socket and its client session */
    if (mSession)
    {
        mSession->SetSocket(NULL);
        mSession = NULL;
    }
}

void EveClientSocket::OnDisconnect()
{
    sSpace.OnClientDisconnect();

    if(mSession != NULL)
    {
        mSession->SetSocket(0);
        mSession = NULL;
    }
}

/** Connection Handshake Packet Description
 * PyTuple:6
 *   itr[0]: PyInt containing the EveBirthday.
 *   itr[1]: PyInt containing the macho version number.
 *   itr[2]: PyInt containing the amount of connected players.
 *   itr[3]: PyFloat containing the eve version number.
 *   itr[4]: PyInt containing the eve build number.
 *   itr[5]: PyString containing the eve project string.
 */
void EveClientSocket::sendConnectionHandShake()
{
    uint32 clusterUserCount = (uint32)sSpace.GetAuthorizedCount();

    PyTuple tulpe(7);
    tulpe.set_int(0, EveBirthday);
    tulpe.set_int(1, MachoNetVersion);
    tulpe.set_int(2, clusterUserCount);
    tulpe.set_float(3, EveVersionNumber);
    tulpe.set_int(4, EveBuildVersion);
    tulpe.set_str(5, EveProjectVersion);

    /*PyClass * script = new PyClass();
    script->setname( new PyString("util.KeyVal") );

    PyDict * script_dict = new PyDict();
    script_dict->set_str("hash", "Mqt8SPIrL9vl87VNKuLzcw==");
    script_dict->set_int("build", 240510);
    script_dict->set_str("fileurl", "http://content.eveonline.com/codePatch/240510/compiled.code");

    script->setdict(script_dict);

    tulpe[6] = (PyObject*)script;*/

    send(tulpe);
}

/** Crypto Context Notify Packet description
 *
 * PyString:"OK"
 * PyString:"OK CC"
 * PyString:"CC"
 */

/* send handshake accept */
void EveClientSocket::sendCryptoContextAccept()
{
    PyString out("OK CC");
    send(out);
}

bool EveClientSocket::sendInt( int number )
{
    PyInt num(number);
    return send(num);
}

/**
 * quick and tricky because we don't want to increase and decrease the ref counter.
 */
bool EveClientSocket::sendPyNone()
{
    return send(mMarshal.PyNone);
}

void EveClientSocket::OnConnect()
{
    sSpace.OnClientConnect();

    /* atm we don't track socket latency*/
    //_latency = GetTickCount();

    /* send initial 'auth' handshake */
    sendConnectionHandShake();
}

void EveClientSocket::OnRead()
{
    printf("EveClientSocket::OnRead\n");
    for(;;)
    {
        //Check for the header if we don't have any bytes to wait for.
        if(mRemaining == 0)
        {
            /* check if we at least have a header */
            if(GetReadBuffer().GetSize() < 4)
                return;

            /* copy the packet size from the buffer */
            uint32 packetSize;
            GetReadBuffer().Read((void*)&packetSize, 4);
            mRemaining = packetSize;
        }

        ReadStream * packet = NULL;
        if(mRemaining > 0)
        {
            /* check if we have a fragmented packet, if so wait until its completed */
            if( GetReadBuffer().GetSize() < mRemaining )
                return;

            /*
             * @todo merge the object deserialization and feeding it directly from the read buffer
             * this requires to add some kind of wrapper mode to ReadStream.
             */
            packet = new ReadStream(mRemaining);

            if ( !GetReadBuffer().Read(packet->content(), mRemaining) )
            {
                Log.Error("ClientSocket","buffer read went wrong");
                SafeDelete(packet);
                mRemaining = 0;
                return;
            }

            mRemaining = 0;
        }

        /* doing the marshal load here is terrible inefficient (it can block the socket worker thread) */
        PyObject* object = mMarshal.load(*packet);
        SafeDelete(packet);

#ifdef OBJECT_DUMPER_SUPPORT
        printf("client packet:\n");
        DumpObject(stdout, object);
        printf("\n");
#endif//OBJECT_DUMPER_SUPPORT

        if (object == NULL)
        {
            Log.Error("ClientSocket","unable to unmarshal object");
            return;
        }

        // the state machine magic
        (this->*mCurrentStateMachine)(object);
        object->DecRef();
    }
}

/* the client sends back its version info...
 */
void EveClientSocket::_authStateHandshake(PyObject* object)
{
    if (object->gettype() != PyTypeTuple)
    {
        dlog("ClientSocket::initial packet isn't a tuple");
        return;
    }

    PyTuple * tuple = (PyTuple*)object;
    // not sure if we need this check because of the scanf function below
    /*if (tuple->size() != 6)
    {
        dlog("ClientSocket::initial packet isn't a tuple size 6");
        return;
    }*/

    int32 birthDay;
    int32 machoVersion;
    int32 userCount;
    double versionNr;
    int32 buildVersion;
    std::string projectVersion;

    // note for mmcs look what I did :P
    //                         'i'        'i'            'i'         'f'         'i'            's'
    if(!tuple->scanf("iiifis", &birthDay, &machoVersion, &userCount, &versionNr, &buildVersion, &projectVersion))
    {
        Log.Error("ClientSocket","unable to read info from tuple");
        return;
    }

    if (birthDay != EveBirthday || machoVersion != MachoNetVersion || versionNr != EveVersionNumber || buildVersion != EveBuildVersion || projectVersion != EveProjectVersion)
    {
        Log.Error("AuthStateMachine", "incompatible client trying to connect.");
        Disconnect();
        return;
    }

    dlog("AuthStateMachine::State changed into StateQueueCommand");
    mCurrentStateMachine = &EveClientSocket::_authStateQueueCommand;
}

void EveClientSocket::_authStateQueueCommand(PyObject* object)
{
    /* we only want to be fed tuple's at this point */
    if (object->gettype() != PyTypeTuple)
    {
        Log.Error("AuthStateMachine", "received object isn't a tuple");
        mCurrentStateMachine = &EveClientSocket::_authStateHandshake;
        sendConnectionHandShake();
        return;
    }

    PyTuple * tuple = (PyTuple*)object;

    /** Packet description
     * tuple[0] = session hash
     * tuple[1] = command
     * tuple[2] = optional.
     */
    std::string queueCommand;
    if(!tuple->GetString(1, queueCommand))
    {
        dlog("AuthStateMachine::_authStateQueueCommand unable to get queueCommand string from packet");
        return;
    }

    /* check if the user has special rights */
    if (queueCommand == "VK") // vip key
    {
        dlog("AuthStateMachine::State changed into StateStateNoCrypto");
        mCurrentStateMachine = &EveClientSocket::_authStateNoCrypto;
        return;
    }
    else if (queueCommand == "QC") // queue command
    {
        /* send login queue position */
        sendInt(1);

        sendConnectionHandShake();

        dlog("AuthStateMachine::State changed into StateHandshake");
        mCurrentStateMachine = &EveClientSocket::_authStateHandshake;
        return;
    }
    else
    {
        Log.Error("AuthStateMachine", "%s: Received invalid command packet:", GetRemoteIP().c_str());
        Disconnect();
    }
    /* If we get here.... it means that the authorization failed in some way, also we didn't handle the exceptions yet.
     *
     */
}

void EveClientSocket::_authStateNoCrypto(PyObject* object)
{
    if (object->gettype() != PyTypeTuple)
    {
        dlog("AuthStateMachine::_authStateNoCrypto packet is not a PyTuple");
        return;
    }

    PyTuple * tuple = (PyTuple*)object;

    std::string cryptoVersion;
    if(!tuple->GetString(0, cryptoVersion))
    {
        dlog("AuthStateMachine::_authStateNoCrypto unable to get crypt version string");
        ResetConnection();
        return;
    }

    /* do the crypt version check */
    if (cryptoVersion == "placebo")
    {
        dlog("AuthStateMachine::State changed into StateCryptoChallenge");
        mCurrentStateMachine = &EveClientSocket::_authStateCryptoChallenge;

        sendCryptoContextAccept();
        return;
    }
    else
    {
        /* add a generic message that they need to change there client config files */
        sLog.Debug("Received invalid 'crypto' request!");
        ResetConnection();
    }
}

/** Challenge-Response Packet Description.
 * PyTuple:4
 *   itr[0]:PyString containing a 64 bytes Session Hash
 *   itr[1]:PyTuple:2
 *     itr[0]:                          PySubStream containing a PyString containing a cPickled piece of Python byte code.
 *     itr[1]:                          PyString or PyBool containing the argument for the byte code (not sure).
 *   itr[2]:PyDict:0                    Empty PyDict.
 *   itr[3]:PyDict:9
 *     dict["boot_version"]=            PyFloat containing the eve version number.
 *     dict["boot_region"]=             PyString containing the Eve Project Region
 *     dict["cluster_usercount"]=       PyInt containing the number of users in that cluster.
 *     dict["user_logonqueueposition"]= PyInt containing the login queue position.
 *     dict["challenge_responsehash"]=  PyString containing a 40 bytes hash.
 *     dict["macho_version"]=           PyInt containing the macho version number.
 *     dict["boot_codename"]=           PyString containing the eve project string.
 *     dict["boot_build"]=              PyInt containing the eve build number.
 *     dict["proxy_nodeid"]=            PyInt containing the node id
 */

/** Client-Handshake Packet Description.
  * PyTuple:2
  *   itr[0]: PyString containing a 64 bytes session hash
  *   itr[1]:PyDict:10
  *     dict["boot_version"]=           PyFloat containing the eve version number.
  *     dict["boot_region"]=            PyString containing the Eve Project Region.
  *     dict["user_password"]=          PyNone or PyString containing nothing or the password if its requested as plain.
  *     dict["user_affiliateid"]=       PyInt (I have no clue where or for what the 'affiliateid' is used for).
  *     dict["user_password_hash"]=     PyString containing a 20 bytes username + password hash.
  *     dict["macho_version"]=          PyInt containing the macho version number.
  *     dict["boot_codename"]=          PyString containing the eve project string.
  *     dict["boot_build"]=             PyInt containing the eve build number.
  *     dict["user_name"]=              PyString containing the username.
  *     dict["user_languageid"]=        PyString containing the 2 characters language 'thingy' ( EN, NL, DE, FR, AU ).
  */
void EveClientSocket::_authStateCryptoChallenge(PyObject* object)
{
    if (object->gettype() != PyTypeTuple)
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge packet is not a PyTuple");
        ResetConnection();
        return;
    }

    PyTuple & tuple = *(PyTuple*)object;
    if (tuple.size() != 2)
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge tuple size is wrong");
        ResetConnection();
        return;
    }

    /*  Client-Handshake Packet */
    PyDict * dict = NULL;
    if (!tuple.scanf("0d", NULL, &dict))
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge received object isn't a dict");
        ResetConnection();
        return;
    }


    
    /*PyDict &dict = *(PyDict *)tuple.GetItem(1);

    if(dict.gettype() != PyTypeDict)
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge received object isn't a dict");
        ResetConnection();
        return;
    }*/

    std::wstring UserName;
    char UserPasswordHash[20];

    /* Client handshake */
    /* the only 2 things we actually need to know for now */
    if(!dict->scanf("user_name", "u", &UserName))
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge unable to read username");
        ResetConnection();
        return;
    }

    if(!dict->get_buffer("user_password_hash", (char*)UserPasswordHash, 20))
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge unable to read user password hash");
        ResetConnection();
        return;
    }

    mAccountInfo = sAccountMgr.lookupAccount(UserName);
    if (mAccountInfo == NULL)
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge account doesn't exist");
        /* account doesn't exist */
        Disconnect();
        return;
    }

    /* check if the user password hash matches the saved one. */
    if(memcmp(UserPasswordHash, mAccountInfo->AccountShaHash, 20) == TRUE)
    {
        dlog("AuthStateMachine::_authStateCryptoChallenge pass hash not correct");
        /* pass doesn't seem to match... send exception... */
        Disconnect();
        return;
    }
    else
    {
        /* send password version */
        sendInt(2);

        PyTuple AuthServerResponceTuple(4);

        /** initial function blob
         * @note cache this tuple as its not required to be build and allocated time and time again.
         */
        PyTuple & initialFunctionBlob = *new PyTuple(2);
          initialFunctionBlob[0] = new PyString((char*)handShakeInitialFunctionBlob, handShakeInitialFunctionBlobSize);
          initialFunctionBlob[1] = new PyBool(false);

        uint32 clusterUserCount = (uint32)sSpace.GetAuthorizedCount();
        uint32 proxyNodeId = sSpace.GetProxyNodeID();
          /* sync data dict */
        PyDict* syncDict = new PyDict();
          syncDict->set_double("boot_version", EveVersionNumber);
          syncDict->set_str("boot_region", EveProjectRegion);
          syncDict->set_str("challenge_responsehash", "55087");
          syncDict->set_int("cluster_usercount", clusterUserCount);
          syncDict->set_int("user_logonqueueposition", 1);
          syncDict->set_int("macho_version", MachoNetVersion);
          syncDict->set_str("boot_codename", EveProjectCodename);
          syncDict->set_int("boot_build", EveBuildVersion);
          syncDict->set_int("proxy_nodeid", proxyNodeId);

        /* the main tuple */
        AuthServerResponceTuple[0] = "hi";                  /* we can send everything here as its related to the handShakeInitialFunctionBlob */
        AuthServerResponceTuple[1] = &initialFunctionBlob;
        AuthServerResponceTuple[2] = new PyDict();          /* empty PyDIct */
        AuthServerResponceTuple[3] = syncDict;

        send(AuthServerResponceTuple);
    }

    dlog("AuthStateMachine::State changed into HandservershakeSend");
    mCurrentStateMachine = &EveClientSocket::_authStateHandshakeSend;
}

void EveClientSocket::_authStateHandshakeSend(PyObject* object)
{
    uint32 userid = sSpace.GenerateUserId();
    uint32 userClientId = mAccountInfo->AccountId;

    /* Server challenge response ack */
    PyDict dict;
    PyDict * session_init = new PyDict();

      PyString * pyAddress = GetAddress();
      session_init->set_str ("languageID",      "EN");
      session_init->set_int ("userid",          userid);
      session_init->set_item("maxSessionTime",  mMarshal.GetPyNone());
      session_init->set_int ("userType",        1); /* userType: trial:23, steam:? normal:1? */
      session_init->set_int ("role",            mAccountInfo->AccountRole);
      session_init->set_item("address",         pyAddress);
      session_init->set_bool("inDetention",     false);

    dict.set_item("live_updates", new PyList());
    dict.set_item("session_init", session_init);
    dict.set_item("client_hashes" , new PyList());
    dict.set_int ("user_clientid", userClientId);

    send(dict);

    Log.Debug("AuthStateMachine","State changed into StateDone");
    mCurrentStateMachine = &EveClientSocket::packetHandler;

    mAuthed = true;
}

enum PacketType
{
    PacketTypeCallRsp,// this really should never happen... weird if this one happens:P
    PacketTypeCallReq,  
    PacketTypeNotification,
    PacketTypeException,
    PacketTypeUnknown,// used for unimplemented packet types.
};

PacketType GetPacketType(PyObject* object)
{
    if (object->gettype() != PyTypeClass)
        return PacketTypeUnknown;

    PyClass* klass = (PyClass*)object;
    
    PyTuple* bases = klass->getbases();

    /* this seems to be valid for both exceptions and calls */
    int machoType = 0;
    if (!bases->scanf("i", &machoType))
        return PacketTypeUnknown;

    /* we do a little convertion just for my own peace of mind */
    switch(machoType)
    {
    case 15:
        return PacketTypeException;
    case 6:
        return PacketTypeCallReq;
    case 12:
        return PacketTypeNotification;
    case 7:
        return PacketTypeCallRsp;
    }

    return PacketTypeUnknown;
}



/* 'ingame' packet dispatcher */
void EveClientSocket::packetHandler( PyObject* object )
{
    Log.Debug("ClientSocket","received packet 'whooo' we passed authorization");

    /* I know that all packets should first be dumped into a client wise packet queue */
    switch (GetPacketType(object))
    {
    case PacketTypeCallRsp:
        break;
    case PacketTypeCallReq:
        break;
    case PacketTypeNotification:
        break;
    case PacketTypeException:
        break;
    case PacketTypeUnknown:
        break;
    }
}

PyString * EveClientSocket::GetAddress()
{
    char address[22];

    /* "The Matrix is a system, 'Neo'. That system is our enemy. But when you're inside, you look around, what do you see?"
     * @note Aim I'm sorry :'( but I don't think this is cross platform compatible.
     */
    uint8 *addr = (uint8*)&m_client.sin_addr;

    int len = snprintf(address, 21, "%u.%u.%u.%u:%u", addr[0], addr[1],addr[2],addr[3], m_client.sin_port);

    /* snprintf will return < 0 when a error occurs so return NULL */
    if (len < 0)
        return NULL;

    return PyString_FromStringAndSize(address, len);
}

void EveClientSocket::ResetConnection()
{
    /* when we are resetting the connection we send the handshake again
     * and set login state machine back to its starting state.
     */
    sendConnectionHandShake();
    mCurrentStateMachine = &EveClientSocket::_authStateHandshake;
}

bool EveClientSocket::prepPacket( PyObject* obj )
{
    /* this is crappy but I can not think of another way */
    if (obj->gettype() != PyTypeClass)
        return false;

    PyClass & data = *(PyClass*)obj;
    
    return true;
}