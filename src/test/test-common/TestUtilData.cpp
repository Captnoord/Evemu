/**
 * @file TestUtilData.cpp
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

#include "TestUtilData.h"

using namespace test;

/*************************************************************************/
/* test::TestUtilData                                                    */
/*************************************************************************/
uint8 TestUtilData::PLAIN_DATA[] =
{
    0xA8, 0xDA, 0x06, 0x84, 0x43, 0x69, 0xC8,
};

CppUnit::TestSuite* TestUtilData::suite()
{
    CppUnit::TestSuite* s = new CppUnit::TestSuite( "TestUtilData" );

    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testConstructorDefault",
                                                         &TestUtilData::testConstructorDefault ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testConstructorValue",
                                                         &TestUtilData::testConstructorValue ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testConstructorRange",
                                                         &TestUtilData::testConstructorRange ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testConstructorCopy",
                                                         &TestUtilData::testConstructorCopy ) );

    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testGet",
                                                         &TestUtilData::testGet ) );

    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testOperatorSubscript",
                                                         &TestUtilData::testOperatorSubscript ) );

    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorConstructorCopy",
                                                         &TestUtilData::testIteratorConstructorCopy ) );

    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorOperatorCopy",
                                                         &TestUtilData::testIteratorOperatorCopy ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorOperatorDereference",
                                                         &TestUtilData::testIteratorOperatorDereference ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorOperatorSubscript",
                                                         &TestUtilData::testIteratorOperatorSubscript ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorOperatorComparison",
                                                         &TestUtilData::testIteratorOperatorComparison ) );
    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorOperatorArithmetic",
                                                         &TestUtilData::testIteratorOperatorArithmetic ) );

    s->addTest( new CppUnit::TestCaller< TestUtilData >( "TestUtilData::testIteratorAs",
                                                         &TestUtilData::testIteratorAs ) );

    return s;
}

void TestUtilData::setUp()
{
    mData = new util::Data( &PLAIN_DATA[ 0 ], &PLAIN_DATA[ sizeof( PLAIN_DATA ) ] );
}

void TestUtilData::tearDown()
{
    util::safeDelete( mData );
}

void TestUtilData::testConstructorDefault()
{
    const size_t size = 0;

    util::safeDelete( mData );
    mData = new util::Data;

    CPPUNIT_ASSERT_EQUAL( size, mData->size() );

    // Revert changes
    tearDown();
    setUp();
}

void TestUtilData::testConstructorValue()
{
    uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 2 ] );

    util::safeDelete( mData );
    mData = new util::Data( val );

    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ), mData->size() );
    CPPUNIT_ASSERT_EQUAL( val, *mData->begin< uint32 >() );

    // Revert changes
    tearDown();
    setUp();
}

void TestUtilData::testConstructorRange()
{
    uint32* begin = reinterpret_cast< uint32* >( &PLAIN_DATA[ 0 ] );
    uint32* end = reinterpret_cast< uint32* >( &PLAIN_DATA[ 7 ] );

    util::safeDelete( mData );
    mData = new util::Data( begin, end );

    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ) * static_cast< size_t >( end - begin ),
                          mData->size() );
    /*
     * The primary iterators must be the Data's ones here, because
     * the way pointer arithmetic works and the way std::equal is
     * implemented guarantee going out-of-bounds.
     */
    CPPUNIT_ASSERT( std::equal( mData->begin< uint32 >(),
                                mData->end< uint32 >(),
                                begin ) );

    // Revert changes
    tearDown();
    setUp();
}

void TestUtilData::testConstructorCopy()
{
    const util::Data data( &PLAIN_DATA[ 0 ], &PLAIN_DATA[ sizeof( PLAIN_DATA ) ] );

    util::safeDelete( mData );
    mData = new util::Data( data );

    CPPUNIT_ASSERT_EQUAL( data.size(), mData->size() );
    CPPUNIT_ASSERT( std::equal( data.begin< uint8 >(), data.end< uint8 >(),
                                mData->begin< uint8 >() ) );

    // Revert changes
    tearDown();
    setUp();
}

