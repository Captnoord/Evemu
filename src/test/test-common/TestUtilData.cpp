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

#include "TestUtilData.h"

/*************************************************************************/
/* Util::Test< Util::Data >                                              */
/*************************************************************************/
uint8 Util::Test< Util::Data >::PLAIN_DATA[] =
{
    0xA8, 0xDA, 0x06, 0x84, 0x43, 0x69, 0xC8,
};

CppUnit::TestSuite* Util::Test< Util::Data >::suite()
{
    CppUnit::TestSuite* s = new CppUnit::TestSuite( "TestUtilData" );

    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testConstructorDefault",
                                                                     &Util::Test< Util::Data >::testConstructorDefault ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testConstructorValue",
                                                                     &Util::Test< Util::Data >::testConstructorValue ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testConstructorRange",
                                                                     &Util::Test< Util::Data >::testConstructorRange ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testConstructorCopy",
                                                                     &Util::Test< Util::Data >::testConstructorCopy ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testGet",
                                                                     &Util::Test< Util::Data >::testGet ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testOperatorSubscript",
                                                                     &Util::Test< Util::Data >::testOperatorSubscript ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorConstructorCopy",
                                                                     &Util::Test< Util::Data >::testIteratorConstructorCopy ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorOperatorCopy",
                                                                     &Util::Test< Util::Data >::testIteratorOperatorCopy ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorOperatorDereference",
                                                                     &Util::Test< Util::Data >::testIteratorOperatorDereference ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorOperatorSubscript",
                                                                     &Util::Test< Util::Data >::testIteratorOperatorSubscript ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorOperatorComparison",
                                                                     &Util::Test< Util::Data >::testIteratorOperatorComparison ) );
    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorOperatorArithmetic",
                                                                     &Util::Test< Util::Data >::testIteratorOperatorArithmetic ) );

    s->addTest( new CppUnit::TestCaller< Util::Test< Util::Data > >( "TestUtilData::testIteratorAs",
                                                                     &Util::Test< Util::Data >::testIteratorAs ) );

    return s;
}

void Util::Test< Util::Data >::setUp()
{
    mData = new Util::Data( &PLAIN_DATA[ 0 ], &PLAIN_DATA[ sizeof( PLAIN_DATA ) ] );
}

void Util::Test< Util::Data >::tearDown()
{
    SafeDelete( mData );
}

void Util::Test< Util::Data >::testConstructorDefault()
{
    const size_t size = 0;

    SafeDelete( mData );
    mData = new Util::Data;

    CPPUNIT_ASSERT_EQUAL( size, mData->size() );

    // Revert changes
    tearDown();
    setUp();
}

void Util::Test< Util::Data >::testConstructorValue()
{
    uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 2 ] );

    SafeDelete( mData );
    mData = new Util::Data( val );

    CPPUNIT_ASSERT_EQUAL( sizeof( uint32 ), mData->size() );
    CPPUNIT_ASSERT_EQUAL( val, *mData->begin< uint32 >() );

    // Revert changes
    tearDown();
    setUp();
}

void Util::Test< Util::Data >::testConstructorRange()
{
    uint32* begin = reinterpret_cast< uint32* >( &PLAIN_DATA[ 0 ] );
    uint32* end = reinterpret_cast< uint32* >( &PLAIN_DATA[ 7 ] );

    SafeDelete( mData );
    mData = new Util::Data( begin, end );

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

void Util::Test< Util::Data >::testConstructorCopy()
{
    const Util::Data data( &PLAIN_DATA[ 0 ], &PLAIN_DATA[ sizeof( PLAIN_DATA ) ] );

    SafeDelete( mData );
    mData = new Util::Data( data );

    CPPUNIT_ASSERT_EQUAL( data.size(), mData->size() );
    CPPUNIT_ASSERT( std::equal( data.begin< uint8 >(), data.end< uint8 >(),
                                mData->begin< uint8 >() ) );

    // Revert changes
    tearDown();
    setUp();
}

void Util::Test< Util::Data >::testGet()
{
    uint32& ref = mData->Get< uint32 >( 0 );
    uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 0 ] );

    CPPUNIT_ASSERT_EQUAL( val, ref );

    ref = ++val;

    CPPUNIT_ASSERT_EQUAL( val, ref );
}

