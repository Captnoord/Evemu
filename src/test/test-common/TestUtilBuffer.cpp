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

#include "TestCommon.h"

#include "TestUtilBuffer.h"

using namespace test;

/*************************************************************************/
/* test::TestUtilBuffer                                                  */
/*************************************************************************/
const uint8 TestUtilBuffer::PLAIN_DATA[] =
{
    0xA8, 0xDA, 0x06, 0x84, 0x43, 0x69, 0xC8
};

CppUnit::TestSuite* TestUtilBuffer::suite()
{
    CppUnit::TestSuite* s = new CppUnit::TestSuite( "TestUtilBuffer" );

    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testConstructorSize",
                                                           &TestUtilBuffer::testConstructorSize ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testConstructorRange",
                                                           &TestUtilBuffer::testConstructorRange ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testConstructorCopy",
                                                           &TestUtilBuffer::testConstructorCopy ) );

    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testOperatorAppend",
                                                           &TestUtilBuffer::testOperatorAppend ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testOperatorAssign",
                                                           &TestUtilBuffer::testOperatorAssign ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testOperatorCopy",
                                                           &TestUtilBuffer::testOperatorCopy ) );

    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testAppendValue",
                                                           &TestUtilBuffer::testAppendValue ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testAppendRange",
                                                           &TestUtilBuffer::testAppendRange ) );

    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testAssignValue",
                                                           &TestUtilBuffer::testAssignValue ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testAssignRange",
                                                           &TestUtilBuffer::testAssignRange ) );

    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testReserve",
                                                           &TestUtilBuffer::testReserve ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testReserveAt",
                                                           &TestUtilBuffer::testReserveAt ) );

    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testResize",
                                                           &TestUtilBuffer::testResize ) );
    s->addTest( new CppUnit::TestCaller< TestUtilBuffer >( "TestUtilBuffer::testResizeAt",
                                                           &TestUtilBuffer::testResizeAt ) );

    return s;
}

void TestUtilBuffer::setUp()
{
    mBuffer = new util::Buffer;
}

void TestUtilBuffer::tearDown()
{
    SafeDelete( mBuffer );
}

void TestUtilBuffer::testConstructorSize()
{
    const size_t size = 8;
    const uint8 fill = PLAIN_DATA[ 5 ];

    SafeDelete( mBuffer );
    mBuffer = new util::Buffer( size, fill );

    CPPUNIT_ASSERT_EQUAL( size * sizeof( uint8 ), mBuffer->size() );
    CPPUNIT_ASSERT_EQUAL( fill, mBuffer->Get< uint8 >( size - 1 ) );
}

void TestUtilBuffer::testConstructorRange()
{
    const uint32* begin = reinterpret_cast< const uint32* >( &PLAIN_DATA[ 0 ] );
    const uint32* end = reinterpret_cast< const uint32* >( &PLAIN_DATA[ 7 ] );

    SafeDelete( mBuffer );
    mBuffer = new util::Buffer( begin, end );

    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ) * static_cast< size_t >( end - begin ),
                          mBuffer->size() );
    /*
     * The primary iterators must be the Buffer's ones here, because
     * the way pointer arithmetic works and the way std::equal is
     * implemented guarantee going out-of-bounds.
     */
    CPPUNIT_ASSERT( std::equal( mBuffer->begin< uint32 >(),
                                mBuffer->end< uint32 >(),
                                begin ) );
}

void TestUtilBuffer::testConstructorCopy()
{
    const util::Buffer buf( &PLAIN_DATA[ 0 ], &PLAIN_DATA[ 7 ] );

    SafeDelete( mBuffer );
    mBuffer = new util::Buffer( buf );

    CPPUNIT_ASSERT_EQUAL( buf.size(), mBuffer->size() );
    CPPUNIT_ASSERT( std::equal( buf.begin< uint8 >(), buf.end< uint8 >(),
                                mBuffer->begin< uint8 >() ) );
}

void TestUtilBuffer::testOperatorAppend()
{
    const util::Buffer::ConstIterator< uint32 > itr = mBuffer->end< uint32 >();
    const uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 1 ] );

    *mBuffer << val;

    CPPUNIT_ASSERT( mBuffer->end< uint32 >() == itr + 1 );
    CPPUNIT_ASSERT_EQUAL( val, *itr );
}

void TestUtilBuffer::testOperatorAssign()
{
    const uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 3 ] );

    *mBuffer = val;

    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ), mBuffer->size() );
    CPPUNIT_ASSERT_EQUAL( val, mBuffer->Get< uint32 >( 0 ) );
}

