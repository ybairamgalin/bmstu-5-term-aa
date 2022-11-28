.PHONY : install-build-deps
install-build-deps : lib/googletest lib/fmt

lib/googletest :
	mkdir -p lib
	mkdir -p lib/googletest
	git clone https://github.com/google/googletest.git lib/googletest

lib/fmt :
	mkdir -p lib
	mkdir -p lib/fmt
	git clone https://github.com/fmtlib/fmt.git lib/fmt
	mkdir -p lib/fmt/build
	cd lib/fmt/build && cmake ..

.PHONY : build-all
build-all :
	cmake -Bcmake-build -S.
	cd cmake-build/ && make

.PHONY : clean-report
clean-report:
	rm  -rf */docs/*.aux
	rm  -rf */docs/*.bbl
	rm  -rf */docs/*.log
	rm  -rf */docs/*.out
	rm  -rf */docs/*.gz
	rm  -rf */docs/*.toc
	rm  -rf */docs/*.blg

.PHONY : clean
clean : clean-report
	rm -rf out
	rm -rf */out
	rm -rf cmake-build-clion
	rm -rf cmake-build
	find . -name '*.tmp*' -print -delete
