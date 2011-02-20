/**
 * @file TestFsFile.cpp
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

#include "TestFsFile.h"

using namespace test;

/*************************************************************************/
/* test::TestFsFile                                                      */
/*************************************************************************/
const char TestFsFile::NAME[] = "TestFsFile.tmp";
const char TestFsFile::NAME_ALT[] = "TestFsFile-Alt.tmp";
const uint8 TestFsFile::PLAIN_DATA[] =
{
    0xA8, 0xDA, 0x06, 0x84, 0x43, 0x69, 0xC8,
};

CppUnit::TestSuite* TestFsFile::suite()
{
    CppUnit::TestSuite* s = new CppUnit::TestSuite( "TestFsFile" );

    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testRename",
                                                       &TestFsFile::testRename ) );
    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testRemove",
                                                       &TestFsFile::testRemove ) );

    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testOpenRead",
                                                       &TestFsFile::testOpenRead ) );
    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testOpenWrite",
                                                       &TestFsFile::testOpenWrite ) );
    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testOpenReadWrite",
                                                       &TestFsFile::testOpenReadWrite ) );

    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testOpenCreateSeparate",
                                                       &TestFsFile::testOpenCreateSeparate ) );
    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testOpenCreateCombined",
                                                       &TestFsFile::testOpenCreateCombined ) );
    s->addTest( new CppUnit::TestCaller< TestFsFile >( "TestFsFile::testOpenTruncate",
                                                       &TestFsFile::testOpenTruncate ) );

    return s;
}

void TestFsFile::setUp()
{
    mFile = new fs::File;
}

void TestFsFile::tearDown()
{
    util::safeDelete( mFile );
}

void TestFsFile::testRename()
{
    CPPUNIT_ASSERT( !fs::File::Rename( NAME, NAME_ALT ) );

    CPPUNIT_ASSERT( mFile->Open( NAME ) );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( fs::File::Rename( NAME, NAME_ALT ) );
    CPPUNIT_ASSERT( !fs::File::Rename( NAME, NAME_ALT ) );

    CPPUNIT_ASSERT( fs::File::Remove( NAME_ALT ) );
}

void TestFsFile::testRemove()
{
    CPPUNIT_ASSERT( !fs::File::Remove( NAME ) );

    CPPUNIT_ASSERT( mFile->Open( NAME ) );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
    CPPUNIT_ASSERT( !fs::File::Remove( NAME ) );
}

void TestFsFile::testOpenRead()
{
    fs::File::Mode mode = fs::File::Mode( fs::File::MODE_ACCESS_READ
                                          | fs::File::MODE_OPEN_DEFAULT );

    CPPUNIT_ASSERT( !mFile->isValid() );
    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );
    CPPUNIT_ASSERT( mFile->isValid() );

    size_t size = 0;
    util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( stream::ERROR_WRITE, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_EOS, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( !mFile->isValid() );

    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
}

void TestFsFile::testOpenWrite()
{
    fs::File::Mode mode = fs::File::Mode( fs::File::MODE_ACCESS_WRITE
                                          | fs::File::MODE_OPEN_DEFAULT );

    CPPUNIT_ASSERT( !mFile->isValid() );
    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );
    CPPUNIT_ASSERT( mFile->isValid() );

    size_t size = 0;
    util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Flush() );
    CPPUNIT_ASSERT( mFile->Seek( -sizeof( PLAIN_DATA ), fs::File::ORIGIN_CURRENT ) );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_READ, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( !mFile->isValid() );

    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
}

void TestFsFile::testOpenReadWrite()
{
    fs::File::Mode mode = fs::File::Mode( fs::File::MODE_ACCESS_READ
                                          | fs::File::MODE_ACCESS_WRITE
                                          | fs::File::MODE_OPEN_DEFAULT );

    CPPUNIT_ASSERT( !mFile->isValid() );
    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );
    CPPUNIT_ASSERT( mFile->isValid() );

    size_t size = 0;
    util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Flush() );
    CPPUNIT_ASSERT( mFile->Seek( -sizeof( PLAIN_DATA ), fs::File::ORIGIN_CURRENT ) );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_EOS, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( !mFile->isValid() );

    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
}

void TestFsFile::testOpenCreateSeparate()
{
    fs::File::Mode modeCreate = fs::File::Mode( fs::File::MODE_ACCESS_DEFAULT
                                                | fs::File::MODE_OPEN_NEW );
    fs::File::Mode modeOpen = fs::File::Mode( fs::File::MODE_ACCESS_DEFAULT
                                              | fs::File::MODE_OPEN_EXISTING );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeOpen ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeCreate ) );

    size_t size = 0;
    util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeCreate ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeOpen ) );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
}

void TestFsFile::testOpenCreateCombined()
{
    fs::File::Mode mode = fs::File::Mode( fs::File::MODE_ACCESS_DEFAULT
                                          | fs::File::MODE_OPEN_NEW
                                          | fs::File::MODE_OPEN_EXISTING );

    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );

    size_t size = 0;
    util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( mFile->Open( NAME, mode ) );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
}

void TestFsFile::testOpenTruncate()
{
    fs::File::Mode modeCreate = fs::File::Mode( fs::File::MODE_ACCESS_DEFAULT
                                                | fs::File::MODE_OPEN_NEW );
    fs::File::Mode modeTruncate = fs::File::Mode( fs::File::MODE_ACCESS_DEFAULT
                                                  | fs::File::MODE_OPEN_TRUNCATE );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeTruncate ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeCreate ) );

    size_t size = 0;
    util::Buffer buf( PLAIN_DATA, PLAIN_DATA + sizeof( PLAIN_DATA ) );
    CPPUNIT_ASSERT_EQUAL( stream::ERROR_OK, mFile->Write( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( sizeof( PLAIN_DATA ), size );
    CPPUNIT_ASSERT( mFile->Close() );

    CPPUNIT_ASSERT( !mFile->Open( NAME, modeCreate ) );
    CPPUNIT_ASSERT( mFile->Open( NAME, modeTruncate ) );

    CPPUNIT_ASSERT_EQUAL( stream::ERROR_EOS, mFile->Read( buf, &size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->GetSize( size ) );
    CPPUNIT_ASSERT_EQUAL( 0UL, size );

    CPPUNIT_ASSERT( mFile->Close() );
    CPPUNIT_ASSERT( fs::File::Remove( NAME ) );
}
