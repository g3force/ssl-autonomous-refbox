
.PHONY: all

all: messages_robocup_ssl_detection.pb.h messages_robocup_ssl_geometry.pb.h messages_robocup_ssl_wrapper.pb.h messages_robocup_ssl_refbox_log.pb.h

%.pb.h: %.proto
	protoc --cpp_out=. $<

clean:
	rm -f *.cc *.h
