/*
    ------------------------------------------------------------------------------------
    LICENSE:
    ------------------------------------------------------------------------------------
    This file is part of EVEmu: EVE Online Server Emulator
    Copyright 2006 - 2009 The EVEmu Team
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
    Author:     Captnoord
*/

#ifndef FILEMODULE_H
#define FILEMODULE_H

#ifndef WIN32
#include <sys/types.h>
#include <dirent.h>
#endif

/**
 * \class FileModule
 *
 * @brief a cross platform file system class.
 *
 * this class acts like a wrapper around platform specific file
 * routines. It makes reading all the files from a directory a
 * bit easier to do. It also makes sure that the code using this
 * class won't result into total unreadable crap.
 *
 * @author Captnoord
 * @date May 2009
 * @note please please please if it happens that you get platform specific
 * code outside this class. Please rewrite this class so all the platform
 * specific code is inside it.
 * @todo prettify this code as it looks a bit chaotic.
 * @todo redesign this baby, i'm not happy with it (capt).
 */
class FileModule
{
public:
    FileModule(const std::string &path) : mHandle(0), mPath(path.c_str())
    {
        memset(&mInfo, 0, sizeof(DirInfo));
    }

#ifdef WIN32
    FileModule(const char *path) : mHandle(0), mPath(path)
    {
        memset(&mInfo, 0, sizeof(DirInfo));
    }
#else
    FileModule(const char *path) : mHandle(NULL), mInfo(NULL), mPath(path)
    {
        mHandle = opendir(path);
        /*if(mHandle == NULL) {
          printf("Failed to open dir %s: %s", mPath, strerror(errno));
        }*/
    }
#endif//WIN32

    ~FileModule()
    {
#ifdef WIN32
        if(mHandle != INVALID_HANDLE_VALUE && mHandle != 0)
            FindClose(mHandle);
#else
        if(mHandle != NULL)
            closedir(mHandle);
#endif
        mHandle = 0;
    }

    /**
     * Get the next file from the directory
     */
    const char *NextFile()
    {
#ifdef WIN32
        // I know that this implementation causes a check every NextFile call but doing it this way makes it
        // more generic. It also means I have to do less tricks.
        if (mHandle == 0)
        {
            mHandle = FindFirstFile(mPath, &mInfo);
        }
        else
        {
            if (!FindNextFile(mHandle, &mInfo))
                return NULL;
        }

        if(mHandle == INVALID_HANDLE_VALUE || mHandle == 0)
            return NULL;

        return((const char *)mInfo.cFileName);
#else
        if (mHandle == NULL)
            return NULL;

        mInfo = readdir(mHandle);

        if (mInfo == NULL)
            return NULL;

        return mInfo->d_name;
#endif//WIN32
    }

#ifdef WIN32
    typedef HANDLE  DirLink;
    typedef WIN32_FIND_DATA DirInfo;
#else
    typedef DIR*    DirLink;
    typedef struct dirent* DirInfo;
#endif//WIN32

protected:
    DirLink mHandle;
    DirInfo mInfo;
    const char *mPath;

};
#endif // FILEMODULE_H
