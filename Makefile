.PHONY : install-build-deps
install-build-deps : lib/googletest

lib/googletest :
	mkdir -p lib
	mkdir -p lib/googletest
	git clone https://github.com/google/googletest.git lib/googletest
