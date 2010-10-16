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

#ifndef __DATA_TEST_H__INCL__
#define __DATA_TEST_H__INCL__

/**
 * @brief Tests Data and its iterators.
 *
 * @author Bloody.Rabbit
 */
class DataTest
: public CppUnit::TestFixture
{
public:
    /// Some plain data to be used during testing.
    static uint8 PLAIN_DATA[];

    /**
     * @brief Builds a suite of tests.
     *
     * @return The suite.
     */
    static CppUnit::TestSuite* suite();

    /**
     * @brief Sets up a testing environment.
     */
    void setUp();
    /**
     * @brief Tears down the testing environment.
     */
    void tearDown();

    /**
     * @brief Tests default constructor.
     */
    void testConstructorDefault();
    /**
     * @brief Tests value constructor.
     */
    void testConstructorValue();
    /**
     * @brief Tests range constructor.
     */
    void testConstructorRange();
    /**
     * @brief Tests copy constructor.
     */
    void testConstructorCopy();

    /**
     * @brief Tests Get< T >( index ).
     */
    void testGet();

    /**
     * @brief Tests operator subscript.
     */
    void testOperatorSubscript();

    /**
     * @brief Tests iterator copy constructor.
     */
    void testIteratorConstructorCopy();

    /**
     * @brief Tests iterator copy operator.
     */
    void testIteratorOperatorCopy();
    /**
     * @brief Tests iterator dereference operator.
     */
    void testIteratorOperatorDereference();
    /**
     * @brief Tests iterator subscript operator.
     */
    void testIteratorOperatorSubscript();
    /**
     * @brief Tests iterator comparison operators.
     */
    void testIteratorOperatorComparison();
    /**
     * @brief Tests iterator arithmetic.
     */
    void testIteratorOperatorArithmetic();

    /**
     * @brief Tests iterator as< T >().
     */
    void testIteratorAs();

protected:
    /// A Data object being tested.
    Util::Data* mData;
};

#endif /* !__DATA_TEST_H__INCL__ */
