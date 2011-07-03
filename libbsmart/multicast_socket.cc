#include <cstdio>
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>

#include "multicast_socket.h"

using namespace BSmart;

Multicast_Socket::Multicast_Socket() throw (IO_Exception)
    : not_empty_receive_queue(0)
{
    sock = socket( AF_INET, SOCK_DGRAM, 0 );
    if ( sock == -1 )
    {
        throw IO_Exception("Create Multicast_Socket", errno);
    }
}

void Multicast_Socket::set_non_blocking()
{
    if ( -1 == fcntl(sock, F_SETFL, O_NONBLOCK) )
    {
        throw BSmart::IO_Exception("Could not set O_NONBLOCK "
                                   "in __FILE__ __LINE__", errno);
    }
}


void Multicast_Socket::set_rcvbuf(const size_t rcvbuf)
{
    if (0 == setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &rcvbuf, 4))
    {
        std::cerr << "multicast-socket: setsockopt for SO_RCVBUF failed: "
                  << strerror(errno) << std::endl;
    }

    int result;
    socklen_t result_len = sizeof(result);
    if (0 == getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &result, &result_len))
    {
        std::cerr << "multicast-socket: receive buffer set to " 
                  << result << " Bytes." << std::endl;
    }
    else
    {
        std::cerr << "multicast-socket: could not get sockopt SO_RCVBUF"
                  << std::endl;
    }
}


void Multicast_Socket::bind(const char* bind_addr, const uint16_t bind_port) throw(IO_Exception)
{
    target.sin_family = AF_INET;
    if (! inet_aton(bind_addr, &target.sin_addr))
    {
        throw IO_Exception("Bad address", errno);
    }
    target.sin_port = htons(bind_port);
    int yes = 1;
#ifdef SO_REUSEADDR
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
#endif
#ifdef SO_REUSEPORT
    setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof(yes));
#endif
    if (::bind(sock, (struct sockaddr*) &target, sizeof(target)))
    {
        throw IO_Exception("bind failed", errno);
    }

    //join multicast group for every interface
    if (IN_MULTICAST(ntohl(target.sin_addr.s_addr)))
    {
        struct ip_mreq mreq;
        struct ifconf ifc;
        struct ifreq* item;
        char buf[1024];
        
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = buf;
        if( ioctl(sock, SIOCGIFCONF, &ifc) < 0 )
        {
            throw IO_Exception("cannot get interface list", errno);
        }
        else
        {
            bool could_join(false);
            for (unsigned int i = 0; i < ifc.ifc_len / sizeof(struct ifreq); i++)
            {
                item = &ifc.ifc_req[i];
                mreq.imr_multiaddr = target.sin_addr;
                mreq.imr_interface = ((struct sockaddr_in *)&item->ifr_addr)->sin_addr;
                if (0 == setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                    (void*)&mreq, sizeof(mreq)))
                    
                {
                    could_join = true;
                }
            }

            if (! could_join)
            {
                throw IO_Exception("join multicast group failed for interface", errno);
            }
        }
    }
}

ssize_t Multicast_Socket::write(
    const void* buffer,
    const size_t buflen) throw (IO_Exception)
{
    ssize_t result = sendto(sock, buffer, buflen, 0,
                            (struct sockaddr*) &target, sizeof(target));
    if (result == -1)
    {
        throw IO_Exception("send failed", errno);
    }
    return result;
}

ssize_t Multicast_Socket::read(
    void* buffer,
    const size_t buflen,
    sockaddr* from,
    socklen_t* fromlen) throw(IO_Exception)
{
    ssize_t result = recvfrom(sock, buffer, buflen, 0, from, fromlen);
    if (result == -1)
    {
        if (errno == EAGAIN)
        {
            throw No_Data_Available();
        }
        else
        {
            throw IO_Exception("recvfrom failed", errno);
        }
    }


    //check size of next packet. Print a warning if there was a packet in
    //the buffer multiple times.
    int next_size = -1;
    if (-1 == ioctl(sock, FIONREAD, &next_size))
    {
        perror("Multicast_Socket::read(): can not get size of next packet in receive queue");
    }
    if (next_size)
    {
        if (not_empty_receive_queue > 50)
        {
            std::cerr << "Multicast_Socket::read(): WARNING: Receive-Queue not empty for some time."
                      << std::endl;
        }
        else
        {
            ++not_empty_receive_queue;
        }
    }
    else
    {
        not_empty_receive_queue = 0;
    }
    
    
        
    
    return result;
}

ssize_t Multicast_Socket::read_select(void* buffer, const size_t buflen, const int msec)
{
    fd_set r,w,e;
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = msec * 1000;

    FD_ZERO(&r);
    FD_ZERO(&w);
    FD_ZERO(&e);
    FD_SET(sock, &r);

    int retval = select(sock + 1, &r, &w, &e, &timeout);
    if (retval == -1)
    {
        //error
        throw IO_Exception("select failed", errno);
    }

    return read(buffer, buflen);
}


