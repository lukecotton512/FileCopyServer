//
//  threadmain.cpp
//  FileCopyServer
//
//  Created by Luke Cotton on 8/3/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#include "threadmain.hpp"

#include <string>

#include "Connection.hpp"
#include "Server.hpp"

// Our main function for our thread.
void * connMain(void *connPtr) {
    // Get the pointer to our object via typecasting.
    FileCopy::Connection *connection = (FileCopy::Connection *)connPtr;
    
    // Call the save file function.
    connection->saveFileToFolder();
    
    // free the connection object.
    delete connection;
    
    // Return null;
    return NULL;
}

// Our server main function.
void * serverMain(void *serverArg) {
    // Get our folder argument
    FileCopy::Server * server = (FileCopy::Server *)serverArg;
    
    // Start the server.
    server->start();
    
    // Delete the server object.
    delete server;
    
    // Return null;
    return NULL;
}
