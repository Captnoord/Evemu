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
	Author:		Captnoord
*/

#include "EvemuPCH.h"

/************************************************************************/
/* Py Unicode object                                                    */
/************************************************************************/
PyUnicodeUCS2::PyUnicodeUCS2() : mType(PyTypeUnicode), mStr(NULL), mStrLen(0), mHashValue(0), mRefcnt(1)
{
	mHash = &PyUnicodeUCS2::_hash;
}
PyUnicodeUCS2::PyUnicodeUCS2(const wchar_t* str) : mType(PyTypeUnicode), mStrLen(0), mStr(NULL), mHashValue(0),mRefcnt(1)
{
	mHash = &PyUnicodeUCS2::_hash;
	size_t len = wcslen(str);
	set(str, len);
}

PyUnicodeUCS2::PyUnicodeUCS2(const wchar_t* str, size_t len) : mType(PyTypeUnicode), mStr(NULL), mHashValue(0), mRefcnt(1)
{
	mHash = &PyUnicodeUCS2::_hash;
	set(str, len);
}

PyUnicodeUCS2::PyUnicodeUCS2(std::wstring& str) : mType(PyTypeUnicode), mStr(NULL), mHashValue(0) , mRefcnt(1)
{
	mHash = &PyUnicodeUCS2::_hash;
	set(str.c_str(), str.length());
}

PyUnicodeUCS2::~PyUnicodeUCS2()
{
	SafeFree(mStr);
	mType = PyTypeDeleted;
	mHash = NULL;
}

bool PyUnicodeUCS2::set( const wchar_t* str, size_t len )
{
	if (str == NULL || len == 0)
		return false;

	if (resize(len) == false)
		return false;

	ASCENT_MEMCPY(mStr, str, len * 2);
	mStr[len] = '\0';
	return true;
}

bool PyUnicodeUCS2::resize(size_t newsize)
{
#ifndef PACKET_PARSER_MEMORY_ON_DRUGS
	if (mStr != NULL)
		mStr = (wchar_t*)ASCENT_REALLOC(mStr,(newsize+1) * 2);
	else
		mStr = (wchar_t*)ASCENT_MALLOC((newsize+1) * 2);
#else
	if (mStr != NULL)
		sBufferPool.ResizeBuffer((void**)&mStr, (mStrLen+1) * 2, (newsize+1) * 2);
	else
		mStr = (wchar_t*)sBufferPool.GetBuffer((newsize+1) * 2);
#endif

	mStrLen = newsize;

	if (mStr == NULL)
	{
		mStrLen = 0;
		return false;
	}

	return true;
}

wchar_t* PyUnicodeUCS2::content()
{
	return mStr;
}

uint8 PyUnicodeUCS2::gettype()
{
	return mType;
}

void PyUnicodeUCS2::IncRef()
{
	mRefcnt++;
}

void PyUnicodeUCS2::DecRef()
{
	mRefcnt--;
	if (mRefcnt <= 0)
		PyDelete(this);
}

const size_t PyUnicodeUCS2::length()
{
	return mStrLen;
}

uint32 PyUnicodeUCS2::hash()
{
	return (this->*mHash)();
}

/* how in gods name do I have to hash a empty string? */
uint32 PyUnicodeUCS2::_hash()
{
	if (mHashValue == 0)
	{
		if (mStr != NULL)
			mHashValue = Utils::Hash::sdbm_hash(mStr);
		else
			mHashValue = Utils::Hash::sdbm_hash("PyUnicodeUCS2::EmptyString");
		return mHashValue;
	}
	else
	{
		return mHashValue;
	}
}