/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _UTIL_H
#define _UTIL_H

#include "Common.h"

#ifdef WIN32
#  define KILL_THIS_PROCESS TerminateProcess(GetCurrentProcess(), 0);
#else
#  define KILL_THIS_PROCESS kill(getpid(), -9);
#endif

///////////////////////////////////////////////////////////////////////////////
// String Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
	SERVER_DECL std::vector<std::string> StrSplit(const std::string &src, const std::string &sep);

	// This HAS to be called outside the threads __try / __except block!
	SERVER_DECL void SetThreadName(const char* format, ...);
	SERVER_DECL time_t convTimePeriod ( uint32 dLength, char dType);
	SERVER_DECL int32 ascent_roundf(float v);
	SERVER_DECL int32 ascent_roundd(double v);
	SERVER_DECL bool CheckIPs(const char* szIPList);
	SERVER_DECL int get_tokens(const char* szInput, char** pOutput, int iMaxCount, char cSeperator);

#ifdef WIN32
	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in user addr space)
		DWORD dwThreadID; // thread ID (-1=caller thread)
		DWORD dwFlags; // reserved for future use, must be zero
	} THREADNAME_INFO;
#endif

	int32 GetTimePeriodFromString(const char * str);
	std::string ConvertTimeStampToString(uint32 timestamp);
	std::string ConvertTimeStampToStringNC(uint32 timestamp);
	std::string ConvertTimeStampToDataTime(uint32 timestamp);

	// returns true if the ip hits the mask, otherwise false
	bool ParseCIDRBan(unsigned int IP, unsigned int Mask, unsigned int MaskBits);
	unsigned int MakeIP(const char * str);
#endif
