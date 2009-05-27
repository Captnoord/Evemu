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

#ifndef _PYUNICODEUCS2_H
#define _PYUNICODEUCS2_H

class PyObject;

class PyUnicodeUCS2
{
    uint8 mType;
    size_t mRefcnt;
    wchar_t* mStr;
    size_t mStrLen;
    uint32 mHashValue; /* string hash cache */
    uint32 (PyUnicodeUCS2::*mHash)();
public:
    uint8 gettype();
    void IncRef();
    void DecRef();
    uint32 hash();
public:
    explicit PyUnicodeUCS2();
    explicit PyUnicodeUCS2(const wchar_t* str);
    explicit PyUnicodeUCS2(const wchar_t* str, size_t len);
    explicit PyUnicodeUCS2(std::wstring& str);
    ~PyUnicodeUCS2();
    bool set(const wchar_t* str, size_t len);
    bool resize(size_t newsize);
    wchar_t * content();
    const size_t length();

private:
    uint32 _hash();
};

/************************************************************************/
/* fake python api                                                      */
/************************************************************************/

/**
 * @brief
 *
 *
 *
 * @param[in]
 * @param[out]
 * @return
 */
PyUnicodeUCS2* PyUnicodeUCS2_FromWideChar(const wchar_t* str, size_t len);

/**
 * @brief Python API cloning, converting a UTF8 string to a unicode UCS2 string.
 *
 *
 *
 * @note this isn't as close to the native python implementation... we will see how far this will get us.
 * @param[in] str is the const char string that is fed into the function.
 * @param[in] len is the length of the char string that is converted by this function.
 * @return a freshly generated PyUnicodeUCS2 object from the char string
 * @note yea I know this function lacks 1 parameter, which is "const char *errors"
 */
PyUnicodeUCS2* PyUnicodeUCS2_DecodeUTF8(const char* str, size_t len);

PyObject *PyUnicode_AsUTF8String(PyObject *unicode);

#endif // _PYUNICODEUCS2_H
