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

#ifndef _PYTUPLE_H
#define _PYTUPLE_H

/**
* \class PyTuple
*
* @brief PyTuple is one of the most used objects within python. Its a object to store objects in.
*
* http://en.wikipedia.org/wiki/Tuple
*
* @note one of the design choices was to use a vector to store the pointers.
* @note this at first hand only cause big slowdowns when allocating a big amount of items.
* @author Captnoord
* @date March 2009
*/
class PyTuple : public PyObject
{
    /* limit the tuple to 1000 items */
#define PY_TUPLE_ELEMENT_MAX 1000
public:
    PyTuple();
    PyTuple(size_t elementCount);
    ~PyTuple();

    // tuple hash function
    uint32 hash();

    /**
    * \brief operator overload for easy object access and storage
    *
    * nothing much to tell about this function, it returns a PyChameleon object reference.
    *
    * @param[in] index is the location of the required object.
    * @return always returns a PyChameleon object even if there isn't a object stored (so it can be used to store objects).
    */
    //PyChameleon &operator[](const int index);
    PyObject* GetItem(const int index);

    // utility functions, warning don't use this should without knowing how it works and what it does.
    int32		GetItem_asInt(const int index);
    int64		GetItem_asLong(const int index);
    double		GetItem_asDouble(const int index);
    PyTuple*	GetItem_asPyTuple(const int index);
    PyList*		GetItem_asPyList(const int index);
    PyString*	GetItem_asPyString(const int index);
    PySubStream*GetItem_asPySubStream(const int index);
    PyClass*    GetItem_asPyClass(const int index);

    /* smarter way todo get stuff... */
    bool GetString( const int index, std::string& rStr )
    {
        if (index > (int)mTuple.size())
            return false;

        PyChameleon * itr = mTuple[index];
        PyObject * object = itr->getPyObject();

        if (object->gettype() != PyTypeString)
            return false;

        PyString * strobj = (PyString*)object;

        rStr.clear(); // make sure its empty
        rStr.append(strobj->content(), strobj->length());

        return true;
    }

    bool		set_item(const int index, PyObject *object);

    /* @todo add other variable types */
    int scanf( const char* pFormat, ... )
    {
        /* should not be possible */
        if (pFormat == NULL || *pFormat == '\0')
            return 0;

        size_t formatLen = strlen(pFormat);
        if (formatLen == 0)
            return 0;

        if (formatLen != size())
            return 0;

        va_list ap;
        va_start(ap, pFormat);

        void* pVar = NULL;
        size_t formatIndex = 0;
        while (formatLen != formatIndex)
        {
            pVar = va_arg( ap, void* );

            char tag = pFormat[formatIndex];

            /* we check for NULL argument pointers but only if the tag isn't a 'ignore' tag*/
            if (pVar == NULL && tag != '0')
            {
                va_end(ap);
                return 0;
            }

            if (formatIndex > mTuple.size())
            {
                va_end(ap);
                return 0;
            }        

            PyObject* pFoundObject = mTuple[formatIndex]->getPyObject();

            ASCENT_ASSERT(pFoundObject);

            switch(tag)
            {
            case '0': // ignore
                {
                } break;

            case 'i': // int
                {
                    if (pFoundObject->gettype() != PyTypeInt)
                    {
                        va_end(ap);
                        return 0;
                    }

                    int32* pNum = (int32*)pVar;
                    *pNum = ((PyInt*)pFoundObject)->GetValue();
                } break;

            case 'f': // double
                {
                    if (pFoundObject->gettype() != PyTypeReal)
                    {
                        va_end(ap);
                        return 0;
                    }

                    double * pNum = (double *)pVar;
                    *pNum = ((PyFloat*)pFoundObject)->GetValue();
                } break;

            case 's': // std::string
                {
                    if (pFoundObject->gettype() != PyTypeString)
                    {
                        va_end(ap);
                        return 0;
                    }

                    std::string * pStr = (std::string *)pVar;
                    pStr->clear();

                    size_t len = ((PyString*)pFoundObject)->length();
                    const char* pBuff = ((PyString*)pFoundObject)->content();
                    pStr->append(pBuff, len);
                } break;

                /*unicode string*/
            case 'u': // std::wstring
                {
                    if (pFoundObject->gettype() != PyTypeUnicode)
                    {
                        va_end(ap);
                        return 0;
                    }

                    std::wstring* pStr = (std::wstring *)pVar;
                    pStr->clear();

                    size_t len = ((PyUnicodeUCS2*)pFoundObject)->length();
                    wchar_t* pBuff = ((PyUnicodeUCS2*)pFoundObject)->content();
                    pStr->append(pBuff, len);
                } break;

                /* scanf internal types will be tricky. Are we required to increase
                * the ref counter when we use the pointer. In a way we are because
                * that will make everything as flexible as I designed it.
                * @todo add ref counter increase stuff.
                */
            case 't': // PyTuple
                {
                    if (pFoundObject->gettype() != PyTypeTuple)
                    {
                        va_end(ap);
                        return 0;
                    }

                    PyTuple** pTuple = (PyTuple **)pVar;
                    (*pTuple) = (PyTuple*)pFoundObject;
                } break;

                /* same stuff as the PyTuple stuff... tricky
                */
            case 'd': // PyDict
                {
                    if (pFoundObject->gettype() != PyTypeDict)
                    {
                        va_end(ap);
                        return 0;
                    }

                    PyDict** pDict = (PyDict **)pVar;
                    (*pDict) = (PyDict*)pFoundObject;
                } break;
            }
            formatIndex++;
        }

        va_end(ap);
        return (int)formatIndex;
    }

    void set_str( const int index, const char* str )
    {
        if (index+1 > (int)mTuple.size())
            mTuple.resize(index+1);
        PyChameleon* itr = mTuple[index];
        PyString* pStr = new PyString(str);
        itr->setPyObject((PyObject*)pStr);
    }

    void set_str( const int index, const char* str, const size_t len )
    {
        if (index+1 > (int)mTuple.size())
            mTuple.resize(index+1);
        PyChameleon* itr = mTuple[index];
        itr->setPyObject((PyObject*)new PyString(str, len)); // GCC is not going to like this
    }

    void set_int( const int index, const int number )
    {
        if (index+1 > (int)mTuple.size())
            mTuple.resize(index+1);
        PyChameleon* itr = mTuple[index];
        itr->setPyObject((PyObject*)new PyInt(number));
    }

    void set_long( const int index, const long number )
    {
        if (index+1 > (int)mTuple.size())
            mTuple.resize(index+1);
        PyChameleon* itr = mTuple[index];
        itr->setPyObject((PyObject*)new PyLong((int64)number));
    }

    void set_float( const int index, const double number )
    {
        if (index+1 > (int)mTuple.size())
            mTuple.resize(index+1);
        PyChameleon* itr = mTuple[index];
        itr->setPyObject((PyObject*)new PyFloat(number));
    }

    // returns the element count
    size_t size();

    // clears the tuple from all objects
    void clear();

    bool resize(size_t elementCount);
private:
    typedef std::vector<PyChameleon*> TupleVector;
public:
    typedef TupleVector::iterator iterator;
    typedef TupleVector::const_iterator const_iterator;
    iterator begin() {return mTuple.begin();}
    iterator end() {return mTuple.end();}
private:
    TupleVector mTuple;
};

#endif //_PYTUPLE_H
