//
//  main.cpp
//  FileCopyServer
//
//  Created by Luke Cotton on 7/14/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#include <iostream>
#include <string>
#include <sys/stat.h>
#include "Server.hpp"
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "WinThreadMain.h"
#else
#include <pthread.h>
#endif

#include "threadmain.hpp"

int main(int argc, const char * argv[]) {
    // Parse command line arguments.
    if (argc > 1) {
        // Tell the user what we are doing.
        std::cout << "FileCopyServer" << std::endl;
        std::cout << "By: Luke Cotton" << std::endl;
        // Local variables.
        char *folderNameCStr;
        // Get the folder name.
        folderNameCStr = (char *)argv[1];
        // Check and make sure it exists, and that it is a folder.
        struct stat statStruct;
        int errorCode = stat(folderNameCStr, &statStruct);
        if (errorCode == -1) {
            // Print an error message.
            perror("Error");
        } else {
            // Check the stat structure to make sure we are a folder.
            if (S_ISDIR(statStruct.st_mode)) {
                // Tell the user how to exit.
                std::cout << "Press a key at any time to exit." << std::endl;
#ifdef _WIN32
                // Setup our WinSock data for the server application.
                WSAData winSockData;
                int result = WSAStartup(MAKEWORD(2, 0), &winSockData);
                // Make sure we have a valid result.
                if (result != 0) {
                    // Print that we have an error.
                    std::cout << "Error setting up WinSock!" << std::endl;
                    // And get out of here.
                    return 0;
                }
#endif
                // Our folder name as a C++ string.
                std::string folderName = folderNameCStr;
                // Create new server objects.
                FileCopy::Server *serverIPv4 = new FileCopy::Server("0.0.0.0", "7979", folderName);
                FileCopy::Server *serverIPv6 = new FileCopy::Server("::", "7979", folderName);
                // Create threads for our server for both IPv4 and IPv6.
#ifdef _WIN32
                // Create our threads using CreateThread.
                CreateThread(NULL, 0, serverWinMain, serverIPv4, 0, NULL);
                CreateThread(NULL, 0, serverWinMain, serverIPv6, 0, NULL);
#else
                // Create the new threads to start the server objects using libpthread.
                pthread_t serverIPv4ThreadID;
                pthread_create(&serverIPv4ThreadID, NULL, serverMain, serverIPv4);
                pthread_detach(serverIPv4ThreadID);
                pthread_t serverIPv6ThreadID;
                pthread_create(&serverIPv6ThreadID, NULL, serverMain, serverIPv6);
                pthread_detach(serverIPv6ThreadID);
#endif
                // Announce we are listening for connections.
                std::cout << "Server listening for connections." << std::endl;
                // Hold the main thread until someone types something in.
                std::cin.ignore();
                // Stop both servers.
                serverIPv4->stop();
                serverIPv6->stop();
                // Say we are exiting.
                std::cout << "Exiting!" << std::endl;
#ifdef _WIN32
                // Make sure to clean up our WinSock data before exiting.
                WSACleanup();
#endif
            } else {
                // We are not a folder, so print an error.
                std::cerr << "Error: Not a folder." << std::endl;
            }
        }
    } else {
        // Print proper usage.
        std::cout << "Usage: " << argv[0] << " <foldername>" << std::endl;
    }
    // Return.
    return 0;
}
