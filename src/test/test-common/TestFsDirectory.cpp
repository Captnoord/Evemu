/**
 * @file TestFsDirectory.cpp
 *
 * This file is part of EVEmu: EVE Online Server Emulator.<br>
 * Copyright (C) 2006-2011 The EVEmu Team<br>
 * For the latest information visit <i>http://evemu.org</i>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA, or go to
 * <i>http://www.gnu.org/copyleft/lesser.txt</i>.
 *
 * @author Bloody.Rabbit
 */

#include "TestCommon.h"

#include "TestFsDirectory.h"

using namespace test;

/*************************************************************************/
/* test::TestFsDirectory                                                 */
/*************************************************************************/
const char TestFsDirectory::NAME_TOP[] = "TestFsDirectory-Top";
const char TestFsDirectory::NAME_BOTTOM[] = "TestFsDirectory-Top/TestFsDirectory-Bottom";

TestFsDirectory::TestFsDirectory()
: CppUnit::TestCase( "TestFsDirectory" )
{
}

void TestFsDirectory::runTest()
{
    char name[ FILENAME_MAX ];
    CPPUNIT_ASSERT( !mDirectory.isValid() );
    CPPUNIT_ASSERT( !mDirectory.Open( NAME_TOP, name, sizeof( name ) ) );
    CPPUNIT_ASSERT( !mDirectory.isValid() );

    CPPUNIT_ASSERT( !fs::Directory::Create( NAME_BOTTOM ) );
    CPPUNIT_ASSERT( fs::Directory::Create( NAME_TOP ) );
    CPPUNIT_ASSERT( fs::Directory::Create( NAME_TOP ) );
    CPPUNIT_ASSERT( fs::Directory::Create( NAME_BOTTOM ) );

    CPPUNIT_ASSERT( mDirectory.Open( NAME_TOP, name, sizeof( name ) ) );
    CPPUNIT_ASSERT( mDirectory.isValid() );

    bool subdirSeen = false;
    const char* subdirName = NAME_BOTTOM + sizeof( NAME_TOP );
    do
    {
        if( !subdirSeen )
        {
            if( 0 == ::strcmp( subdirName, name ) )
                subdirSeen = true;
        }
    } while( mDirectory.Next( name, sizeof( name ) ) );

    CPPUNIT_ASSERT( subdirSeen );

    CPPUNIT_ASSERT( mDirectory.isValid() );
    CPPUNIT_ASSERT( mDirectory.Close() );
    CPPUNIT_ASSERT( !mDirectory.isValid() );

    CPPUNIT_ASSERT( !fs::Directory::Remove( NAME_TOP ) );
    CPPUNIT_ASSERT( fs::Directory::Remove( NAME_BOTTOM ) );
    CPPUNIT_ASSERT( !fs::Directory::Remove( NAME_BOTTOM ) );
    CPPUNIT_ASSERT( fs::Directory::Remove( NAME_TOP ) );

    CPPUNIT_ASSERT( !mDirectory.Open( NAME_TOP, name, sizeof( name ) ) );
}
