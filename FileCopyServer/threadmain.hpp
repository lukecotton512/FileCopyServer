//
//  threadmain.hpp
//  FileCopyServer
//
//  Created by Luke Cotton on 8/3/16.
//  Copyright © 2016 Luke Cotton. All rights reserved.
//

#ifndef threadmain_hpp
#define threadmain_hpp

#include <stdio.h>

// Thread main functions.
void * serverMain(void *serverArg);
void * connMain(void *connPtr);

#endif /* threadmain_hpp */
