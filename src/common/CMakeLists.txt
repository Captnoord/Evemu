#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/${TARGET_NAME}" )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/Common.h"
     "${TARGET_INCLUDE_DIR}/CommonPCH.h" )
SET( SOURCE
     "" )

SET( db_INCLUDE
     "${TARGET_INCLUDE_DIR}/db/Common.h"
     "${TARGET_INCLUDE_DIR}/db/Core.h"
     "${TARGET_INCLUDE_DIR}/db/Type.h" )
SET( db_SOURCE
     "${TARGET_SOURCE_DIR}/db/Core.cpp"
     "${TARGET_SOURCE_DIR}/db/Type.cpp" )

SET( fs_INCLUDE
     "${TARGET_INCLUDE_DIR}/fs/Common.h"
     "${TARGET_INCLUDE_DIR}/fs/DirWalker.h"
     "${TARGET_INCLUDE_DIR}/fs/Utils.h" )
SET( fs_SOURCE
     "${TARGET_SOURCE_DIR}/fs/DirWalker.cpp"
     "${TARGET_SOURCE_DIR}/fs/Utils.cpp" )

SET( ga_INCLUDE
     "${TARGET_INCLUDE_DIR}/ga/Common.h"
     "${TARGET_INCLUDE_DIR}/ga/Utils.h" )
SET( ga_SOURCE
     "" )

SET( mt_INCLUDE
     "${TARGET_INCLUDE_DIR}/mt/Common.h"
     "${TARGET_INCLUDE_DIR}/mt/Condition.h"
     "${TARGET_INCLUDE_DIR}/mt/Mutex.h"
     "${TARGET_INCLUDE_DIR}/mt/Target.h"
     "${TARGET_INCLUDE_DIR}/mt/Thread.h"
     "${TARGET_INCLUDE_DIR}/mt/ThreadMgr.h" )
SET( mt_SOURCE
     "${TARGET_SOURCE_DIR}/mt/Condition.cpp"
     "${TARGET_SOURCE_DIR}/mt/Mutex.cpp"
     "${TARGET_SOURCE_DIR}/mt/Target.cpp"
     "${TARGET_SOURCE_DIR}/mt/Thread.cpp"
     "${TARGET_SOURCE_DIR}/mt/ThreadMgr.cpp" )

SET( net_INCLUDE
     "${TARGET_INCLUDE_DIR}/net/Common.h"
     "${TARGET_INCLUDE_DIR}/net/Ip4.h"
     "${TARGET_INCLUDE_DIR}/net/Ip6.h"
     "${TARGET_INCLUDE_DIR}/net/Socket.h"
     "${TARGET_INCLUDE_DIR}/net/StreamPacketizer.h"
     "${TARGET_INCLUDE_DIR}/net/StreamSocket.h"
     "${TARGET_INCLUDE_DIR}/net/TcpConnection.h"
     "${TARGET_INCLUDE_DIR}/net/TcpServer.h"
     "${TARGET_INCLUDE_DIR}/net/Utils.h" )
SET( net_SOURCE
     "${TARGET_SOURCE_DIR}/net/Ip4.cpp"
     "${TARGET_SOURCE_DIR}/net/Ip6.cpp"
     "${TARGET_SOURCE_DIR}/net/StreamPacketizer.cpp"
     "${TARGET_SOURCE_DIR}/net/TcpConnection.cpp"
     "${TARGET_SOURCE_DIR}/net/TcpServer.cpp"
     "${TARGET_SOURCE_DIR}/net/Utils.cpp" )

IF( NOT WIN32 )
    SET( posix_INCLUDE
         "${TARGET_INCLUDE_DIR}/posix/Common.h"
         "${TARGET_INCLUDE_DIR}/posix/Condition.h"
         "${TARGET_INCLUDE_DIR}/posix/ConditionAttribute.h"
         "${TARGET_INCLUDE_DIR}/posix/Mutex.h"
         "${TARGET_INCLUDE_DIR}/posix/MutexAttribute.h"
         "${TARGET_INCLUDE_DIR}/posix/Thread.h"
         "${TARGET_INCLUDE_DIR}/posix/ThreadAttribute.h" )
    SET( posix_SOURCE
         "${TARGET_SOURCE_DIR}/posix/Condition.cpp"
         "${TARGET_SOURCE_DIR}/posix/ConditionAttribute.cpp"
         "${TARGET_SOURCE_DIR}/posix/Mutex.cpp"
         "${TARGET_SOURCE_DIR}/posix/MutexAttribute.cpp"
         "${TARGET_SOURCE_DIR}/posix/Thread.cpp"
         "${TARGET_SOURCE_DIR}/posix/ThreadAttribute.cpp" )
