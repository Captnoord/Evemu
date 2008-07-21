ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign
EXTRA_DIST = configure

SUBDIRS = common xmlpktgen packets common
# yes common, twice, after compiling packets, its updated

if COLLECTOR
SUBDIRS+= collector
endif
