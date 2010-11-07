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

#include "TestFsFile.h"

/*************************************************************************/
/* Util::Test< Fs::File >                                                */
/*************************************************************************/
const char Util::Test< Fs::File >::NAME[] = "TestFsFile.tmp";
const char Util::Test< Fs::File >::NAME_ALT[] = "TestFsFile-Alt.tmp";
const uint8 Util::Test< Fs::File >::PLAIN_DATA[] =
{
    0xA8, 0xDA, 0x06, 0x84, 0x43, 0x69, 0xC8,
};

CppUnit::TestSuite* Util::Test< Fs::File >::suite()
{
    CppUnit::TestSuite* s = new CppUnit::TestSuite( "TestFsFile" );

    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testRename",
                                                                   &Util::Test< Fs::File >::testRename ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testRemove",
                                                                   &Util::Test< Fs::File >::testRemove ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testOpenRead",
                                                                   &Util::Test< Fs::File >::testOpenRead ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testOpenWrite",
                                                                   &Util::Test< Fs::File >::testOpenWrite ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testOpenReadWrite",
                                                                   &Util::Test< Fs::File >::testOpenReadWrite ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testOpenCreateSeparate",
                                                                   &Util::Test< Fs::File >::testOpenCreateSeparate ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testOpenCreateCombined",
                                                                   &Util::Test< Fs::File >::testOpenCreateCombined ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Fs::File > >( "TestFsFile::testOpenTruncate",
                                                                   &Util::Test< Fs::File >::testOpenTruncate ) );

    return s;
}

void Util::Test< Fs::File >::setUp()
{
    mFile = new Fs::File;
}

void Util::Test< Fs::File >::tearDown()
{
    SafeDelete( mFile );
}

void Util::Test< Fs::File >::testRename()
{
    CPPUNIT_ASSERT( !Fs::File::Rename( NAME, NAME_ALT ) );

    CPPUNIT_ASSERT( mFile->Open( NAME ) );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( Fs::File::Rename( NAME, NAME_ALT ) );
    CPPUNIT_ASSERT( !Fs::File::Rename( NAME, NAME_ALT ) );

    CPPUNIT_ASSERT( Fs::File::Remove( NAME_ALT ) );
}

void Util::Test< Fs::File >::testRemove()
{
    CPPUNIT_ASSERT( !Fs::File::Remove( NAME ) );

    CPPUNIT_ASSERT( mFile->Open( NAME ) );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
    CPPUNIT_ASSERT( !Fs::File::Remove( NAME ) );
}

void Util::Test< Fs::File >::testOpenRead()
{
    Fs::File::Mode mode = Fs::File::Mode( Fs::File::MODE_ACCESS_READ
                                          | Fs::File::MODE_OPEN_DEFAULT );

    CPPUNIT_ASSERT( !mFile->isValid() );
    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );
    CPPUNIT_ASSERT( mFile->isValid() );

    size_t size = 0;
    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_WRITE, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_EOS, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( !mFile->isValid() );

    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
}

void Util::Test< Fs::File >::testOpenWrite()
{
    Fs::File::Mode mode = Fs::File::Mode( Fs::File::MODE_ACCESS_WRITE
                                          | Fs::File::MODE_OPEN_DEFAULT );

    CPPUNIT_ASSERT( !mFile->isValid() );
    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );
    CPPUNIT_ASSERT( mFile->isValid() );

    size_t size = 0;
    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Flush() );
    CPPUNIT_ASSERT( mFile->Seek( -sizeof( PLAIN_DATA ), Fs::File::ORIGIN_CURRENT ) );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_READ, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( !mFile->isValid() );

    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
}

void Util::Test< Fs::File >::testOpenReadWrite()
{
    Fs::File::Mode mode = Fs::File::Mode( Fs::File::MODE_ACCESS_READ
                                          | Fs::File::MODE_ACCESS_WRITE
                                          | Fs::File::MODE_OPEN_DEFAULT );

    CPPUNIT_ASSERT( !mFile->isValid() );
    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );
    CPPUNIT_ASSERT( mFile->isValid() );

    size_t size = 0;
    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Flush() );
    CPPUNIT_ASSERT( mFile->Seek( -sizeof( PLAIN_DATA ), Fs::File::ORIGIN_CURRENT ) );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_EOS, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( !mFile->isValid() );

    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
}

void Util::Test< Fs::File >::testOpenCreateSeparate()
{
    Fs::File::Mode modeCreate = Fs::File::Mode( Fs::File::MODE_ACCESS_DEFAULT
                                                | Fs::File::MODE_OPEN_NEW );
    Fs::File::Mode modeOpen = Fs::File::Mode( Fs::File::MODE_ACCESS_DEFAULT
                                              | Fs::File::MODE_OPEN_EXISTING );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeOpen ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeCreate ) );

    size_t size = 0;
    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeCreate ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeOpen ) );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
}

void Util::Test< Fs::File >::testOpenCreateCombined()
{
    Fs::File::Mode mode = Fs::File::Mode( Fs::File::MODE_ACCESS_DEFAULT
                                          | Fs::File::MODE_OPEN_NEW
                                          | Fs::File::MODE_OPEN_EXISTING );

    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );

    size_t size = 0;
    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
}

void Util::Test< Fs::File >::testOpenTruncate()
{
    Fs::File::Mode modeCreate = Fs::File::Mode( Fs::File::MODE_ACCESS_DEFAULT
                                                | Fs::File::MODE_OPEN_NEW );
    Fs::File::Mode modeTruncate = Fs::File::Mode( Fs::File::MODE_ACCESS_DEFAULT
                                                  | Fs::File::MODE_OPEN_TRUNCATE );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeTruncate ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeCreate ) );

    size_t size = 0;
    Util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeCreate ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeTruncate ) );

    CPPUNIT_ASSERT_EQUAL( Fs::File::ERROR_EOS, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( Fs::File::Remove( NAME ) );
}
