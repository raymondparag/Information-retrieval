//
//  webquery.cpp
//  Information Retrieval
//
//  Created by Raymond Parag on 28-02-16.
//  Copyright © 2016 Raymond Parag. All rights reserved.
//

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include "htmlstreamparser.h"
#include <curl/curl.h>
#include <string>
#include <fstream>

using namespace std;


int main(int argc, const char * argv[]) {
    
    string line;
    string line2;
    string path = __FILE__;
    size_t found = path.find_last_of("/\\");
    path = path.substr(0, found);
    
    //string path_copy = path; //For OSX
    //path_copy += "/queryterms.txt"; //For OSX
    ifstream myfile("queryterms.txt"); //Instead of path_copy. For Linux
    if(myfile.is_open())
    {
        while(getline(myfile, line))
        {
            
        }
    }
    else
    {
        cout << "No query file found!" << endl;
    }
    
    //path += "/webindex/"; //For OSX
    
    line += ".txt"; //For both
    string path_linux = "webindex/"; // For Linux
    path_linux +=line; // For Linux
    //path += line; //For OSX
    
    ifstream myfile2(path_linux); //For Linux
    
    if(myfile2.is_open())
    {
        while (getline(myfile2, line2)) {
            cout << line2 << endl;
        }
        myfile2.close();
    }
    else{
        cout << "No results" << endl;
    }
        
}