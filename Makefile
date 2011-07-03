
.PHONY: ssl-refbox proto clean

all: proto ssl-refbox

ssl-refbox:
	echo "building ssl-refbox"
	make -C ssl-refbox

proto:
	echo "building proto-data"
	make -C proto

clean:
	echo "cleaning ssl-refbox"
	make -C ssl-refbox clean

