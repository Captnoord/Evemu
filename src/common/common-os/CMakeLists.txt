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

#########
# Files #
#########
SET( INCLUDE
     "${TARGET_INCLUDE_DIR}/CommonOs.h" )
SET( SOURCE
     "" )

SET( log_INCLUDE
     "" )
SET( log_SOURCE
     "${TARGET_SOURCE_DIR}/log/Message.cpp" )

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

SET( time_INCLUDE
     "${TARGET_INCLUDE_DIR}/time/TimeMgr.h"
     "${TARGET_INCLUDE_DIR}/time/Timer.h"
     "${TARGET_INCLUDE_DIR}/time/Timeval.h"
     "${TARGET_INCLUDE_DIR}/time/Utils.h" )
SET( time_SOURCE
     "${TARGET_SOURCE_DIR}/time/Msec.cpp"
     "${TARGET_SOURCE_DIR}/time/TimeMgr.cpp"
     "${TARGET_SOURCE_DIR}/time/Timer.cpp"
     "${TARGET_SOURCE_DIR}/time/Timeval.cpp"
     "${TARGET_SOURCE_DIR}/time/WinTime.cpp" )

#####################
# Setup the library #
#####################
INCLUDE_DIRECTORIES( "${common-std_INCLUDE_DIR}"
                     "${TARGET_INCLUDE_DIR}" )

SOURCE_GROUP( "include"       FILES ${INCLUDE} )
SOURCE_GROUP( "include\\log"  FILES ${log_INCLUDE} )
SOURCE_GROUP( "include\\net"  FILES ${net_INCLUDE} )
SOURCE_GROUP( "include\\time" FILES ${time_INCLUDE} )

SOURCE_GROUP( "src"       FILES ${SOURCE} )
SOURCE_GROUP( "src\\log"  FILES ${log_SOURCE} )
SOURCE_GROUP( "src\\net"  FILES ${net_SOURCE} )
SOURCE_GROUP( "src\\time" FILES ${time_SOURCE} )

ADD_LIBRARY( "${TARGET_NAME}"
             ${INCLUDE}      ${SOURCE}
             ${log_INCLUDE}  ${log_SOURCE}
             ${net_INCLUDE}  ${net_SOURCE}
             ${time_INCLUDE} ${time_SOURCE} )

TARGET_BUILD_PCH( "${TARGET_NAME}"
                  "CommonOs.h"
                  "net/Ip4.cpp" )
TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                       "common-std" )

IF( WIN32 )
    TARGET_LINK_LIBRARIES( "${TARGET_NAME}"
                           "wsock32" )
ENDIF( WIN32 )

######################
# Export directories #
######################
SET( ${TARGET_NAME}_INCLUDE_DIR "${TARGET_INCLUDE_DIR}" PARENT_SCOPE )
