PREFIX=
DESTDIR=

install:
	mkdir -p $(PREFIX)$(DESTDIR)/bin/ $(PREFIX)$(DESTDIR)/etc/spkg.d/repos
	install -m644 spkg.conf   $(DESTDIR)/etc/
	install -m755 spkg-rindex $(PREFIX)$(DESTDIR)/bin/
	install -m755 spkg-install $(PREFIX)$(DESTDIR)/bin/