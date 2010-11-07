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

#include "TestCommonPCH.h"

#include "TestStdFile.h"

/*************************************************************************/
/* Util::Test< Std::File >                                               */
/*************************************************************************/
const char Util::Test< Std::File >::NAME[] = "TestStdFile.tmp";
const char Util::Test< Std::File >::NAME_ALT[] = "TestStdFile-Alt.tmp";
const uint8 Util::Test< Std::File >::PLAIN_DATA[] =
{
    0xA8, 0xDA, 0x06, 0x84, 0x43, 0x69, 0xC8,
};

Util::Test< Std::File >::Test()
: CppUnit::TestCase( "TestStdFile" )
{
}

void Util::Test< Std::File >::runTest()
{
    CPPUNIT_ASSERT( mFile.Open( NAME, "wb" ) );

    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );

    CPPUNIT_ASSERT_EQUAL( Std::File::ERROR_OK, mFile.Write( buf ) );
    CPPUNIT_ASSERT( mFile.Flush() );
    CPPUNIT_ASSERT_EQUAL( buf.size(), static_cast< size_t >( mFile.tell() ) );

    CPPUNIT_ASSERT( mFile.Close() );
    CPPUNIT_ASSERT_EQUAL( 0, Std::File::Rename( NAME, NAME_ALT ) );

    CPPUNIT_ASSERT( !mFile.Open( NAME, "rb" ) );
    CPPUNIT_ASSERT( mFile.Open( NAME_ALT, "rb" ) );

    CPPUNIT_ASSERT_EQUAL( Std::File::ERROR_OK, mFile.Read( buf ) );
    CPPUNIT_ASSERT_EQUAL( buf.size(), static_cast< size_t >( mFile.tell() ) );
    CPPUNIT_ASSERT( std::equal( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ),
                                buf.begin< uint8 >() ) );

    CPPUNIT_ASSERT_EQUAL( Std::File::ERROR_EOS, mFile.Read( buf ) );
    CPPUNIT_ASSERT( mFile.eof() );
    CPPUNIT_ASSERT_EQUAL( buf.size(), static_cast< size_t >( mFile.tell() ) );

    CPPUNIT_ASSERT( mFile.Seek( 0, SEEK_SET ) );
    CPPUNIT_ASSERT_EQUAL( 0L, mFile.tell() );

    CPPUNIT_ASSERT( mFile.Close() );
    CPPUNIT_ASSERT_EQUAL( 0, Std::File::Remove( NAME_ALT ) );
    CPPUNIT_ASSERT( !mFile.Open( NAME_ALT, "rb" ) );
}
