//
//  Connection.hpp
//  FileCopyServer
//
//  Created by Luke Cotton on 8/2/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#ifndef Connection_hpp
#define Connection_hpp

#include <string>

// Our socket data type.
#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#define BADSOCKET INVALID_SOCKET
typedef SOCKET FileCopySocket;
#else
#define BADSOCKET -1
typedef int FileCopySocket;
#endif

namespace FileCopy {
    class Connection {
        // Instance variables.
        private:
            FileCopySocket connfd;
            std::string ipAddress;
            std::string folderName;
        public:
            Connection(FileCopySocket connectionfd, std::string theIPAddress, std::string theFolderName);
            void saveFileToFolder();
    
    };
}

#endif /* Connection_hpp */
