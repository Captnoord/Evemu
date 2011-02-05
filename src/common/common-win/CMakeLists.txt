#
# CMake build system file for EVEmu.
#
# Author: Bloody.Rabbit
#

##############
# Initialize #
##############
SET( TARGET_NAME        "common-win" )
SET( TARGET_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include/common/${TARGET_NAME}" )
SET( TARGET_SOURCE_DIR  "${PROJECT_SOURCE_DIR}/src/common/${TARGET_NAME}" )

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonWin.h" )
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

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${common-std_INCLUDE_DIR}" "${common-os_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"      FILES ${INCLUDE} )
SOURCE_GROUP( "include\\fs"  FILES ${fs_INCLUDE} )
SOURCE_GROUP( "include\\log" FILES ${log_INCLUDE} )
SOURCE_GROUP( "include\\mt"  FILES ${mt_INCLUDE} )
SOURCE_GROUP( "include\\win" FILES ${win_INCLUDE} )

SOURCE_GROUP( "src"      FILES ${SOURCE} )
SOURCE_GROUP( "src\\fs"  FILES ${fs_SOURCE} )
SOURCE_GROUP( "src\\log" FILES ${log_SOURCE} )
SOURCE_GROUP( "src\\mt"  FILES ${mt_SOURCE} )
SOURCE_GROUP( "src\\win" FILES ${win_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}     ${SOURCE}
             ${fs_INCLUDE}  ${fs_SOURCE}
             ${log_INCLUDE} ${log_SOURCE}
             ${mt_INCLUDE}  ${mt_SOURCE}
             ${win_INCLUDE} ${win_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonWin.h"
                  "win/Handle.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-os" )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
