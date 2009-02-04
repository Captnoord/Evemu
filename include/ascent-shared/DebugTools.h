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

#ifndef _DEBUGTOOLS_H
#define _DEBUGTOOLS_H

/* a small macro to add some evil debug options to some situations */
#ifdef WIN32
#  define ASCENT_HARDWARE_BREAKPOINT {sLog.Debug("hardware breakpoint"); __asm{int 3}}
#else
#  define ASCENT_HARDWARE_BREAKPOINT
#endif//WIN32

#define ASCENT_MALLOC malloc
#define ASCENT_FREE free
#define ASCENT_REALLOC realloc
#define ASCENT_MEMCPY memcpy

#endif //_DEBUGTOOLS_H
