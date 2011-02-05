#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common-os" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/common/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/common/${TARGET_NAME}" )

FIND_PACKAGE( "Threads" REQUIRED )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonOs.h" )
SET( SOURCE
     "" )

SET( fs_INCLUDE
     "${TARGET_INCLUDE_DIR}/fs/Directory.h"
     "${TARGET_INCLUDE_DIR}/fs/File.h" )
SET( fs_SOURCE
     "${TARGET_SOURCE_DIR}/fs/Directory.cpp"
     "${TARGET_SOURCE_DIR}/fs/File.cpp" )

SET( log_INCLUDE
     "${TARGET_INCLUDE_DIR}/log/Console.h"
     "${TARGET_INCLUDE_DIR}/log/File.h"
     "${TARGET_INCLUDE_DIR}/log/LogMgr.h"
     "${TARGET_INCLUDE_DIR}/log/Message.h" )
SET( log_SOURCE
     "${TARGET_SOURCE_DIR}/log/Console.cpp"
     "${TARGET_SOURCE_DIR}/log/File.cpp"
     "${TARGET_SOURCE_DIR}/log/LogMgr.cpp"
     "${TARGET_SOURCE_DIR}/log/Message.cpp" )


SET( mt_INCLUDE
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
     "${TARGET_INCLUDE_DIR}/net/Ip4.h"
     "${TARGET_INCLUDE_DIR}/net/Ip6.h"
     "${TARGET_INCLUDE_DIR}/net/Socket.h"
     "${TARGET_INCLUDE_DIR}/net/StreamPacketizer.h"
     "${TARGET_INCLUDE_DIR}/net/StreamSocket.h"
#     "${TARGET_INCLUDE_DIR}/net/TcpConnection.h"
#     "${TARGET_INCLUDE_DIR}/net/TcpServer.h"
     "${TARGET_INCLUDE_DIR}/net/Utils.h" )
SET( net_SOURCE
     "${TARGET_SOURCE_DIR}/net/Ip4.cpp"
     "${TARGET_SOURCE_DIR}/net/Ip6.cpp"
     "${TARGET_SOURCE_DIR}/net/StreamPacketizer.cpp" )
#     "${TARGET_SOURCE_DIR}/net/TcpConnection.cpp"
#     "${TARGET_SOURCE_DIR}/net/TcpServer.cpp"

IF( NOT WIN32 )
    SET( posix_INCLUDE
         "${TARGET_INCLUDE_DIR}/posix/Condition.h"
         "${TARGET_INCLUDE_DIR}/posix/Directory.h"
         "${TARGET_INCLUDE_DIR}/posix/Fd.h"
         "${TARGET_INCLUDE_DIR}/posix/File.h"
         "${TARGET_INCLUDE_DIR}/posix/Mutex.h"
         "${TARGET_INCLUDE_DIR}/posix/Thread.h" )
    SET( posix_SOURCE
         "${TARGET_SOURCE_DIR}/posix/Condition.cpp"
         "${TARGET_SOURCE_DIR}/posix/Directory.cpp"
         "${TARGET_SOURCE_DIR}/posix/Fd.cpp"
         "${TARGET_SOURCE_DIR}/posix/File.cpp"
         "${TARGET_SOURCE_DIR}/posix/Mutex.cpp"
         "${TARGET_SOURCE_DIR}/posix/Thread.cpp" )
ENDIF( NOT WIN32 )

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/Const.h"
     "${TARGET_INCLUDE_DIR}/time/Msec.h"
     "${TARGET_INCLUDE_DIR}/time/TimeMgr.h"
     "${TARGET_INCLUDE_DIR}/time/Timer.h"
     "${TARGET_INCLUDE_DIR}/time/Timespec.h"
     "${TARGET_INCLUDE_DIR}/time/Timeval.h"
     "${TARGET_INCLUDE_DIR}/time/Tm.h"
     "${TARGET_INCLUDE_DIR}/time/Utils.h"
     "${TARGET_INCLUDE_DIR}/time/WinTime.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/Const.cpp"
     "${TARGET_SOURCE_DIR}/time/Msec.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeMgr.cpp"
     "${TARGET_SOURCE_DIR}/time/Timer.cpp"
     "${TARGET_SOURCE_DIR}/time/Timespec.cpp"
     "${TARGET_SOURCE_DIR}/time/Timeval.cpp"
     "${TARGET_SOURCE_DIR}/time/Tm.cpp"
     "${TARGET_SOURCE_DIR}/time/WinTime.cpp" )

