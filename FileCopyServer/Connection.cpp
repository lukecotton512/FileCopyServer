//
//  Connection.cpp
//  FileCopyServer
//
//  Created by Luke Cotton on 8/2/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#include "Connection.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <ios>
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

// Our buffer size.
#define BUFSIZE 512

namespace FileCopy {
    // Creates our connection object.
    Connection::Connection(FileCopySocket connectionfd, std::string theIPAddress, std::string theFolderName) {
        // Setup our object.
        connfd = connectionfd;
        ipAddress = theIPAddress;
        folderName = theFolderName;
    }
    // Gets file from client and saves it.
    void Connection::saveFileToFolder() {
        // Print who connected.
        std::cout << ipAddress << ": Connected." << std::endl;
        // First, get our file name.
        // Create our buffer, which we will be using for later.
        char *buf = (char *)malloc(BUFSIZE);
        // Clear the buffer using memset.
        memset(buf, 0, BUFSIZE);
        // Recieve the filename from the client.
        size_t fileNameLength = recv(connfd, buf, BUFSIZE, 0);
        // Create a new buffer and copy our c-string into it.
        char *fileNameBuf = (char *)malloc(fileNameLength);
        strncpy(fileNameBuf, buf, fileNameLength);
        // Set our string object to the buffer.
        std::string fileName = fileNameBuf;
        // Free the string buffer.
        free(fileNameBuf);
        // Clear our normal buffer.
        memset(buf, 0, BUFSIZE);
        // Print file name to console.
        // Print who connected.
        std::cout << ipAddress << ": Recieving " << fileName << "." << std::endl;
        
        // Now, get the file size as in the number of blocks it has.
        recv(connfd, buf, BUFSIZE, 0);
        // Convert to a string.
        long numBlocks = atol(buf);
        // Clear the buffer.
        memset(buf, 0, BUFSIZE);
        
        // Get the remaining number of bytes.
        recv(connfd, buf, BUFSIZE, 0);
        // Convert to a string.
        long numBytes = atol(buf);
        // Clear the buffer.
        memset(buf, 0, BUFSIZE);
        
        // Now, get our destination path.
        std::string destPath = folderName + "/" + fileName;
        // Open a file at that path in binary mode.
        std::ofstream fileStream;
        fileStream.open(destPath.c_str(), std::ios::out | std::ios::binary);
        // Amount of data read.
        size_t fileSize = numBlocks * BUFSIZE + numBytes;
        // Print the file size.
        std::cout << ipAddress << ": File size: " << fileSize << " bytes." << std::endl;
        // Fake data read value to make sure we go into the loop.
        size_t dataRead = BUFSIZE + 1;
        // Go into a loop.
        while (dataRead != 0) {
            // Get a block.
            dataRead = recv(connfd, buf, BUFSIZE, 0);
            // Write the block to a file.
            fileStream.write(buf, dataRead);
            // Clear the buffer.
            memset(buf, 0, BUFSIZE);
        }
#ifdef _WIN32
		// Close the socket using closesocket.
		closesocket(connfd);
#else 
        // Close our socket.
        close(connfd);
#endif
        // Free our buffer.
        free(buf);
        // Announce we are done.
        std::cout << ipAddress << ": Done!" << std::endl;
    }
}