void TestUtilData::testGet()
{
    uint32& ref = mData->Get< uint32 >( 0 );
    uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 0 ] );

    CPPUNIT_ASSERT_EQUAL( val, ref );

    ref = ++val;

    CPPUNIT_ASSERT_EQUAL( val, ref );
}

void TestUtilData::testOperatorSubscript()
{
    const size_t index = 3;
    uint8 val = PLAIN_DATA[ index ];

    CPPUNIT_ASSERT_EQUAL( val, ( *mData )[ index ] );

    ( *mData )[ index ] = --val;

    CPPUNIT_ASSERT_EQUAL( val, ( *mData )[ index ] );
}

void TestUtilData::testIteratorConstructorCopy()
{
    const util::Data::ConstIterator< uint32 > itr( mData->begin< uint32 >() );

    CPPUNIT_ASSERT( mData->begin< uint32 >() == itr );
}

void TestUtilData::testIteratorOperatorCopy()
{
    util::Data::ConstIterator< uint32 > itr;
    itr = mData->begin< uint32 >();

    CPPUNIT_ASSERT( mData->begin< uint32 >() == itr );
}

void TestUtilData::testIteratorOperatorDereference()
{
    const util::Data::Iterator< uint16 > itr = mData->begin< uint16 >();
    uint16 val = reinterpret_cast< const uint16& >( PLAIN_DATA[ 0 ] );

    CPPUNIT_ASSERT_EQUAL( val, *itr );

    *itr = ++val;

    CPPUNIT_ASSERT_EQUAL( val, *itr );
}

void TestUtilData::testIteratorOperatorSubscript()
{
    const util::Data::Iterator< uint16 > itr = mData->begin< uint16 >();
    uint16 val = reinterpret_cast< const uint16& >( PLAIN_DATA[ 4 ] );

    CPPUNIT_ASSERT_EQUAL( val, itr[ 2 ] );

    itr[ 2 ] = --val;

    CPPUNIT_ASSERT_EQUAL( val, itr[ 2 ] );
}

void TestUtilData::testIteratorOperatorComparison()
{
    const util::Data::ConstIterator< uint8 > begin = mData->begin< uint8 >();
    const util::Data::ConstIterator< uint8 > end = mData->end< uint8 >();

    CPPUNIT_ASSERT( !( begin == end ) );
    CPPUNIT_ASSERT( begin != end );

    CPPUNIT_ASSERT( begin < end );
    CPPUNIT_ASSERT( end > begin );

    CPPUNIT_ASSERT( begin <= end );
    CPPUNIT_ASSERT( end >= begin );

    const util::Data::ConstIterator< uint32 > itr1 = ++( begin.as< uint32 >() );
    const util::Data::ConstIterator< uint32 > itr2 = --( end.as< uint32 >() );

    CPPUNIT_ASSERT( itr1 == itr2 );
    CPPUNIT_ASSERT( !( itr1 != itr2 ) );

    CPPUNIT_ASSERT( !( itr1 < itr2 ) );
    CPPUNIT_ASSERT( !( itr2 > itr1 ) );

    CPPUNIT_ASSERT( itr1 <= itr2 );
    CPPUNIT_ASSERT( itr2 >= itr1 );
}

void TestUtilData::testIteratorOperatorArithmetic()
{
    util::Data::ConstIterator< uint8 > itr = mData->begin< uint8 >();

    itr += 2;
    ++itr;

    CPPUNIT_ASSERT( mData->begin< uint32 >() == itr.as< uint32 >() );

    itr = itr + 2 ;
    itr++;

    CPPUNIT_ASSERT( mData->begin< uint32 >() != itr.as< uint32 >() );

    itr -= 2;
    --itr;

    CPPUNIT_ASSERT( mData->begin< uint32 >() == itr.as< uint32 >() );

    itr = itr - 2;
    itr--;

    CPPUNIT_ASSERT( mData->begin< uint8 >() == itr );
}

void TestUtilData::testIteratorAs()
{
    const util::Data::ConstIterator< uint8 > itr = mData->begin< uint8 >() + 3;
    const uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 3 ] );

    CPPUNIT_ASSERT_EQUAL( val, *itr.as< uint32 >() );
}
