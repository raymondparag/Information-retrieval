//
//  main.cpp
//  Information Retrieval
//
//  Created by Raymond Parag on 29-01-16.
//  Copyright © 2016 Raymond Parag. All rights reserved.
//

#include <iostream>
//#include "htmlstreamparser.h"
#include <curl/curl.h>
using namespace std;

static string readBufferHTML;

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    readBufferHTML.append((char*)buffer, realsize);
    return realsize;
}

char *GetWebPage(char *myurl)
{
    CURL *easyhandle = curl_easy_init();
    CURLcode results;
    readBufferHTML.clear();
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, myurl);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBufferHTML);
    
    results = curl_easy_perform(easyhandle);
    curl_easy_cleanup(easyhandle);
   
    char stringHTML[readBufferHTML.size()];
    strcpy(stringHTML, readBufferHTML.c_str());
    char *HTMLpointer = stringHTML;
    
    if(strlen(HTMLpointer) != 0)
    {
        cout << "Successfully obtained HTML!";
        return HTMLpointer; //Somewhere deallocate pointer at end of function when used --> free(....)
    }
    else
    {
        cout << "Failed obtaining HTML!";
        return 0;
    }
}

char *GetLinksFromWebPage(char *myhtmlpage, char *myurl)
{
    //hiermee aan de slag
    return 0;
}


int main(int argc, const char * argv[]) {
    
    curl_global_init( CURL_GLOBAL_ALL );
    
    char url[] = "http://tweakers.net/";
    //char *test = GetWebPage(url);
    GetWebPage(url);
    
    cout << " \n";
    return 0;
}



