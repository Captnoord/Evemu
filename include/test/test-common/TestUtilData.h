/**
 * @file TestUtilData.h
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

#ifndef __TEST__TEST_UTIL_DATA_H__INCL__
#define __TEST__TEST_UTIL_DATA_H__INCL__

namespace test
{
    /**
     * @brief Tests util::Data and its iterators.
     *
     * @author Bloody.Rabbit
     */
    class TestUtilData
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
        util::Data* mData;
    };
}

#endif /* !__TEST__TEST_UTIL_DATA_H__INCL__ */
