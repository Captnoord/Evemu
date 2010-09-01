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

SET( gangsta_INCLUDE
     "${TARGET_INCLUDE_DIR}/gangsta/GaCommon.h"
     "${TARGET_INCLUDE_DIR}/gangsta/GaUtils.h" )
SET( gangsta_SOURCE
     "" )

SET( mt_INCLUDE
     "${TARGET_INCLUDE_DIR}/mt/Common.h"
     "${TARGET_INCLUDE_DIR}/mt/Condition.h"
     "${TARGET_INCLUDE_DIR}/mt/Mutex.h"
     "${TARGET_INCLUDE_DIR}/mt/Target.h"
     "${TARGET_INCLUDE_DIR}/mt/Thread.h"
     "${TARGET_INCLUDE_DIR}/mt/ThreadMgr.h"
     "${TARGET_INCLUDE_DIR}/mt/Worker.h" )
SET( mt_SOURCE
     "${TARGET_SOURCE_DIR}/mt/Condition.cpp"
     "${TARGET_SOURCE_DIR}/mt/Mutex.cpp"
     "${TARGET_SOURCE_DIR}/mt/Target.cpp"
     "${TARGET_SOURCE_DIR}/mt/Thread.cpp"
     "${TARGET_SOURCE_DIR}/mt/ThreadMgr.cpp"
     "${TARGET_SOURCE_DIR}/mt/Worker.cpp" )

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
         "${TARGET_INCLUDE_DIR}/posix/PosixCommon.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixCondition.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixConditionAttribute.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixMutex.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixMutexAttribute.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixThread.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixThreadAttribute.h" )
    SET( posix_SOURCE
         "${TARGET_SOURCE_DIR}/posix/PosixCondition.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixConditionAttribute.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixMutex.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixMutexAttribute.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixThread.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixThreadAttribute.cpp" )
ENDIF( NOT WIN32 )

SET( std_INCLUDE
     "${TARGET_INCLUDE_DIR}/std/StdCommon.h"
     "${TARGET_INCLUDE_DIR}/std/StdTypes.h"
     "${TARGET_INCLUDE_DIR}/std/StdUtils.h" )
SET( std_SOURCE
     "${TARGET_SOURCE_DIR}/std/StdUtils.cpp" )

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/TimeCommon.h"
     "${TARGET_INCLUDE_DIR}/time/TimeConst.h"
     "${TARGET_INCLUDE_DIR}/time/TimeMsec.h"
     "${TARGET_INCLUDE_DIR}/time/Timer.h"
     "${TARGET_INCLUDE_DIR}/time/TimeTimespec.h"
     "${TARGET_INCLUDE_DIR}/time/TimeTimeval.h"
     "${TARGET_INCLUDE_DIR}/time/TimeTm.h"
     "${TARGET_INCLUDE_DIR}/time/TimeWin.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/TimeConst.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeMsec.cpp"
     "${TARGET_SOURCE_DIR}/time/Timer.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeTimespec.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeTimeval.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeTm.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeWin.cpp" )

SET( utils_INCLUDE
     "${TARGET_INCLUDE_DIR}/utils/Buffer.h"
     "${TARGET_INCLUDE_DIR}/utils/crc32.h"
     "${TARGET_INCLUDE_DIR}/utils/Lock.h"
     "${TARGET_INCLUDE_DIR}/utils/Log.h"
     "${TARGET_INCLUDE_DIR}/utils/misc.h"
     "${TARGET_INCLUDE_DIR}/utils/RefPtr.h"
     "${TARGET_INCLUDE_DIR}/utils/SafeDelete.h"
     "${TARGET_INCLUDE_DIR}/utils/Singleton.h"
     "${TARGET_INCLUDE_DIR}/utils/StrConv.h"
     "${TARGET_INCLUDE_DIR}/utils/StrUtils.h" )
