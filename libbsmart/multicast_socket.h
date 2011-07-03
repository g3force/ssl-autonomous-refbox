#ifndef MULTICAST_SOCKET_H
#define MULTICAST_SOCKET_H

#include <netinet/in.h>
#include "exception.h"

namespace BSmart
{
    class Multicast_Socket
    {
    public:

        class No_Data_Available: public BSmart::IO_Exception
        {
        public:
            No_Data_Available()
                : BSmart::IO_Exception("No data available", EAGAIN)
            {
            };

            virtual ~No_Data_Available() throw () 
            {
            };
        };
        

        // Construct new Multicast_Socket
        Multicast_Socket() throw(BSmart::IO_Exception);

        void bind(const char* addr, const uint16_t port) throw(BSmart::IO_Exception);

        // write data to socket. returns number of bytes written
        // or -1 on failure.
        ssize_t write(const void* buffer, const size_t buflen) throw(BSmart::IO_Exception);

        // read data from socket. blocks until data could be
        // read, source address is stored in "from".
        ssize_t read(void* buffer, const size_t buflen,
                     sockaddr* from = 0, socklen_t* fromlen = 0) throw(BSmart::IO_Exception);

        // read data from socket. Wait msec for data
        ssize_t read_select(void* buffer, const size_t buflen, const int msec);
        
        // does what the name suggests. see "man 2 socket".
        void set_non_blocking();
        
        /**
         * Set size of receive buffer (see "man 7 socket").
         *
         * @param rcvbuf   Size of buffer in bytes.
         */
        void set_rcvbuf(const size_t rcvbuf);

    protected:
        int sock;
        struct sockaddr_in target;
        int not_empty_receive_queue;
    };
}

#endif
