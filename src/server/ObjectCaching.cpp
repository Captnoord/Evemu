#include "EvemuPCH.h"
#include "ShaModule.h"

initialiseSingleton(ObjectCachingSvc);

/************************************************************************/
/* CachedObject                                                         */
/************************************************************************/
CachedObject::CachedObject( bool shared, const char* objectID, PyObject *object, PyObject* objectVersion /*= NULL */ ) : PyClass()
{
    //ASCENT_HARDWARE_BREAKPOINT;
    ASCENT_ASSERT(!mBases);
    ASCENT_ASSERT(!mName);

    mName = new PyString("objectCaching.CachedObject");
    mBases = new PyTuple(7);
    PyTuple & bases = *mBases;

    // make sure we are generating a version for this object if we need to
    if (shared == true || objectVersion == NULL)
    {
        WriteStream * wstream = new WriteStream();
        MarshalStream marshalStream;
        marshalStream.save(object, *wstream);

        // very inefficient.
        PySubStream * wSubStream = new PySubStream();
        wSubStream->set(wstream->content(), wstream->size());

        bases[1] = wSubStream;
        // do zlib shit.. and store it as the pickled....

        PyObject* object_version = GenerateObjectVersion(*wstream);

        SafeDelete(wstream);
        bases[0].setPyObject(object_version); // this is going to crash hehe...
    }
    else
    {
        bases[0] = objectVersion;       //object version
        bases[1] = new PyBaseNone;      //the pickle, containing the MarshalSream of a object.
    }

    bases[2] = 0;                   //nodeID
    bases[3] = shared;              //shared
    bases[4] = object;              //object
    bases[5] = 0;                   //compressed
    bases[6] = objectID;            //objectID
}

PyObject* CachedObject::GenerateObjectVersion( WriteStream& stream )

{
    char shaResult1[20];
    std::string shaInit = "thisIsAFuckedUpSaltX";
    ShaModule::SHAobject shaObj;
    ShaModule::sha_init(&shaObj);
    ShaModule::sha_digest(&shaObj, shaInit);
    ShaModule::sha_update(&shaObj, (ShaModule::SHA_BYTE*)stream.content(), (int)stream.size());
    ShaModule::sha_final((unsigned char*)shaResult1, &shaObj);

    // using a predefined result sha
    //unsigned char shaResult[20] = {'\x02','\x0f','\x0a','\x0b','\x02','\x0f','\x0a','\x0b','\x02','\x0f',
    //                               '\x0a','\x0b','\x02','\x0f','\x0a','\x0b','\x02','\x0f','\x0a','\x0b'};

    PyTuple *obj_version = new PyTuple(2);

    obj_version->set_int(0, 1337);                          // timestamp
    obj_version->set_str(1, (char*)shaResult1, 20);         // hash
    return (PyObject*)obj_version;
}

/************************************************************************/
/* Object Caching Service <singleton>                                   */
/************************************************************************/
PyObject* ObjectCachingSvc::GetCachedObject( PyObject * objectID /*PyString*/ )
{
    PyObject *obj = mCachedObjects.get_item(objectID);
    if (obj != NULL)
    {
        obj->IncRef();
        return obj;
    }
    PyNone.IncRef();
    return (PyObject*)&PyNone;
}

PyObject* ObjectCachingSvc::GetCachedObjectVersion( PyObject * objectID )
{
    PyObject *obj = mCachedObjects.get_item(objectID);
    if (obj != NULL)
    {
        CachedObject * v = (CachedObject *)obj;
        PyTuple & base = *v->getbases();
        if (v != NULL && &base != NULL && base.gettype() == PyTypeTuple)
        {
            PyObject * ret_obj = base.GetItem(1);
            if (ret_obj != NULL)
            {
                ret_obj->IncRef();
                return ret_obj;
            }
            /* if object is null we return a PyNone. */
        }
    }
    PyNone.IncRef();
    return (PyObject*)&PyNone;
}

void ObjectCachingSvc::CacheObject( PyObject* object, const char * objectID )
{
    CachedObject * newCachedObject = new CachedObject(1, objectID, object);
    mCachedObjects.set_item(objectID, (PyObject*)newCachedObject);
}

void ObjectCachingSvc::AddCachedObject( CachedObject *cachedObject, const char *objectID )
{
    mCachedObjects.set_item(objectID, (PyObject*)cachedObject);
}
