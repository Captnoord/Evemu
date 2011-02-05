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

#include "CommonStd.h"

#include "db/Type.h"

/*************************************************************************/
/* Db                                                                    */
/*************************************************************************/
uint8 Db::TYPE_GetSizeBits( Db::TYPE type )
{
    switch( type )
    {
        case Db::TYPE_I8:
        case Db::TYPE_UI8:
        case Db::TYPE_R8:
        case Db::TYPE_CY:
        case Db::TYPE_FILETIME:
            return 64;
        case Db::TYPE_I4:
        case Db::TYPE_UI4:
        case Db::TYPE_R4:
            return 32;
        case Db::TYPE_I2:
        case Db::TYPE_UI2:
            return 16;
        case Db::TYPE_I1:
        case Db::TYPE_UI1:
            return 8;
        case Db::TYPE_BOOL:
            return 1;
        case Db::TYPE_BYTES:
        case Db::TYPE_STR:
        case Db::TYPE_WSTR:
            return 0;
    }

    return 0;
}

uint8 Db::TYPE_GetSizeBytes( Db::TYPE type )
{
    return ( ( Db::TYPE_GetSizeBits( type ) + 7 ) >> 3 );
}
