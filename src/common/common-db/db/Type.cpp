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
    Author:     Bloody.Rabbit
*/

#include "CommonDb.h"

#include "db/Type.h"

using namespace common;
using namespace common::db;

/*************************************************************************/
/* common::db                                                            */
/*************************************************************************/
uint8 db::TYPE_GetSizeBits( TYPE type )
{
    switch( type )
    {
        case TYPE_I8:
        case TYPE_UI8:
        case TYPE_R8:
        case TYPE_CY:
        case TYPE_FILETIME:
            return 64;
        case TYPE_I4:
        case TYPE_UI4:
        case TYPE_R4:
            return 32;
        case TYPE_I2:
        case TYPE_UI2:
            return 16;
        case TYPE_I1:
        case TYPE_UI1:
            return 8;
        case TYPE_BOOL:
            return 1;
        case TYPE_BYTES:
        case TYPE_STR:
        case TYPE_WSTR:
            return 0;
    }

    return 0;
}

uint8 db::TYPE_GetSizeBytes( TYPE type )
{
    return ( TYPE_GetSizeBits( type ) + 7 ) >> 3;
}
