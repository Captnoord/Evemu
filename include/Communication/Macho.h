#ifndef _MACHO_H
#define _MACHO_H

/**
 * A macho address identifies a client, client call, broadcast, narrowcast, scattercast,
 * node, node service call, any node service call, or an any/whatever type of address.
 * Pretty much, if it can be targeted with a macho packet, it can be addressed with
 * a macho address.
 */
class MachoAddress
{
public:
protected:
	PyTuple info;
};
//macho.Notification(destination=macho.MachoAddress(broadcastID=method, idtype='charid', narrowcast=charids

/*    
__legalclientaddr__ = ('clientID', 'callID', 'service')
__legalbroadcastaddr__ = ('broadcastID', 'narrowcast', 'idtype')
__legalnodeaddr__ = ('nodeID', 'service', 'callID')
__legalanyaddr__ = ('service', 'callID')  
*/

class Macho
{
public:
	Macho(PyObject* object)
	{

	}

	PyInt * machoCallType;
	PyString * machoFunctionCall;


};

class MachoPacket
{
public:
protected:
	PyInt mUserID;
	PyInt mCompressedPart;
	MachoAddress mSource;
	MachoAddress mDestination;
	PyObject * oob;
	PyObject * channel;

};

#endif // _MACHO_H