ENDIF( NOT WIN32 )

SET( std_INCLUDE
     "${TARGET_INCLUDE_DIR}/std/Common.h"
     "${TARGET_INCLUDE_DIR}/std/File.h"
     "${TARGET_INCLUDE_DIR}/std/Tm.h"
     "${TARGET_INCLUDE_DIR}/std/Types.h"
     "${TARGET_INCLUDE_DIR}/std/Utils.h" )
SET( std_SOURCE
     "${TARGET_SOURCE_DIR}/std/File.cpp"
     "${TARGET_SOURCE_DIR}/std/Tm.cpp"
     "${TARGET_SOURCE_DIR}/std/Utils.cpp" )

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/Common.h"
     "${TARGET_INCLUDE_DIR}/time/Const.h"
     "${TARGET_INCLUDE_DIR}/time/Msec.h"
     "${TARGET_INCLUDE_DIR}/time/TimeMgr.h"
     "${TARGET_INCLUDE_DIR}/time/Timer.h"
     "${TARGET_INCLUDE_DIR}/time/Timespec.h"
     "${TARGET_INCLUDE_DIR}/time/Timeval.h"
     "${TARGET_INCLUDE_DIR}/time/WinTime.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/Const.cpp"
     "${TARGET_SOURCE_DIR}/time/Msec.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeMgr.cpp"
     "${TARGET_SOURCE_DIR}/time/Timer.cpp"
     "${TARGET_SOURCE_DIR}/time/Timespec.cpp"
     "${TARGET_SOURCE_DIR}/time/Timeval.cpp"
     "${TARGET_SOURCE_DIR}/time/WinTime.cpp" )

SET( util_INCLUDE
     "${TARGET_INCLUDE_DIR}/util/Buffer.h"
     "${TARGET_INCLUDE_DIR}/util/Crc32.h"
     "${TARGET_INCLUDE_DIR}/util/Lock.h"
     "${TARGET_INCLUDE_DIR}/util/Log.h"
     "${TARGET_INCLUDE_DIR}/util/Misc.h"
     "${TARGET_INCLUDE_DIR}/util/RefPtr.h"
     "${TARGET_INCLUDE_DIR}/util/SafeDelete.h"
     "${TARGET_INCLUDE_DIR}/util/Singleton.h"
     "${TARGET_INCLUDE_DIR}/util/StrConv.h"
     "${TARGET_INCLUDE_DIR}/util/StrUtils.h" )
SET( util_SOURCE
     "${TARGET_SOURCE_DIR}/util/Crc32.cpp"
     "${TARGET_SOURCE_DIR}/util/Log.cpp"
     "${TARGET_SOURCE_DIR}/util/Misc.cpp"
     "${TARGET_SOURCE_DIR}/util/StrConv.cpp"
     "${TARGET_SOURCE_DIR}/util/StrUtils.cpp" )

IF( WIN32 )
    SET( win_INCLUDE
         "${TARGET_INCLUDE_DIR}/win/Common.h"
         "${TARGET_INCLUDE_DIR}/win/Condition.h"
         "${TARGET_INCLUDE_DIR}/win/ConsoleScreenBuffer.h"
         "${TARGET_INCLUDE_DIR}/win/CriticalSection.h"
         "${TARGET_INCLUDE_DIR}/win/Event.h"
         "${TARGET_INCLUDE_DIR}/win/Handle.h"
         "${TARGET_INCLUDE_DIR}/win/Thread.h" )
    SET( win_SOURCE
         "${TARGET_SOURCE_DIR}/win/Condition.cpp"
         "${TARGET_SOURCE_DIR}/win/ConsoleScreenBuffer.cpp"
         "${TARGET_SOURCE_DIR}/win/CriticalSection.cpp"
         "${TARGET_SOURCE_DIR}/win/Event.cpp"
         "${TARGET_SOURCE_DIR}/win/Handle.cpp"
         "${TARGET_SOURCE_DIR}/win/Thread.cpp" )
