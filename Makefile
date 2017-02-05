OBJS = main.o

## For Windows
OBJS += compat.o
EXEC_SUFFIX = .exe
CC = mingw32-gcc
CFLAGS = -DCOMPAT_MINGW_MS_VSNPRINTF -Wall -O3
LDFLAGS = -L.
RM = del

## For Mac OS X (Fink)
#EXEC_SUFFIX =
#CFLAGS = -Wall -O3
#LDFLAGS = -L/sw/lib
#RM = /bin/rm -f

usbpctest$(EXEC_SUFFIX): main.o compat.o
	$(CC) -o $@ $(OBJS) $(LDFLAGS) -lusb-1.0

clean:
	$(RM) $(OBJS) usbpctest$(EXEC_SUFFIX)
