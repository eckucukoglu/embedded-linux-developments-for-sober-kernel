# *****************************************************
# Parameters to control Makefile operation

# CC = gcc

DBUS_LIB =  -ldbus-1
DBUS_INCLUDE = -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include
LIBS = -lm -lpthread -lcrypto $(DBUS_LIB)

CFLAGS = -Wall
DEBUG = -g -DDEBUG

DAEMON = appmand
TESTER = tester

# ****************************************************

all: cJSON.o appmand.c
	$(CC) $(CFLAGS) $(DBUS_INCLUDE) -o $(DAEMON) appmand.c cJSON.o $(LIBS)

debug: CFLAGS += $(DEBUG)
debug: all

cJSON.o: cJSON.c cJSON.h
	$(CC) $(CFLAGS) -c cJSON.c

tester: test/tester.c test/tester.h
	$(CC) $(CFLAGS) $(DBUS_INCLUDE) -o $(TESTER) test/tester.c $(LIBS)

clean:
	rm -f *.o
	rm -f $(DAEMON)
	rm -f $(TESTER)
