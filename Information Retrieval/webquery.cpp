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

/*!
    @brief Function that reads the urls from a file given the query
 */
void printURL(string line, string line2)
{
    line += ".txt"; //Both
    string path_linux = "webindex/"; //Linux
    path_linux +=line; //Linux
    //path += line; //OSX
    
    ifstream myfile2(path_linux.c_str()); //Linux
    
    if(myfile2.is_open())
    {
        while (getline(myfile2, line2))
        {
            cout << line2 << endl;
        }
        myfile2.close();
        line.clear();
        line2.clear();
    }
    else{
        //cout << "No results" << endl;
    }
}  

/*!
    @brief Main function which reads the query in queryterms.txt and splits
    the queries if more queries are given. Then it gives the strings to the 
    printURL function which reads the urls from the files in webindex
 */
int main(int argc, const char * argv[]) {
    
    string line;
    string line2;
    total.push_back("!");
    
    /* Uncomment lines for OSX on OSX. Currently Linux is used. */
    
    //string path = __FILE__; //OSX
    //size_t found = path.find_last_of("/\\"); //OSX
    //path = path.substr(0, found); //OSX
    
    //string path_copy = path; //OSX
    //path_copy += "/queryterms.txt"; //OSX
    ifstream myfile("queryterms.txt"); //Instead of path_copy. Linux
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
    
    //path += "/webindex/"; //OSX
    
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
