DESTDIR=
PREFIX=/usr

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -m755 spkg $(DESTDIR)$(PREFIX)/bin/