//
//  Server.cpp
//  FileCopyServer
//
//  Created by Luke Cotton on 7/14/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#include "Server.hpp"

// Include a whole bunch of different files, depending on platform.
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#endif

// Our includes.
#include "Connection.hpp"
#include "threadmain.hpp"
#ifdef _WIN32
#include "WinThreadMain.h"
#endif


// Namespace declaration
namespace FileCopy {
    // Constructors.
    Server::Server() {
        // Set IP address and port to defaults.
        ipAddress = "";
        port = "7979";
    }
    Server::Server(std::string theAddress, std::string thePort, std::string theFolder) {
        // Set IP address and port
        ipAddress = theAddress;
        port = thePort;
        folderPath = theFolder;
    }
    // Getters and setters.
    std::string Server::getIPAddress() {
        return ipAddress;
    }
    std::string Server::getPort() {
        return port;
    }
    void Server::setPort(std::string thePort) {
        port = thePort;
    }
    void Server::setIPAddress(std::string theIPAddress) {
        // Set the IP address to the IP address.
        ipAddress = theIPAddress;
    }
    // Server methods.
    void Server::start() {
        // Set keepGoing to true.
        keepGoing = true;
        // Keep going.
        while (keepGoing) {
            // Start listening for connections.
            if (listen(sockfd, 5) != 0) {
                // We have an error, print it and exit.
                perror("Error");
                return;
            }
            
            // Accept a connection.
            struct sockaddr_storage connInfo;
            socklen_t connInfoLen = sizeof(connInfo);
            int connfd = accept(sockfd, (struct sockaddr *)&connInfo, &connInfoLen);
            if (connfd > 0) {
                // Get the name of our host.
                char * hostNameCStr = (char *)malloc(NI_MAXHOST);
                getnameinfo((struct sockaddr *)&connInfo, connInfoLen, hostNameCStr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                std::string hostName = hostNameCStr;
                // Load up a connection object with our info.
                Connection *conn = new Connection(connfd, hostName, folderPath);
				// We are on Windows.
#ifdef _WIN32
				// Use CreateThread to create a new connection thread.
				CreateThread(NULL, 0, connWinMain, conn, 0, NULL);
#else
				// We are on Unix.
                // Call pthread_create, giving it our function to call and the server object.
                pthread_t threadid;
                pthread_create(&threadid, NULL, connMain, conn);
                // Detach thread from main thread.
                pthread_detach(threadid);
#endif
                // Free buffer
                free(hostNameCStr);
            }
        }
    }
    void Server::stop() {
        // Set the keepGoing variable to false.
        keepGoing = false;
        // Call takeDownServer.
        takeDownServer();
    }
    // Private methods.
    bool Server::setupServer() {
        // Setup our required structures.
        struct addrinfo hints, *res;
        
        // Zero out hints.
        memset(&hints, 0, sizeof(hints));
        
        // This means either IPv6 or IPv4.
        hints.ai_family = AF_UNSPEC;
        // This means we want to create a stream socket, for TCP communication.
        hints.ai_socktype = SOCK_STREAM;
        // Fill in everything for us.
        hints.ai_flags = AI_PASSIVE;
        
        // Convert IP address to a c-style string.
        char * ipAddrCStr = (char *)ipAddress.c_str();
        if (ipAddress == "") {
            // Set ipAddCstr to null.
            ipAddrCStr = NULL;
        }
        // Call getaddrinfo in order to fill in res.
        int result = getaddrinfo(ipAddrCStr, port.c_str(), &hints, &res);
        if (result != 0) {
            // Print error message and return.
            std::cerr << "Error using getaddrinfo." << std::endl;
            std::cerr << gai_strerror(result) << std::endl;
            return false;
        }
        // Call socket to create our new socket file descriptor.
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        // Make sure we have a valid socket file descriptor.
        if (sockfd == BADSOCKET) {
            // We have an error, print it and exit.
            perror("Error");
            // Free our address information.
            freeaddrinfo(res);
            // Return.
            return false;
        }
        // Set the socket option, if IPv6, to IPv6 only.
        if (res->ai_family == AF_INET6) {
            int IPV6Only = 1;
            if (setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &IPV6Only, sizeof(IPV6Only)) == -1) {
                perror("Error");
                return false;
            }
        }
        // Bind our socket to its port
        if (bind(sockfd, (struct sockaddr *)res->ai_addr, res->ai_addrlen) != 0) {
            // Print an error, because we have one.
            perror("Error");
            // Free our address information.
            freeaddrinfo(res);
            // Return.
            return false;
        }
        // Free our address information.
        freeaddrinfo(res);
        // We were successful, return true.
        return true;
        
    }
    // Takes down the server
    void Server::takeDownServer() {
#ifdef _WIN32
		// Close the socket the Windows way.
		closesocket(sockfd);
#else
        // Close our socket file descriptor.
        close(sockfd);
#endif
    }
}

