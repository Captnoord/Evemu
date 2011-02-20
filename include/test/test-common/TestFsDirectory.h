/**
 * @file TestFsDirectory.h
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

#ifndef __TEST__TEST_FS_DIRECTORY_H__INCL__
#define __TEST__TEST_FS_DIRECTORY_H__INCL__

namespace test
{
    /**
     * @brief Tests fs::Directory.
     *
     * @author Bloody.Rabbit
     */
    class TestFsDirectory
    : public CppUnit::TestCase
    {
    public:
        /// A directory name used during the test.
        static const char NAME_TOP[];
        /// Another directory name used during the test.
        static const char NAME_BOTTOM[];

        /**
         * @brief A primary constructor.
         */
        TestFsDirectory();

        /**
         * @brief Runs the directory test.
         */
        void runTest();

    protected:
        /// The directory being tested.
        fs::Directory mDirectory;
    };
}

#endif /* !__TEST__TEST_FS_DIRECTORY_H__INCL__ */