ENDIF( WIN32 )

SET( xml_INCLUDE
     "${TARGET_INCLUDE_DIR}/xml/Common.h"
     "${TARGET_INCLUDE_DIR}/xml/Parser.h"
     "${TARGET_INCLUDE_DIR}/xml/ParserEx.h" )
SET( xml_SOURCE
     "${TARGET_SOURCE_DIR}/xml/Parser.cpp" )

SET( zlib_INCLUDE
     "${TARGET_INCLUDE_DIR}/zlib/Common.h"
     "${TARGET_INCLUDE_DIR}/zlib/Utils.h" )
SET( zlib_SOURCE
     "${TARGET_SOURCE_DIR}/zlib/Utils.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${GANGSTA_INCLUDE_DIR}" )
INCLUDE_DIRECTORIES( "${MYSQL_INCLUDE_DIR}" )
INCLUDE_DIRECTORIES( "${TINYXML_INCLUDE_DIR}" )
INCLUDE_DIRECTORIES( "${ZLIB_INCLUDE_DIR}" )

INCLUDE_DIRECTORIES( "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"        FILES ${INCLUDE} )
SOURCE_GROUP( "include\\db"    FILES ${db_INCLUDE} )
SOURCE_GROUP( "include\\fs"    FILES ${fs_INCLUDE} )
SOURCE_GROUP( "include\\ga"    FILES ${ga_INCLUDE} )
SOURCE_GROUP( "include\\mt"    FILES ${mt_INCLUDE} )
SOURCE_GROUP( "include\\net"   FILES ${net_INCLUDE} )
SOURCE_GROUP( "include\\posix" FILES ${posix_INCLUDE} )
SOURCE_GROUP( "include\\std"   FILES ${std_INCLUDE} )
SOURCE_GROUP( "include\\time"  FILES ${time_INCLUDE} )
SOURCE_GROUP( "include\\util"  FILES ${util_INCLUDE} )
SOURCE_GROUP( "include\\win"   FILES ${win_INCLUDE} )
SOURCE_GROUP( "include\\xml"   FILES ${xml_INCLUDE} )
SOURCE_GROUP( "include\\zlib"  FILES ${zlib_INCLUDE} )

SOURCE_GROUP( "src"        FILES ${SOURCE} )
SOURCE_GROUP( "src\\db"    FILES ${db_SOURCE} )
SOURCE_GROUP( "src\\fs"    FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\ga"    FILES ${ga_SOURCE} )
SOURCE_GROUP( "src\\mt"    FILES ${mt_SOURCE} )
SOURCE_GROUP( "src\\net"   FILES ${net_SOURCE} )
SOURCE_GROUP( "src\\posix" FILES ${posix_SOURCE} )
SOURCE_GROUP( "src\\std"   FILES ${std_SOURCE} )
SOURCE_GROUP( "src\\time"  FILES ${time_SOURCE} )
SOURCE_GROUP( "src\\util"  FILES ${util_SOURCE} )
SOURCE_GROUP( "src\\win"   FILES ${win_SOURCE} )
SOURCE_GROUP( "src\\xml"   FILES ${xml_SOURCE} )
SOURCE_GROUP( "src\\zlib"  FILES ${zlib_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}       ${SOURCE}
             ${db_INCLUDE}    ${db_SOURCE}
             ${fs_INCLUDE}    ${fs_SOURCE}
             ${ga_INCLUDE}    ${ga_SOURCE}
             ${mt_INCLUDE}    ${mt_SOURCE}
             ${net_INCLUDE}   ${net_SOURCE}
             ${posix_INCLUDE} ${posix_SOURCE}
             ${std_INCLUDE}   ${std_SOURCE}
             ${time_INCLUDE}  ${time_SOURCE}
             ${util_INCLUDE}  ${util_SOURCE}
             ${win_INCLUDE}   ${win_SOURCE}
             ${xml_INCLUDE}   ${xml_SOURCE}
             ${zlib_INCLUDE}  ${zlib_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonPCH.h"
                  "Common.cpp" )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
