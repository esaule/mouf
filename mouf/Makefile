all: config install

config: local.properties build.xml

install:
	ndk-build
	ant release install


local.properties build.xml:
	android update project --name MouF --path .

clean:
	ndk-build clean
	ant clean
	rm -Rf obj libs


.PHONY: all config clean
