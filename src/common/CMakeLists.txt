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
     "${TARGET_INCLUDE_DIR}/db/DbCommon.h"
     "${TARGET_INCLUDE_DIR}/db/DbCore.h"
     "${TARGET_INCLUDE_DIR}/db/DbType.h" )
SET( db_SOURCE
     "${TARGET_SOURCE_DIR}/db/DbCore.cpp"
     "${TARGET_SOURCE_DIR}/db/DbType.cpp" )

SET( fs_INCLUDE
     "${TARGET_INCLUDE_DIR}/fs/DirWalker.h"
     "${TARGET_INCLUDE_DIR}/fs/FsCommon.h"
     "${TARGET_INCLUDE_DIR}/fs/FsUtils.h" )
SET( fs_SOURCE
     "${TARGET_SOURCE_DIR}/fs/DirWalker.cpp"
     "${TARGET_SOURCE_DIR}/fs/FsUtils.cpp" )

SET( gangsta_INCLUDE
     "${TARGET_INCLUDE_DIR}/gangsta/GaCommon.h"
     "${TARGET_INCLUDE_DIR}/gangsta/GaUtils.h" )
SET( gangsta_SOURCE
     "" )

SET( log_INCLUDE
     "${TARGET_INCLUDE_DIR}/log/HexUtils.h"
     "${TARGET_INCLUDE_DIR}/log/LogNew.h"
     "${TARGET_INCLUDE_DIR}/log/logsys.h"
     "${TARGET_INCLUDE_DIR}/log/logtypes.h" )
SET( log_SOURCE
     "${TARGET_SOURCE_DIR}/log/HexUtils.cpp"
     "${TARGET_SOURCE_DIR}/log/LogNew.cpp"
     "${TARGET_SOURCE_DIR}/log/logsys.cpp" )

SET( net_INCLUDE
     "${TARGET_INCLUDE_DIR}/net/NetCommon.h"
     "${TARGET_INCLUDE_DIR}/net/NetUtils.h"
     "${TARGET_INCLUDE_DIR}/net/Socket.h"
     "${TARGET_INCLUDE_DIR}/net/StreamPacketizer.h"
     "${TARGET_INCLUDE_DIR}/net/TCPConnection.h"
     "${TARGET_INCLUDE_DIR}/net/TCPServer.h" )
SET( net_SOURCE
     "${TARGET_SOURCE_DIR}/net/NetUtils.cpp"
     "${TARGET_SOURCE_DIR}/net/Socket.cpp"
     "${TARGET_SOURCE_DIR}/net/StreamPacketizer.cpp"
     "${TARGET_SOURCE_DIR}/net/TCPConnection.cpp"
     "${TARGET_SOURCE_DIR}/net/TCPServer.cpp" )

IF( NOT WIN32 )
    SET( posix_INCLUDE
         "${TARGET_INCLUDE_DIR}/posix/PosixCommon.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixCondition.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixConditionAttribute.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixMutex.h"
         "${TARGET_INCLUDE_DIR}/posix/PosixMutexAttribute.h" )
    SET( posix_SOURCE
         "${TARGET_SOURCE_DIR}/posix/PosixCondition.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixConditionAttribute.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixMutex.cpp"
         "${TARGET_SOURCE_DIR}/posix/PosixMutexAttribute.cpp" )
ENDIF( NOT WIN32 )

SET( std_INCLUDE
     "${TARGET_INCLUDE_DIR}/std/StdCommon.h"
     "${TARGET_INCLUDE_DIR}/std/StdTypes.h"
     "${TARGET_INCLUDE_DIR}/std/StdUtils.h" )
SET( std_SOURCE
     "${TARGET_SOURCE_DIR}/std/StdUtils.cpp" )

SET( thread_INCLUDE
     "${TARGET_INCLUDE_DIR}/thread/Mutex.h"
     "${TARGET_INCLUDE_DIR}/thread/ThreadUtils.h" )
SET( thread_SOURCE
     "${TARGET_SOURCE_DIR}/thread/Mutex.cpp" )

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/TimeCommon.h"
     "${TARGET_INCLUDE_DIR}/time/Timer.h"
     "${TARGET_INCLUDE_DIR}/time/TimeUtils.h"
     "${TARGET_INCLUDE_DIR}/time/TimeWin.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/Timer.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeUtils.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeWin.cpp" )

