//
//  Server.hpp
//  FileCopyServer
//
//  Created by Luke Cotton on 7/14/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#ifndef Server_hpp
#define Server_hpp

#include <string>
#ifdef _WIN32
#include <WinSock2.h>
#include <Ws2tcpip.h>
#endif

// Our socket data type.
#ifdef _WIN32
#define BADSOCKET INVALID_SOCKET
typedef SOCKET FileCopySocket;
#else
#define BADSOCKET -1
typedef int FileCopySocket;
#endif

// Server class
namespace FileCopy {
    class Server {
    private:
        // Instance variables.
        std::string ipAddress;
        std::string port;
		std::string folderPath;
        bool keepGoing;
		FileCopySocket sockfd;
        // Private methods.
        bool setupServer();
        void takeDownServer();
        
    public:
        // Constructors.
        Server();
        Server(std::string theAddress, std::string thePort, std::string theFolder);
        // Getters and setters.
        std::string getIPAddress();
        std::string getPort();
        void setPort(std::string thePort);
        void setIPAddress(std::string theIPAddress);
        // Server methods.
        void start();
        void stop();
    };
}
#endif /* Server_hpp */
