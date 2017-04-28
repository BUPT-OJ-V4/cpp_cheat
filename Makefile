.PHONY: all release test clean
pkg_name = $(shell echo CheatServer-`date +%Y%m%d-%H%M%S`.tar.gz)
all: release

release:
	sh build.sh && mkdir -p ./release && cd ./build/release && tar -zcvf ../../release/$(pkg_name) ./*;
clean:
	rm -rf ./release
