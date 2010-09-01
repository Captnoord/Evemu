/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2008 The EVEmu Team
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
    Author:     Aim, Captnoord, Zhur, Bloody.Rabbit
*/

#include "CommonPCH.h"

#include "fs/Utils.h"

#ifdef WIN32
int mkdir( const char* pathname, int mode ) 
{ 
    // mkdir returns 0 for success, opposite of CreateDirectory().
    return ( CreateDirectory( pathname, NULL ) ? 0 : -1 ); 
} 
#endif /* WIN32 */

uint64 Fs::GetFilesize( const char* filename )
{
    FILE* fd = fopen( filename, "r" );
    if( fd == NULL )
        return 0;

    return Fs::GetFilesize( fd );
}

uint64 Fs::GetFilesize( FILE* fd )
{
#ifdef WIN32
	return _filelength( _fileno( fd ) );
#else
	struct stat file_stat;
	fstat( fileno( fd ), &file_stat );
	return file_stat.st_size;
#endif
}
