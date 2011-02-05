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

#ifndef __COMMON_POSIX_H__INCL__
#define __COMMON_POSIX_H__INCL__

/*************************************************************************/
/* common-std                                                            */
/*************************************************************************/
namespace common
{
    namespace posix
    {
        class Directory;
        class File;

        class Condition;
        class Mutex;
        class Thread;
    }
}

#define COMMON_FS_DIRECTORY_TYPE common::posix::Directory
#define COMMON_FS_FILE_TYPE      common::posix::File

#define COMMON_MT_CONDITION_TYPE common::posix::Condition
#define COMMON_MT_MUTEX_TYPE     common::posix::Mutex
#define COMMON_MT_THREAD_TYPE    common::posix::Thread

#include "CommonStd.h"

#include "fs/Directory.h"
#include "fs/File.h"
#include "log/Console.h"
#include "mt/Condition.h"
#include "mt/Mutex.h"
#include "mt/Thread.h"
#include "stream/Input.h"
#include "stream/Output.h"
#include "time/Const.h"

/*************************************************************************/
/* common-os                                                             */
/*************************************************************************/
#include "CommonOs.h"

#include "time/Timeval.h"
#include "time/TimeMgr.h"

#endif /* !__COMMON_POSIX_H__INCL__ */