IF( WIN32 )
    SET( win_INCLUDE
         "${TARGET_INCLUDE_DIR}/win/Condition.h"
         "${TARGET_INCLUDE_DIR}/win/ConsoleScreenBuffer.h"
         "${TARGET_INCLUDE_DIR}/win/CriticalSection.h"
         "${TARGET_INCLUDE_DIR}/win/Directory.h"
         "${TARGET_INCLUDE_DIR}/win/Event.h"
         "${TARGET_INCLUDE_DIR}/win/File.h"
         "${TARGET_INCLUDE_DIR}/win/FileSearch.h"
         "${TARGET_INCLUDE_DIR}/win/Handle.h"
         "${TARGET_INCLUDE_DIR}/win/InitWinsock.h"
         "${TARGET_INCLUDE_DIR}/win/Thread.h" )
    SET( win_SOURCE
         "${TARGET_SOURCE_DIR}/win/Condition.cpp"
         "${TARGET_SOURCE_DIR}/win/ConsoleScreenBuffer.cpp"
         "${TARGET_SOURCE_DIR}/win/CriticalSection.cpp"
         "${TARGET_SOURCE_DIR}/win/Directory.cpp"
         "${TARGET_SOURCE_DIR}/win/Event.cpp"
         "${TARGET_SOURCE_DIR}/win/File.cpp"
         "${TARGET_SOURCE_DIR}/win/FileSearch.cpp"
         "${TARGET_SOURCE_DIR}/win/Handle.cpp"
         "${TARGET_SOURCE_DIR}/win/InitWinsock.cpp"
         "${TARGET_SOURCE_DIR}/win/Thread.cpp" )
ENDIF( WIN32 )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${common-std_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"        FILES ${INCLUDE} )
SOURCE_GROUP( "include\\fs"    FILES ${fs_INCLUDE} )
SOURCE_GROUP( "include\\log"   FILES ${log_INCLUDE} )
SOURCE_GROUP( "include\\mt"    FILES ${mt_INCLUDE} )
SOURCE_GROUP( "include\\net"   FILES ${net_INCLUDE} )
SOURCE_GROUP( "include\\posix" FILES ${posix_INCLUDE} )
SOURCE_GROUP( "include\\time"  FILES ${time_INCLUDE} )
SOURCE_GROUP( "include\\win"   FILES ${win_INCLUDE} )

SOURCE_GROUP( "src"        FILES ${SOURCE} )
SOURCE_GROUP( "src\\fs"    FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\log"   FILES ${log_SOURCE} )
SOURCE_GROUP( "src\\mt"    FILES ${mt_SOURCE} )
SOURCE_GROUP( "src\\net"   FILES ${net_SOURCE} )
SOURCE_GROUP( "src\\posix" FILES ${posix_SOURCE} )
SOURCE_GROUP( "src\\time"  FILES ${time_SOURCE} )
SOURCE_GROUP( "src\\win"   FILES ${win_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}       ${SOURCE}
             ${fs_INCLUDE}    ${fs_SOURCE}
             ${log_INCLUDE}   ${log_SOURCE}
             ${mt_INCLUDE}    ${mt_SOURCE}
             ${net_INCLUDE}   ${net_SOURCE}
             ${posix_INCLUDE} ${posix_SOURCE}
             ${time_INCLUDE}  ${time_SOURCE}
             ${win_INCLUDE}   ${win_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonOs.h"
                  "net/Ip4.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-std"
                       ${CMAKE_THREAD_LIBS_INIT} )

IF( WIN32 )
    TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                           "wsock32" )
ENDIF( WIN32 )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
