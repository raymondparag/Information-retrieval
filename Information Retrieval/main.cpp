//
//  main.cpp
//  Information Retrieval
//
//  Created by Raymond Parag on 29-01-16.
//  Copyright © 2016 Raymond Parag. All rights reserved.
//

#include <iostream>
#include "htmlstreamparser.h"
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
    curl_easy_setopt(easyhandle, CURLOPT_FOLLOWLOCATION, 1);
    
    results = curl_easy_perform(easyhandle);
    curl_easy_cleanup(easyhandle);
   
    char stringHTML[readBufferHTML.size()]; //Wellicht probeer dit static te maken!
    strcpy(stringHTML, readBufferHTML.c_str());
    char *HTMLpointer = stringHTML;
    
    if(strlen(HTMLpointer) != 0)
    {
        cout << "Successfully obtained HTML!" << endl;
        return HTMLpointer; //Somewhere deallocate pointer at end of function when used --> free(....)
    }
    else
    {
        cout << "Failed obtaining HTML!" << endl;
        return 0;
    }
}

char *GetLinksFromWebPage(char *myhtmlpage, char *myurl)
{
    HTMLSTREAMPARSER *hsp = html_parser_init();
    
    html_parser_set_tag_to_lower(hsp, 1);
    html_parser_set_attr_to_lower(hsp, 1);
    
    char tag[2]; char attr[4]; char val[128];
    html_parser_set_tag_buffer(hsp, tag, sizeof(tag));
    html_parser_set_attr_buffer(hsp, attr, sizeof(attr));
    html_parser_set_val_buffer(hsp, val, sizeof(val)-1);
    
    //myhtmlpage wordt waarschijnlijk doordat stack geleegd wordt gealtered ---> FIX dat
    char stringhtml[readBufferHTML.size()];
    
    for(int i = 0; i < strlen(stringhtml); ++i)
    {
        html_parser_char_parse(hsp, stringhtml[i]);
        if (html_parser_cmp_tag(hsp, "a", 1))
            if (html_parser_cmp_attr(hsp, "href", 4))
                if (html_parser_is_in(hsp, HTML_VALUE_ENDED))
                {
                    val[html_parser_val_length(hsp)] = '\0';
                    printf("%s\n", val);
                }
    }
    html_parser_cleanup(hsp);
    
    return 0;
}


int main(int argc, const char * argv[]) {
    
    curl_global_init( CURL_GLOBAL_ALL );
    
    char url[] = "http://www.liacs.nl/~mlew";
    char *htmlpage = GetWebPage(url);
    GetLinksFromWebPage(htmlpage, url);
    
    cout << " \n";
    return 0;
}



