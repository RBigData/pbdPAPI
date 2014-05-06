papi_home = "./papi-5.3.0/src"


all: mk_papi mk_pkg 

mk_papi:
	cd $(papi_home) && \
		CFLAGS='-fPIC' ./configure --prefix=`pwd` && \
		make && \
		make install


mk_pkg:
	mkdir -p build;
	cd build; \
	cmake ..; \
	make install


clean:
	clean:
	rm -rf ./build

