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
    Author:     Zhur
*/

#ifndef __FS__DIR_WALKER_H__INCL__
#define __FS__DIR_WALKER_H__INCL__

namespace Fs
{
    /**
     * @brief Simple class for directory listing.
     *
     * @note Capt: Even that I don't always like boost,
     *       it has a very useful class called FileSystem
     *       which does this stuff very crossplatform.
     *
     * @author Zhur, Bloody.Rabbit
     */
    class DirWalker
    {
    public:
        /// A primary constructor.
        DirWalker();
        /// A destructor.
        ~DirWalker();

        /// @return Current file name.
        const char* currentFileName();

        /**
         * @brief Opens directory for listing.
         *
         * @param[in] dir    Path to directory; must NOT end with a backslash.
         * @param[in] suffix Only files with this suffix are matched.
         *
         * @retval true  Directory opened successfully.
         * @retval false Failed to open directory.
         */
        bool OpenDir( const char* dir, const char* suffix = "" );
        /// Closes opened directory.
        void CloseDir();

        /**
         * @brief Iterates over to next file in directory.
         *
         * This function must be called after OpenDir()
         * before calling currentFileName().
         *
         * @retval true  Iteration successfull.
         * @retval false Iteration failed; most likely there are no more files.
         */
        bool NextFile();

    protected:
#   ifdef WIN32
        /// A search handle.
        HANDLE mFind;
        /// A struct which holds data about the current file.
        WIN32_FIND_DATA mFindData;
        /// True if mFind is valid.
        bool mValid;
        /// True if this is the first file in the directory.
        bool mFirst;
#   else
        /// A handle to the directory.
        DIR* mDir;
        /// A current directory entry.
        struct dirent* mFile;
        /// A match suffix.
        std::string mSuffix;
#   endif
    };
}

#endif /* !__FS__DIR_WALKER_H__INCL__ */
