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
#include <vector>
#include <algorithm>

using namespace std;

vector<string> total;

void printURL(string line, string line2)
{
    //cout << line << endl;
    line += ".txt"; //For both
    //cout << line << endl;
    string path_linux = "webindex/"; // For Linux
    path_linux +=line; // For Linux
    //path += line; //For OSX
    
    //cout << path_linux.c_str() <<endl;
    ifstream myfile2(path_linux.c_str()); //For Linux
    
    if(myfile2.is_open())
    {
        while (getline(myfile2, line2))
        {
            cout << line2 << endl;
            /*if(find(total.begin(), total.end(), line2) != total.end())
            {
                //cout << "LOBBO" << endl;
            }
            else
            {
                cout << line2 << endl;
                if(line2 != "\0")
                {
                   total.push_back(line2);
                }
            }*/
        }
        myfile2.close();
        line.clear();
        line2.clear();
    }
    else{
        //cout << "No results" << endl;
    }
}  

int main(int argc, const char * argv[]) {
    
    string line;
    string line2;
    total.push_back("!");
    
    //string path = __FILE__;
    //size_t found = path.find_last_of("/\\");
    //path = path.substr(0, found);
    
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
    
    char temp[line.size()+1];
    strcpy(temp, line.c_str());
    char *split = strtok(temp, " ");

    while(split!=NULL)
    {
        string temp2 = split;
        printURL(temp2, line2);
        split = strtok(NULL, " ");
    }    
}
