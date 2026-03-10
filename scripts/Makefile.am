.PHONY: \
	logme

EXTRA_DIST = \
	porg_bash_completion

bin_SCRIPTS = \
	paco2porg \
	porgball

install-exec-local:
		$(mkinstalldirs) $(DESTDIR)$(datadir)/bash-completions/completions && \
		$(INSTALL_DATA) porg_bash_completion $(DESTDIR)$(datadir)/bash-completions/completions/porg

logme_files = \
	$(DESTDIR)$(datadir)/bash-completions/completions/porg \
	$(DESTDIR)$(bindir)/paco2porg \
	$(DESTDIR)$(bindir)/porgball

logme:
	ls $(logme_files) | porg -lp+ porg-$(PACKAGE_VERSION)

