
include config.mk

SRC := mol.c
OBJ := $(SRC:.c=.o)

all: mol

mol: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(LDLIBS) $(OBJ)

$(OBJ): %.o : %.c
	$(CC) -c $< $(CFLAGS) $(CPPFLAGS)

clean:
	$(RM) mol $(OBJ) mol-$(VERSION).tar.gz

dist: clean
	mkdir -p mol-$(VERSION)
	cp -R Makefile config.mk LICENSE README mol.1 arg.h $(SRC) mol-$(VERSION)
	tar -cf - mol-$(VERSION) | gzip > mol-$(VERSION).tar.gz
	$(RM) -r mol-$(VERSION)

install: mol
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f mol $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/mol
	mkdir -p $(DESTDIR)/$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < mol.1 > $(DESTDIR)$(MANPREFIX)/man1/mol.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/mol.1

uninstall:
	$(RM) $(DESTDIR)$(PREFIX)/bin/mol
	$(RM) $(DESTDIR)$(MANPREFIX)/man1/mol.1

.PHONY: all clean dist install uninstall