void Util::Test< Util::Data >::testOperatorSubscript()
{
    const size_t index = 3;
    uint8 val = PLAIN_DATA[ index ];

    CPPUNIT_ASSERT_EQUAL( val, ( *mData )[ index ] );

    ( *mData )[ index ] = --val;

    CPPUNIT_ASSERT_EQUAL( val, ( *mData )[ index ] );
}

void Util::Test< Util::Data >::testIteratorConstructorCopy()
{
    const Util::Data::ConstIterator< uint32 > itr( mData->begin< uint32 >() );

    CPPUNIT_ASSERT( mData->begin< uint32 >() == itr );
}

void Util::Test< Util::Data >::testIteratorOperatorCopy()
{
    Util::Data::ConstIterator< uint32 > itr;
    itr = mData->begin< uint32 >();

    CPPUNIT_ASSERT( mData->begin< uint32 >() == itr );
}

void Util::Test< Util::Data >::testIteratorOperatorDereference()
{
    const Util::Data::Iterator< uint16 > itr = mData->begin< uint16 >();
    uint16 val = reinterpret_cast< const uint16& >( PLAIN_DATA[ 0 ] );

    CPPUNIT_ASSERT_EQUAL( val, *itr );

    *itr = ++val;

    CPPUNIT_ASSERT_EQUAL( val, *itr );
}

void Util::Test< Util::Data >::testIteratorOperatorSubscript()
{
    const Util::Data::Iterator< uint16 > itr = mData->begin< uint16 >();
    uint16 val = reinterpret_cast< const uint16& >( PLAIN_DATA[ 4 ] );

    CPPUNIT_ASSERT_EQUAL( val, itr[ 2 ] );

    itr[ 2 ] = --val;

    CPPUNIT_ASSERT_EQUAL( val, itr[ 2 ] );
}

void Util::Test< Util::Data >::testIteratorOperatorComparison()
{
    const Util::Data::ConstIterator< uint8 > begin = mData->begin< uint8 >();
    const Util::Data::ConstIterator< uint8 > end = mData->end< uint8 >();

    CPPUNIT_ASSERT( !( begin == end ) );
    CPPUNIT_ASSERT( begin != end );

    CPPUNIT_ASSERT( begin < end );
    CPPUNIT_ASSERT( end > begin );

    CPPUNIT_ASSERT( begin <= end );
    CPPUNIT_ASSERT( end >= begin );

    const Util::Data::ConstIterator< uint32 > itr1 = ++( begin.as< uint32 >() );
    const Util::Data::ConstIterator< uint32 > itr2 = --( end.as< uint32 >() );

    CPPUNIT_ASSERT( itr1 == itr2 );
    CPPUNIT_ASSERT( !( itr1 != itr2 ) );

    CPPUNIT_ASSERT( !( itr1 < itr2 ) );
    CPPUNIT_ASSERT( !( itr2 > itr1 ) );

    CPPUNIT_ASSERT( itr1 <= itr2 );
    CPPUNIT_ASSERT( itr2 >= itr1 );
}

void Util::Test< Util::Data >::testIteratorOperatorArithmetic()
{
    Util::Data::ConstIterator< uint8 > itr = mData->begin< uint8 >();

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

void Util::Test< Util::Data >::testIteratorAs()
{
    const Util::Data::ConstIterator< uint8 > itr = mData->begin< uint8 >() + 3;
    const uint32 val = reinterpret_cast< const uint32& >( PLAIN_DATA[ 3 ] );

    CPPUNIT_ASSERT_EQUAL( val, *itr.as< uint32 >() );
}
