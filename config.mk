
VERSION = 0.1.0

PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

CPPFLAGS = -DVERSION=\"$(VERSION)\"
CFLAGS = -std=c99 -Wall -pedantic

