class PyDict : public PyObject
{
public:
    PyDict();
    ~PyDict();
    uint32 hash();

    bool    mMappingMode;
    uint32  mMappingIndex;

    PyChameleon operator[](const char* keyName);

    /**
    * \brief set_item adds or sets a database entry.
    *
    * PyDict::set_item handles the adding and setting of object in
    * mapped and non mapped dictionary's.
    *
    * @param[in]
    * @param[out]
    * @return
    */
    template<typename T>
    bool set_item(const char* key_name, T* obj)
    {
        if (key_name == NULL || obj == NULL)
        {
            /* something broke so debug it */
            ASCENT_HARDWARE_BREAKPOINT;
            return false;
        }

        /*if (mMappingMode == true)
        {
        // create a new dictionary entry
        PyDictEntry * entry = new PyDictEntry;
        entry->key = (PyObject*)new PyString(key_name);
        entry->obj = (PyObject*)obj;
        mDict.insert(std::make_pair(mMappingIndex++, entry));
        obj->IncRef();
        }
        else
        {
        // IMPLEMENT THIS ONE..... 
        //ASCENT_HARDWARE_BREAKPOINT;
        // do the same as the set_item function that takes a PyObject as a key, but do some tricks with it 

        // NOTE: not used.
        //size_t str_len = strlen(key_name);

        // test this and check these 
        //uint32 hsh = Utils::Hash::sdbm_hash(key_name, (int)str_len);
        //uint32 hsh1 = Utils::Hash::sdbm_hash(key_name);
        }*/

        set_item((PyObject*)new PyString(key_name),(PyObject*)obj);

        return true;
    }

    __forceinline bool set_item(PyObject* key, PyObject* obj);
    PyObject* get_item(PyObject* key);
    PyObject* get_item(const char* key_name);
    PyObject* get_item(const char* key_name, PyObject* default_obj);

    bool scanf(const char * keyName, const char * format, ...);

    bool get_buffer(const char * keyName, char* dst, size_t dst_len);
    bool set_int(const char * keyName, int number);
    bool set_double(const char * keyName, double number);
    bool set_str(const char * keyName, const char* str);
    bool set_bool(const char * keyName, bool check);

    size_t size();

    /* atm only changing static mapped dict to hashed dict */
    void update();

private:
    typedef std::map<uint32, PyDictEntry*>  DictMap;
    typedef DictMap::iterator               DictMapItr;
public:
    typedef DictMap::iterator               iterator;
    iterator begin();
    iterator end();
private:
    DictMap mDict;
};