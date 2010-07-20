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

#ifndef __GANGSTA__GA_UTILS_H__INCL__
#define __GANGSTA__GA_UTILS_H__INCL__

//typedef Ga::GaVec3 GPoint;

class GPoint
: public Ga::GaVec3
{
public:
    GPoint()
    : Ga::GaVec3()
    {
    }
    GPoint( Ga::GaFloat v )
    : Ga::GaVec3( v )
    {
    }
    GPoint( const Ga::GaFloat* v )
    : Ga::GaVec3( v )
    {
    }
    GPoint( Ga::GaFloat X, Ga::GaFloat Y, Ga::GaFloat Z )
    : Ga::GaVec3( X, Y, Z )
    {
    }
    GPoint( const GPoint& oth )
    : Ga::GaVec3( oth )
    {
    }
    GPoint( const Ga::GaVec3& oth )
    : Ga::GaVec3( oth )
    {
    }
};

class GVector
: public Ga::GaVec3
{
public:
    GVector()
    : Ga::GaVec3()
    {
    }
    GVector( Ga::GaFloat v )
    : Ga::GaVec3( v )
    {
    }
    GVector( const Ga::GaFloat* v )
    : Ga::GaVec3( v )
    {
    }
    GVector( Ga::GaFloat X, Ga::GaFloat Y, Ga::GaFloat Z )
    : Ga::GaVec3( X, Y, Z )
    {
    }
    GVector( const GPoint& oth )
    : Ga::GaVec3( oth )
    {
    }
    GVector( const Ga::GaVec3& oth )
    : Ga::GaVec3( oth )
    {
    }
    GVector( const GPoint& from, const GPoint& to )
    : Ga::GaVec3( ( to.x - from.x ),
                  ( to.y - from.y ),
                  ( to.z - from.z ) )
    {
    }
};

#endif /* !__GANGSTA__GA_UTILS_H__INCL__ */
