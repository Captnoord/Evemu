# - Define macro to add precompiled header to target
# Uses precompiled header while building target; currently MSVC support only.
#
#  TARGET_PRECOMPILED_HEADER( TARGET PRECOMPILED_HEADER BOUND_SOURCE )
#  TARGET - target to which add the precompiled header
#  PRECOMPILED_HEADER - the header itself
#  BOUND_SOURCE - the source the header is bound to, i.e. it is built during
#                 compilation of given source file

MACRO( TARGET_PRECOMPILED_HEADER TARGET PRECOMPILED_HEADER BOUND_SOURCE )
    IF( MSVC )
        GET_PROPERTY( CFLAGS TARGET ${TARGET} PROPERTY COMPILE_FLAGS )
        SET_PROPERTY( TARGET ${TARGET}        PROPERTY COMPILE_FLAGS "${CFLAGS} /Yu\"${PRECOMPILED_HEADER}\"" )
        SET_PROPERTY( SOURCE ${BOUND_SOURCE}  PROPERTY COMPILE_FLAGS "${CFLAGS} /Yc\"${PRECOMPILED_HEADER}\"" )
    ELSE( MSVC )
        # Don't freak out users with harmless warning
        #MESSAGE( STATUS "Unable to add precompiled header to ${TARGET}: Precompiled headers not supported." )
    ENDIF( MSVC )
ENDMACRO( TARGET_PRECOMPILED_HEADER )
