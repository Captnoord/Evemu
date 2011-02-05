#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common-std" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/common/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/common/${TARGET_NAME}" )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonStd.h" )
SET( SOURCE
     "" )

SET( db_INCLUDE
     "${TARGET_INCLUDE_DIR}/db/Type.h" )
SET( db_SOURCE
     "" )

SET( fs_INCLUDE
     "${TARGET_INCLUDE_DIR}/fs/Directory.h"
     "${TARGET_INCLUDE_DIR}/fs/File.h" )
SET( fs_SOURCE
     "" )

SET( log_INCLUDE
     "${TARGET_INCLUDE_DIR}/log/Console.h"
     "${TARGET_INCLUDE_DIR}/log/File.h"
     "${TARGET_INCLUDE_DIR}/log/LogMgr.h"
     "${TARGET_INCLUDE_DIR}/log/Message.h" )
SET( log_SOURCE
     "${TARGET_SOURCE_DIR}/log/File.cpp"
     "${TARGET_SOURCE_DIR}/log/LogMgr.cpp" )

SET( mt_INCLUDE
     "${TARGET_INCLUDE_DIR}/mt/Condition.h"
     "${TARGET_INCLUDE_DIR}/mt/Mutex.h"
     "${TARGET_INCLUDE_DIR}/mt/Target.h"
     "${TARGET_INCLUDE_DIR}/mt/Thread.h"
     "${TARGET_INCLUDE_DIR}/mt/ThreadMgr.h" )
SET( mt_SOURCE
     "${TARGET_SOURCE_DIR}/mt/Target.cpp"
     "${TARGET_SOURCE_DIR}/mt/ThreadMgr.cpp" )

SET( std_INCLUDE
     "${TARGET_INCLUDE_DIR}/std/File.h"
     "${TARGET_INCLUDE_DIR}/std/Tm.h"
     "${TARGET_INCLUDE_DIR}/std/Types.h"
     "${TARGET_INCLUDE_DIR}/std/Utils.h" )
SET( std_SOURCE
     "${TARGET_SOURCE_DIR}/std/File.cpp"
     "${TARGET_SOURCE_DIR}/std/Tm.cpp"
     "${TARGET_SOURCE_DIR}/std/Utils.cpp" )

SET( stream_INCLUDE
     "${TARGET_INCLUDE_DIR}/stream/Error.h"
     "${TARGET_INCLUDE_DIR}/stream/Hub.h"
     "${TARGET_INCLUDE_DIR}/stream/Input.h"
     "${TARGET_INCLUDE_DIR}/stream/Output.h" )
SET( stream_SOURCE
     "" )

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/Const.h"
     "${TARGET_INCLUDE_DIR}/time/Msec.h"
     "${TARGET_INCLUDE_DIR}/time/WinTime.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/Const.cpp"
     "${TARGET_SOURCE_DIR}/time/Msec.cpp"
     "${TARGET_SOURCE_DIR}/time/WinTime.cpp" )

SET( util_INCLUDE
     "${TARGET_INCLUDE_DIR}/util/Buffer.h"
     "${TARGET_INCLUDE_DIR}/util/Crc32.h"
     "${TARGET_INCLUDE_DIR}/util/Data.h"
     "${TARGET_INCLUDE_DIR}/util/Lock.h"
     "${TARGET_INCLUDE_DIR}/util/Misc.h"
     "${TARGET_INCLUDE_DIR}/util/RefPtr.h"
     "${TARGET_INCLUDE_DIR}/util/SafeDelete.h"
     "${TARGET_INCLUDE_DIR}/util/Singleton.h"
     "${TARGET_INCLUDE_DIR}/util/String.h"
     "${TARGET_INCLUDE_DIR}/util/Test.h" )
SET( util_SOURCE
     "${TARGET_SOURCE_DIR}/util/Crc32.cpp"
     "${TARGET_SOURCE_DIR}/util/Misc.cpp"
     "${TARGET_SOURCE_DIR}/util/String.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"         FILES ${INCLUDE} )
SOURCE_GROUP( "include\\db"     FILES ${db_INCLUDE} )
SOURCE_GROUP( "include\\fs"     FILES ${fs_INCLUDE} )
SOURCE_GROUP( "include\\log"    FILES ${log_INCLUDE} )
SOURCE_GROUP( "include\\mt"     FILES ${mt_INCLUDE} )
SOURCE_GROUP( "include\\std"    FILES ${std_INCLUDE} )
SOURCE_GROUP( "include\\stream" FILES ${stream_INCLUDE} )
SOURCE_GROUP( "include\\time"   FILES ${time_INCLUDE} )
SOURCE_GROUP( "include\\util"   FILES ${util_INCLUDE} )

SOURCE_GROUP( "src"         FILES ${SOURCE} )
SOURCE_GROUP( "src\\db"     FILES ${db_SOURCE} )
SOURCE_GROUP( "src\\fs"     FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\log"    FILES ${log_SOURCE} )
SOURCE_GROUP( "src\\mt"     FILES ${mt_SOURCE} )
SOURCE_GROUP( "src\\std"    FILES ${std_SOURCE} )
SOURCE_GROUP( "src\\stream" FILES ${stream_SOURCE} )
SOURCE_GROUP( "src\\time"   FILES ${time_SOURCE} )
SOURCE_GROUP( "src\\util"   FILES ${util_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}        ${SOURCE}
             ${db_INCLUDE}     ${db_SOURCE}
             ${fs_INCLUDE}     ${fs_SOURCE}
             ${log_INCLUDE}    ${log_SOURCE}
             ${mt_INCLUDE}     ${mt_SOURCE}
             ${std_INCLUDE}    ${std_SOURCE}
             ${stream_INCLUDE} ${stream_SOURCE}
             ${time_INCLUDE}   ${time_SOURCE}
             ${util_INCLUDE}   ${util_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonStd.h"
                  "log/LogMgr.cpp" )

IF( WIN32 )
    TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                           "common-win" )
ELSE( WIN32 )
    TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                           "common-posix" )
ENDIF( WIN32 )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
