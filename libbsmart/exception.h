#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <errno.h>
#include <cstring>
#include <iostream>

namespace BSmart
{
    /**
     * General Exception class
     */
    class Exception: public std::exception
    {
    public:
        Exception(): reason("internal error")
        {
        }
        
        Exception(const std::string& reason): reason(reason)
        {
        }
        
        virtual ~Exception() throw () {};
        

        virtual const char* what() const throw ()
        {
            return reason.c_str();
        }
    protected:
        std::string reason;
    };


    /**
     * IO Error: Networking, File Handling,...
     */
    class IO_Exception: public Exception {
    public:
        IO_Exception(const std::string& what, const int error)
        {
            reason = what + ": " + strerror(error);
        }
        
        IO_Exception(const std::string& what)
        {
            reason = what;
        }  
        
        virtual ~IO_Exception() throw () {};
    };
    
    /**
     * Resource not available (i.e. camera not connected)
     */
    class Resource_Unavailable_Exception : public Exception {
    public: 
        Resource_Unavailable_Exception(const std::string& reason)
             : Exception(reason) {};
        virtual ~Resource_Unavailable_Exception() throw () {};
    };
    
    /**
     * Allocation for something failes (i.e. SDL surface, ...)
     */
    class Allocation_Failed_Exception : public Exception {
    public: 
        Allocation_Failed_Exception(const std::string& reason)
             : Exception(reason) {};
        virtual ~Allocation_Failed_Exception() throw () {};
    };
}

#endif //EXCEPTION_H
