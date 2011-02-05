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

#ifndef __TEST__TEST_FS_FILE_H__INCL__
#define __TEST__TEST_FS_FILE_H__INCL__

namespace test
{
    /**
     * @brief Tests fs::File.
     *
     * @author Bloody.Rabbit
     */
    class TestFsFile
    : public CppUnit::TestFixture
    {
    public:
        /// A filename used during the test.
        static const char NAME[];
        /// Another filename used during the test.
        static const char NAME_ALT[];
        /// Some data to use during testing.
        static const uint8 PLAIN_DATA[];

        /**
         * @brief Creates a test suite.
         *
         * @return The test suite.
         */
        static CppUnit::TestSuite* suite();

        /**
         * @brief Sets up the testing environment.
         */
        void setUp();
        /**
         * @brief Tears down the testing environment.
         */
        void tearDown();

        /**
         * @brief Tests file rename.
         */
        void testRename();
        /**
         * @brief Tests file removal.
         */
        void testRemove();

        /**
         * @brief Tests read-only open.
         */
        void testOpenRead();
        /**
         * @brief Tests write-only open.
         */
        void testOpenWrite();
        /**
         * @brief Tests read-write open.
         */
        void testOpenReadWrite();

        /**
         * @brief Tests create-new and open-existing modes separately.
         */
        void testOpenCreateSeparate();
        /**
         * @brief Tests create-new and open-existing modes combined.
         */
        void testOpenCreateCombined();
        /**
         * @brief Tests truncation open.
         */
        void testOpenTruncate();

    protected:
        /// The file being tested.
        fs::File* mFile;
    };
}

#endif /* !__TEST__TEST_FS_FILE_H__INCL__ */
