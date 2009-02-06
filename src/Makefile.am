INCLUDES += -I$(srcdir)/../../3rdparty/
INCLUDES += -I$(srcdir)/../../include/ascent-shared/
INCLUDES += -I$(srcdir)/../../include/Auth/
INCLUDES += -I$(srcdir)/../../include/Communication/
INCLUDES += -I$(srcdir)/../../include/PythonObjects/
INCLUDES += -I$(srcdir)/../../include/server/
INCLUDES += -I$(srcdir)/../../include/TestCases/
INCLUDES += -I$(srcdir)/../../include/Utilities/



bin_PROGRAMS = evemu_server

server_SOURCES = \
	Auth/PasswordModule.cpp \
	Communication/MarshalStream.cpp \
	Communication/PyStream.cpp \
	Communication/PyStringTable.cpp \
	PythonObjects/PyObjects.cpp \
	server/AccountMgr.cpp \
	server/EveClientSession.cpp \
	server/EveClientSocket.cpp \
	main.cpp

server_LDADD = \
  -lascent \
  -ltinyxml \
  -lz \
  -lpthread \
  -ldl 

server_LDFLAGS = \
	-L$(srcdir)/../../3rdparty/tinyxml \
	-L$(srcdir)/../ascent-shared \
	@MYSQL_LDFLAGS@

AM_CXXFLAGS+= \
	-pipe \
	-fno-inline \
	-Wall \
	-ansi
