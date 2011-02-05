#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common-posix" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/common/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/common/${TARGET_NAME}" )

FIND_PACKAGE( "Threads" REQUIRED )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonPosix.h" )
SET( SOURCE
     "" )

SET( fs_INCLUDE
     "" )
SET( fs_SOURCE
     "${TARGET_SOURCE_DIR}/fs/Directory.cpp"
     "${TARGET_SOURCE_DIR}/fs/File.cpp" )

SET( log_INCLUDE
     "" )
SET( log_SOURCE
     "${TARGET_SOURCE_DIR}/log/Console.cpp" )

SET( mt_INCLUDE
     "" )
SET( mt_SOURCE
     "${TARGET_SOURCE_DIR}/mt/Condition.cpp"
     "${TARGET_SOURCE_DIR}/mt/Mutex.cpp"
     "${TARGET_SOURCE_DIR}/mt/Thread.cpp" )

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

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/Timespec.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/Msec.cpp"
     "${TARGET_SOURCE_DIR}/time/Timespec.cpp"
     "${TARGET_SOURCE_DIR}/time/Timeval.cpp"
     "${TARGET_SOURCE_DIR}/time/WinTime.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${common-std_INCLUDE_DIR}" "${common-os_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"        FILES ${INCLUDE} )
SOURCE_GROUP( "include\\fs"    FILES ${fs_INCLUDE} )
SOURCE_GROUP( "include\\log"   FILES ${log_INCLUDE} )
SOURCE_GROUP( "include\\mt"    FILES ${mt_INCLUDE} )
SOURCE_GROUP( "include\\posix" FILES ${posix_INCLUDE} )
SOURCE_GROUP( "include\\time"  FILES ${time_INCLUDE} )

SOURCE_GROUP( "src"        FILES ${SOURCE} )
SOURCE_GROUP( "src\\fs"    FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\log"   FILES ${log_SOURCE} )
SOURCE_GROUP( "src\\mt"    FILES ${mt_SOURCE} )
SOURCE_GROUP( "src\\posix" FILES ${posix_SOURCE} )
SOURCE_GROUP( "src\\time"  FILES ${time_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}       ${SOURCE}
             ${fs_INCLUDE}    ${fs_SOURCE}
             ${log_INCLUDE}   ${log_SOURCE}
             ${mt_INCLUDE}    ${mt_SOURCE}
             ${posix_INCLUDE} ${posix_SOURCE}
             ${time_INCLUDE}  ${time_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonPosix.h"
                  "Fd.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-os"
                       ${CMAKE_THREAD_LIBS_INIT} )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
