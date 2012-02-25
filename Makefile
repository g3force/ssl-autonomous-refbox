
.PHONY: ssl-refbox proto clean install

all: proto ssl-refbox

ssl-refbox:
	echo "Configure, if not done already"
	./configure
	echo "Running qmake"
	qmake -o ssl-refbox/Makefile ssl-refbox/ssl-refbox.pro
	echo "building ssl-refbox"
	make -C ssl-refbox

proto:
	echo "building proto-data"
	make -C proto

clean:
	if [ -f "./ssl-refbox/Makefile" ]; then make -C ssl-refbox clean; fi	
	make -C proto clean
	rm -f ssl-refbox/*.orig
	rm -f ssl-refbox/ssl-refbox.pro
	rm -f ssl-refbox/Makefile
	rm -f bin/ssl-autonomous-refbox
	rm -f bin/ssl-autonomous-refbox.log*

install:
	mkdir -p ${DESTDIR}/usr/bin/
	cp bin/ssl-autonomous-refbox ${DESTDIR}/usr/bin/
	mkdir -p ${DESTDIR}/etc/
	if [ ! -f "${DESTDIR}/etc/ssl-autonomous-refbox.conf" ]; then cp bin/ssl-autonomous-refbox.conf ${DESTDIR}/etc/; fi

