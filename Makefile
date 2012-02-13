
.PHONY: ssl-refbox proto clean

all: proto log4cxx ssl-refbox

ssl-refbox:
	echo "building ssl-refbox"
	qmake -o ssl-refbox/Makefile ssl-refbox/ssl-refbox.pro
	make -C ssl-refbox

proto:
	echo "building proto-data"
	make -C proto

clean:
	make -C ssl-refbox clean
	make -C proto clean

