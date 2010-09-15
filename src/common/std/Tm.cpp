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

#include "CommonPCH.h"

#include "std/Tm.h"

/*************************************************************************/
/* Std::Tm                                                               */
/*************************************************************************/
Std::Tm Std::Tm::now()
{
    const time_t t = ::time( NULL );
    assert( -1 != t );

    return Tm( t );
}

Std::Tm::Tm()
{
}

Std::Tm::Tm( const tm& t )
: mTm( t )
{
}

Std::Tm::Tm( time_t t )
{
    const tm* result = ::localtime_r( &t, &mTm );
    assert( &mTm == result );
}

Std::Tm::Tm( const Tm& oth )
: mTm( oth.t() )
{
}
