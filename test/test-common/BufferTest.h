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

#ifndef __BUFFER_TEST_H__INCL__
#define __BUFFER_TEST_H__INCL__

/**
 * @brief Tests Buffer.
 *
 * @author Bloody.Rabbit
 */
class BufferTest
: public CppUnit::TestFixture
{
public:
    /// An array with some data for testing.
    static const uint8 PLAIN_DATA[];

    /**
     * @brief Creates a test suite.
     *
     * @return The test suite.
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
     * @brief Tests the size constructor.
     */
    void testConstructorSize();
    /**
     * @brief Tests the range constructor.
     */
    void testConstructorRange();
    /**
     * @brief Tests the copy constructor.
     */
    void testConstructorCopy();

    /**
     * @brief Tests the append operator.
     */
    void testOperatorAppend();
    /**
     * @brief Tests the assign operator.
     */
    void testOperatorAssign();
    /**
     * @brief Tests the copy operator.
     */
    void testOperatorCopy();

    /**
     * @brief Tests value Append.
     */
    void testAppendValue();
    /**
     * @brief Tests range Append.
     */
    void testAppendRange();

    /**
     * @brief Tests value Assign.
     */
    void testAssignValue();
    /**
     * @brief Tests range Assign.
     */
    void testAssignRange();

    /**
     * @brief Tests Reserve.
     */
    void testReserve();
    /**
     * @brief Tests ReserveAt.
     */
    void testReserveAt();

    /**
     * @brief Tests Resize.
     */
    void testResize();
    /**
     * @brief Tests ResizeAt.
     */
    void testResizeAt();

protected:
    /// The tested buffer.
    Util::Buffer* mBuffer;
};

#endif /* !__BUFFER_TEST_H__INCL__ */
