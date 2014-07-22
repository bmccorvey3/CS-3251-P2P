//
//  main.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include <iostream>

int main(int argc, const char * argv[])
{

    // insert code here...
    void* parse;
    char string[100] =  "Hello trying to parse";
    parse = &string;
    std::string *sp = static_cast<std::string*>(parse);
    std::string s = *sp;
    
    std::cout << s.at(s.find_last_of("ing")) << std::endl;
    
    return 0;
}

