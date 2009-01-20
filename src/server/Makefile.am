INCLUDES += -I$(srcdir)/../../3rdparty/
INCLUDES += -I$(srcdir)/../../include/common/
INCLUDES += -I$(srcdir)/../../include/server/
INCLUDES += -I$(srcdir)/../../include/ascent-shared/
INCLUDES += -I$(srcdir)/../rewrite/


bin_PROGRAMS = server

server_SOURCES = \
	main.cpp

server_LDADD = \
  -lascent \
  -lcommon \
  -lrewrite \
  -ltinyxml \
  -lz \
  -lpthread \
  -ldl 

server_LDFLAGS = \
	-L$(srcdir)/../../3rdparty/tinyxml \
	-L$(srcdir)/../ascent-shared \
	-L$(srcdir)/../common/ \
	-L$(srcdir)/../rewrite/ \
	@MYSQL_LDFLAGS@

AM_CXXFLAGS+= \
	-pipe \
	-fno-inline \
	-Wall \
	-ansi
