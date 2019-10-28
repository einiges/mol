
VERSION = 0.2.0

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -std=c99 -Wall -pedantic

