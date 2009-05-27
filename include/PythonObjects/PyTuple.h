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
 * @brief PyTuple is one of the most used objects within python. Its a object to store objects in.
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
     * \brief operator overload for easy object access and storage
     *
     * nothing much to tell about this function, it returns a PyChameleon object reference.
     *
     * @param[in] index is the location of the required object.
     * @return always returns a PyChameleon object even if there isn't a object stored (so it can be used to store objects).
     */
    PyChameleon &operator[](const int index);

    PyObject* GetItem(const int index);

    int GetInt(const int index);
    double GetFloat(const int index);
    /**
     * Slow GetString function
     */
    std::string GetString(const int index);

    /**
     * get item as std::string
     */
    bool GetString(const int index, std::string & str);

    template<typename T>
    void set_item(const int index, T * object);

    // generic object setters..... because it makes the code a bit cleaner
    void set_str(const int index, const char* str);
    void set_str(const int index, const char* str, const size_t len);
    void set_int(const int index, const int number);
    void set_long(const int index, const long number);

    /**
     * \brief a VA function for getting multiple object from a tuple.
     *
     * a VA function for getting multiple object from a tuple.
     *
     * @param[in] format is the VA string containing the expected object types.
     * @param[out] ... this VA field contains the pointers to the VA items.
     * @return the number of 'scanned' objects and 0 of a error has happened.
     */
    int scanf(const char * format, ...);

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

    // tuple hash function
    uint32 _hash();
};

#pragma pack(pop)

#endif //_PYTUPLE_H