SET( utils_SOURCE
     "${TARGET_SOURCE_DIR}/utils/crc32.cpp"
     "${TARGET_SOURCE_DIR}/utils/Log.cpp"
     "${TARGET_SOURCE_DIR}/utils/misc.cpp"
     "${TARGET_SOURCE_DIR}/utils/StrConv.cpp"
     "${TARGET_SOURCE_DIR}/utils/StrUtils.cpp" )

IF( WIN32 )
    SET( win_INCLUDE
         "${TARGET_INCLUDE_DIR}/win/Common.h"
         "${TARGET_INCLUDE_DIR}/win/Condition.h"
         "${TARGET_INCLUDE_DIR}/win/CriticalSection.h"
         "${TARGET_INCLUDE_DIR}/win/Event.h"
         "${TARGET_INCLUDE_DIR}/win/Handle.h"
         "${TARGET_INCLUDE_DIR}/win/Thread.h" )
    SET( win_SOURCE
         "${TARGET_SOURCE_DIR}/win/Condition.cpp"
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

SOURCE_GROUP( "include"          FILES ${INCLUDE} )
SOURCE_GROUP( "include\\db"      FILES ${db_INCLUDE} )
SOURCE_GROUP( "include\\fs"      FILES ${fs_INCLUDE} )
SOURCE_GROUP( "include\\gangsta" FILES ${gangsta_INCLUDE} )
SOURCE_GROUP( "include\\mt"      FILES ${mt_INCLUDE} )
SOURCE_GROUP( "include\\net"     FILES ${net_INCLUDE} )
SOURCE_GROUP( "include\\posix"   FILES ${posix_INCLUDE} )
SOURCE_GROUP( "include\\std"     FILES ${std_INCLUDE} )
SOURCE_GROUP( "include\\time"    FILES ${time_INCLUDE} )
SOURCE_GROUP( "include\\utils"   FILES ${utils_INCLUDE} )
SOURCE_GROUP( "include\\win"     FILES ${win_INCLUDE} )
SOURCE_GROUP( "include\\xml"     FILES ${xml_INCLUDE} )
SOURCE_GROUP( "include\\zlib"    FILES ${zlib_INCLUDE} )

SOURCE_GROUP( "src"          FILES ${SOURCE} )
SOURCE_GROUP( "src\\db"      FILES ${db_SOURCE} )
SOURCE_GROUP( "src\\fs"      FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\gangsta" FILES ${gangsta_SOURCE} )
SOURCE_GROUP( "src\\mt"      FILES ${mt_SOURCE} )
SOURCE_GROUP( "src\\net"     FILES ${net_SOURCE} )
SOURCE_GROUP( "src\\posix"   FILES ${posix_SOURCE} )
SOURCE_GROUP( "src\\std"     FILES ${std_SOURCE} )
SOURCE_GROUP( "src\\time"    FILES ${time_SOURCE} )
SOURCE_GROUP( "src\\utils"   FILES ${utils_SOURCE} )
SOURCE_GROUP( "src\\win"     FILES ${win_SOURCE} )
SOURCE_GROUP( "src\\xml"     FILES ${xml_SOURCE} )
SOURCE_GROUP( "src\\zlib"    FILES ${zlib_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}         ${SOURCE}
             ${db_INCLUDE}      ${db_SOURCE}
             ${fs_INCLUDE}      ${fs_SOURCE}
             ${gangsta_INCLUDE} ${gangsta_SOURCE}
             ${mt_INCLUDE}      ${mt_SOURCE}
             ${net_INCLUDE}     ${net_SOURCE}
             ${posix_INCLUDE}   ${posix_SOURCE}
             ${std_INCLUDE}     ${std_SOURCE}
             ${time_INCLUDE}    ${time_SOURCE}
             ${utils_INCLUDE}   ${utils_SOURCE}
             ${win_INCLUDE}     ${win_SOURCE}
             ${xml_INCLUDE}     ${xml_SOURCE}
             ${zlib_INCLUDE}    ${zlib_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonPCH.h"
                  "Common.cpp" )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
