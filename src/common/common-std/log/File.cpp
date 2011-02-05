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

#include "CommonStd.h"

#include "log/File.h"

using namespace common;
using namespace common::log;

/*************************************************************************/
/* common::log::File                                                     */
/*************************************************************************/
File::File( const char* name )
: mFile( name, "a" )
{
}

stream::Error File::Write( const Message& m )
{
    const stdx::Tm& tm = m.time();

    int code = mFile.Printf( "%04d-%02d-%02d %02d:%02d:%02d %c %s: %s\n",
                             1900 + tm.year(), 1 + tm.mon(), tm.mday(),
                             tm.hour(), tm.min(), tm.sec(),
                             m.prefix(), m.source().c_str(),
                             m.message().c_str() );

#ifndef NDEBUG
    /* Flush immediately to keep the log file
       accurate even in case of a crash. */
    mFile.Flush();
#endif /* !NDEBUG */

    return 0 <= code ? stream::ERROR_OK : stream::ERROR_WRITE;
}

stream::Error File::Write( const Message* mp, size_t count, size_t* countWritten )
{
    for( size_t i = 0; i < count; ++i )
    {
        const stream::Error err = Write( mp[ i ] );
        if( stream::ERROR_OK != err )
        {
            if( NULL != countWritten )
                *countWritten = i;

            return err;
        }
    }

    if( NULL != countWritten )
        *countWritten = count;

    return stream::ERROR_OK;
}
