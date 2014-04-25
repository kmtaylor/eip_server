bin_PROGRAMS = eip_server

eip_server_SOURCES = main.c cip_translate.c modbus_thread.c
eip_server_CFLAGS = $(CIP_CFLAGS) $(MODBUS_CFLAGS)
eip_server_LDADD = $(CIP_LIBS) $(MODBUS_LIBS)

EXTRA_DIST = modbus_thread.h
