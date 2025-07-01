build: meson
	@ninja -C builddir

clean: meson
	@ninja -C builddir clean

debug: meson
	@ninja -C builddir debug

docs: meson
	@ninja -C builddir docs

docs-clean: meson
	@rm -r builddir/subprojects/acews/docs

browse: meson
	@ninja -C builddir browse

meson:
	@if [ ! -d builddir ]; then mkdir -p builddir; meson setup . builddir; fi

nuke:
	@rm -rf builddir

run: meson
	@ninja -C builddir run

.PHONY: build clean debug docs docs-clean meson nuke run
