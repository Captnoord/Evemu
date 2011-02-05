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

#ifndef __COMMON_WIN_H__INCL__
#define __COMMON_WIN_H__INCL__

/*************************************************************************/
/* common-std                                                            */
/*************************************************************************/
namespace common
{
    namespace win
    {
        class File;
        class FileSearch;

        class Condition;
        class CriticalSection;
        class Thread;
    }
}

#define COMMON_FS_DIRECTORY_TYPE common::win::FileSearch
#define COMMON_FS_FILE_TYPE      common::win::File

#define COMMON_MT_CONDITION_TYPE common::win::Condition
#define COMMON_MT_MUTEX_TYPE     common::win::CriticalSection
#define COMMON_MT_THREAD_TYPE    common::win::Thread

#include "CommonStd.h"

#include "fs/Directory.h"
#include "fs/File.h"
#include "log/Console.h"
#include "mt/Condition.h"
#include "mt/Mutex.h"
#include "mt/Thread.h"
#include "stream/Input.h"
#include "stream/Output.h"
#include "time/Msec.h"

/*************************************************************************/
/* common-os                                                             */
/*************************************************************************/
#include "CommonOs.h"

#endif /* !__COMMON_WIN_H__INCL__ */
