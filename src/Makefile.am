INCLUDES += -I$(srcdir)/../3rdparty/
INCLUDES += -I$(srcdir)/../include/ascent-shared/
INCLUDES += -I$(srcdir)/../include/Auth/
INCLUDES += -I$(srcdir)/../include/Communication/
INCLUDES += -I$(srcdir)/../include/PCH/
INCLUDES += -I$(srcdir)/../include/PythonObjects/
INCLUDES += -I$(srcdir)/../include/server/
INCLUDES += -I$(srcdir)/../include/server/Session/
INCLUDES += -I$(srcdir)/../include/server/Space/
INCLUDES += -I$(srcdir)/../include/TestCases/
INCLUDES += -I$(srcdir)/../include/Utilities/



bin_PROGRAMS = evemuserver

evemuserver_SOURCES = \
	Auth/PasswordModule.cpp \
	Communication/MarshalReadStream.cpp \
	Communication/MarshalStream.cpp \
	Communication/MarshalWriteStream.cpp \
	Communication/PyStream.cpp \
	Communication/PyStringTable.cpp \
	PythonObjects/PyObjects.cpp \
	server/Session/EveSession.cpp \
	server/Space/RegionMapManager.cpp \
	server/Space/Space.cpp \
	server/Space/SpaceRunnable.cpp \
	server/AccountMgr.cpp \
	server/EveClientSession.cpp \
	server/EveClientSocket.cpp \
	main.cpp

evemuserver_LDADD = \
  -lascent \
  -ltinyxml \
  -lz \
  -lpthread \
  -ldl 

evemuserver_LDFLAGS = \
	-L$(srcdir)/../3rdparty/tinyxml \
	-L$(srcdir)/ascent-shared \
	@MYSQL_LDFLAGS@

AM_CXXFLAGS+= \
	-pipe \
	-fno-inline \
	-Wall \
	-ansi
