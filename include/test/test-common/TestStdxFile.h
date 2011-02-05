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

#ifndef __TEST__TEST_STDX_FILE_H__INCL__
#define __TEST__TEST_STDX_FILE_H__INCL__

/**
 * @brief Aggregates all unit test classes.
 */
namespace test
{
    /**
     * @brief Tests stdx::File.
     *
     * @author Bloody.Rabbit
     */
    class TestStdxFile
    : public CppUnit::TestCase
    {
    public:
        /// A name for the test file
        static const char NAME[];
        /// An alternate name for the file.
        static const char NAME_ALT[];
        /// Some plain data to be used during testing.
        static const uint8 PLAIN_DATA[];

        /**
         * @brief A default constructor.
         */
        TestStdxFile();

        /**
         * @brief Runs the test.
         */
        void runTest();

    protected:
        /// The file being tested.
        stdx::File mFile;
    };
}

#endif /* !__TEST__TEST_STDX_FILE_H__INCL__ */