SET( utils_INCLUDE
     "${TARGET_INCLUDE_DIR}/utils/Buffer.h"
     "${TARGET_INCLUDE_DIR}/utils/crc32.h"
     "${TARGET_INCLUDE_DIR}/utils/Lock.h"
     "${TARGET_INCLUDE_DIR}/utils/misc.h"
     "${TARGET_INCLUDE_DIR}/utils/RefPtr.h"
     "${TARGET_INCLUDE_DIR}/utils/SafeDelete.h"
     "${TARGET_INCLUDE_DIR}/utils/Singleton.h"
     "${TARGET_INCLUDE_DIR}/utils/StrConv.h"
     "${TARGET_INCLUDE_DIR}/utils/StrUtils.h" )
SET( utils_SOURCE
     "${TARGET_SOURCE_DIR}/utils/crc32.cpp"
     "${TARGET_SOURCE_DIR}/utils/misc.cpp"
     "${TARGET_SOURCE_DIR}/utils/StrConv.cpp"
     "${TARGET_SOURCE_DIR}/utils/StrUtils.cpp" )

IF( WIN32 )
    SET( win_INCLUDE
         "${TARGET_INCLUDE_DIR}/win/WinCommon.h" )
    SET( win_SOURCE
         "" )
ENDIF( WIN32 )

SET( xml_INCLUDE
     "${TARGET_INCLUDE_DIR}/xml/XmlCommon.h"
     "${TARGET_INCLUDE_DIR}/xml/XmlParser.h"
     "${TARGET_INCLUDE_DIR}/xml/XmlParserEx.h" )
SET( xml_SOURCE
     "${TARGET_SOURCE_DIR}/xml/XmlParser.cpp" )

SET( zlib_INCLUDE
     "${TARGET_INCLUDE_DIR}/zlib/ZlibCommon.h"
     "${TARGET_INCLUDE_DIR}/zlib/ZlibUtils.h" )
SET( zlib_SOURCE
     "${TARGET_SOURCE_DIR}/zlib/ZlibUtils.cpp" )

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
SOURCE_GROUP( "include\\log"     FILES ${log_INCLUDE} )
SOURCE_GROUP( "include\\net"     FILES ${net_INCLUDE} )
SOURCE_GROUP( "include\\posix"   FILES ${posix_INCLUDE} )
SOURCE_GROUP( "include\\std"     FILES ${std_INCLUDE} )
SOURCE_GROUP( "include\\thread"  FILES ${thread_INCLUDE} )
SOURCE_GROUP( "include\\time"    FILES ${time_INCLUDE} )
SOURCE_GROUP( "include\\utils"   FILES ${utils_INCLUDE} )
SOURCE_GROUP( "include\\win"     FILES ${win_INCLUDE} )
SOURCE_GROUP( "include\\xml"     FILES ${xml_INCLUDE} )
SOURCE_GROUP( "include\\zlib"    FILES ${zlib_INCLUDE} )

SOURCE_GROUP( "src"          FILES ${SOURCE} )
SOURCE_GROUP( "src\\db"      FILES ${db_SOURCE} )
SOURCE_GROUP( "src\\fs"      FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\gangsta" FILES ${gangsta_SOURCE} )
SOURCE_GROUP( "src\\log"     FILES ${log_SOURCE} )
SOURCE_GROUP( "src\\net"     FILES ${net_SOURCE} )
SOURCE_GROUP( "src\\posix"   FILES ${posix_SOURCE} )
SOURCE_GROUP( "src\\std"     FILES ${std_SOURCE} )
SOURCE_GROUP( "src\\thread"  FILES ${thread_SOURCE} )
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
             ${log_INCLUDE}     ${log_SOURCE}
             ${net_INCLUDE}     ${net_SOURCE}
             ${posix_INCLUDE}   ${posix_SOURCE}
             ${std_INCLUDE}     ${std_SOURCE}
             ${thread_INCLUDE}  ${thread_SOURCE}
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