void TestUtilBuffer::testOperatorCopy()
{
    const util::Buffer buf( &PLAIN_DATA[ 0 ], &PLAIN_DATA[ 7 ] );

    *mBuffer = buf;

    CPPUNIT_ASSERT_EQUAL( buf.size(), mBuffer->size() );
    CPPUNIT_ASSERT( std::equal( buf.begin< uint8 >(), buf.end< uint8 >(),
                                mBuffer->begin< uint8 >() ) );
}

void TestUtilBuffer::testAppendValue()
{
    const util::Buffer::ConstIterator< uint32 > itr = mBuffer->end< uint32 >();
    const uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 2 ] );

    mBuffer->Append( val );

    CPPUNIT_ASSERT( mBuffer->end< uint32 >() == itr + 1 );
    CPPUNIT_ASSERT_EQUAL( val, *itr );
}

void TestUtilBuffer::testAppendRange()
{
    const util::Buffer::Iterator< uint32 > itr = mBuffer->end< uint32 >();
    const uint32* begin = reinterpret_cast< const uint32* >( &PLAIN_DATA[ 0 ] );
    const uint32* end = reinterpret_cast< const uint32* >( &PLAIN_DATA[ 7 ] );

    mBuffer->Append( begin, end );

    CPPUNIT_ASSERT( mBuffer->end< uint32 >() == itr + ( end - begin ) );
    /*
     * The primary iterators must be the Buffer's ones here, because
     * the way pointer arithmetic works and the way std::equal is
     * implemented guarantee going out-of-bounds.
     */
    CPPUNIT_ASSERT( std::equal( itr, mBuffer->end< uint32 >(), begin ) );
}

void TestUtilBuffer::testAssignValue()
{
    const uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 1 ] );

    mBuffer->Assign( val );

    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ), mBuffer->size() );
    CPPUNIT_ASSERT_EQUAL( val, mBuffer->Get< uint32 >( 0 ) );
}

void TestUtilBuffer::testAssignRange()
{
    const uint32* begin = reinterpret_cast< const uint32* >( &PLAIN_DATA[ 0 ] );
    const uint32* end = reinterpret_cast< const uint32* >( &PLAIN_DATA[ 7 ] );

    mBuffer->Assign( begin, end );
                     
    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ) * static_cast< size_t >( end - begin ),
                          mBuffer->size() );
    /*
     * The primary iterators must be the Buffer's ones here, because
     * the way pointer arithmetic works and the way std::equal is
     * implemented guarantee going out-of-bounds.
     */
    CPPUNIT_ASSERT( std::equal( mBuffer->begin< uint32 >(),
                                mBuffer->end< uint32 >(),
                                begin ) );
}

void TestUtilBuffer::testReserve()
{
    const size_t count = 4;

    mBuffer->Reserve< uint32 >( count );

    CPPUNIT_ASSERT( count * sizeof( uint32 ) <= mBuffer->capacity() );
}

void TestUtilBuffer::testReserveAt()
{
    mBuffer->Resize< uint32 >( 4 );
    const util::Buffer::ConstIterator< uint32 > itr = mBuffer->begin< uint32 >() + 4;
    const size_t count = 4;

    mBuffer->ReserveAt< uint32 >( itr, count );

    CPPUNIT_ASSERT( ( count + 4 ) * sizeof( uint32 ) <= mBuffer->capacity() );
}

void TestUtilBuffer::testResize()
{
    mBuffer->Resize< uint32 >( 0 );
    const size_t size = 4;
    const uint32 fill = reinterpret_cast< const uint32& >( PLAIN_DATA[ 0 ] );

    mBuffer->Resize( size, fill );

    CPPUNIT_ASSERT_EQUAL( size * sizeof( uint32 ), mBuffer->size() );
    CPPUNIT_ASSERT_EQUAL( fill, mBuffer->Get< uint32 >( size - 1 ) );
}

void TestUtilBuffer::testResizeAt()
{
    mBuffer->Resize< uint32 >( 4 );
    const util::Buffer::ConstIterator< uint32 > itr = mBuffer->begin< uint32 >() + 4;
    const size_t size = 4;
    const uint32 fill = reinterpret_cast< const uint32& >( PLAIN_DATA[ 1 ] );

    mBuffer->ResizeAt( itr, size, fill );

    CPPUNIT_ASSERT_EQUAL( ( size + 4 ) * sizeof( uint32 ), mBuffer->size() );
    CPPUNIT_ASSERT_EQUAL( fill, mBuffer->Get< uint32 >( size + 3 ) );
}
