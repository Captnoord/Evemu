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

#pragma pack(push,1)

/**
 * \class PyTuple
 *
 * @brief PyTuple is one of the most used objects within python. Its a pObject to store objects in.
 *
 * http://en.wikipedia.org/wiki/Tuple
 *
 * @note one of the design choices was to use a vector to store the pointers.
 * @note this at first hand only cause big slowdowns when allocating a big amount of items.
 * @author Captnoord
 * @date March 2009
 */
class PyTuple
{
/* limit the tuple to 1000 items */
#define PY_TUPLE_ELEMENT_MAX 1000
    uint8 mType;
    size_t mRefcnt;
    uint32 (PyTuple::*mHash)();
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    explicit PyTuple();
    explicit PyTuple(size_t elementCount);
    ~PyTuple();

    /**
     * \brief operator overload for easy pObject access and storage
     *
     * nothing much to tell about this function, it returns a PyChameleon pObject reference.
     *
     * @param[in] index is the location of the required pObject.
     * @return always returns a PyChameleon pObject even if there isn't a pObject stored (so it can be used to store objects).
     */
    PyChameleon &operator[](const int index);

    /**
     * Generic get pObject function. We shouldn't use this function much as it makes the code a mess
     * unless we are working with generic objects.
     */
    PyObject* GetItem(const int index);

    /**
     * unsafe get integer function. We shouldn't use this function much as it is unsafe.
     * use scanf for most of the value retrievers.
     */
    int GetInt(const int index);

    /**
     * unsafe get float function. We shouldn't use this function much as it is unsafe.
     * use scanf for most of the value retrievers.
     */
    double GetFloat(const int index);
    
    /**
     * unsafe get string function. We shouldn't use this function much as it is unsafe.
     * use scanf for most of the value retrievers.
     */
    std::string GetString(const int index);

    /**
     * get item as std::string. This function we can consider safe as we can check if a
     * retrieve action has been successful.
     */
    bool GetString(const int index, std::string& rStr);

    template<typename T>
    void set_item(const int index, T* pObject);

    // generic pObject setters..... because it makes the code a bit cleaner
    void set_str(const int index, const char* pStr);
    void set_str(const int index, const char* pStr, const size_t len);
    void set_int(const int index, const int number);
    void set_long(const int index, const long number);

    /**
     * \brief a VA function for getting multiple pObject from a tuple.
     *
     * a VA function for getting multiple pObject from a tuple.
     *
     * @param[in] format is the VA string containing the expected pObject types.
     * @param[out] ... this VA field contains the pointers to the VA items.
     * @return the number of 'scanned' objects and 0 of a error has happened.
     */
    int scanf(const char * format, ...);

    /**
     * @brief this function returns the amount of objects.
     * @return the element count
     */
    size_t size();

    /**
     * @brief clears the tuple from all objects
     */
    void clear();

    /**
     * @brief this function resizes the tuple. Objects that are lost will get a decrease in there reference.
     */
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

    /**
     * @brief tuple hash function
     */ 
    uint32 _hash();
};

#pragma pack(pop)

#endif //_PYTUPLE_H
