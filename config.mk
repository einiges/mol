
VERSION = 0.2.1

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -std=c99 -Wall -pedantic

