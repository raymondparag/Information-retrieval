//
//  main.cpp
//  Information Retrieval
//
//  Created by Raymond Parag on 29-01-16.
//  Copyright © 2016 Raymond Parag. All rights reserved.
//

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include "htmlstreamparser.h"
#include <curl/curl.h>
using namespace std;

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    ((string*)userp)->append((char*)buffer, realsize);
    return realsize;
}

char *GetWebPage(char *myurl)
{
    CURL *easyhandle = curl_easy_init();
    CURLcode results;
    string readBufferHTML;
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, myurl);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBufferHTML);
    curl_easy_setopt(easyhandle, CURLOPT_FOLLOWLOCATION, 1);
    
    results = curl_easy_perform(easyhandle);
    curl_easy_cleanup(easyhandle);
   
    char *HTMLpointer = (char*)malloc(readBufferHTML.size());
    strcpy(HTMLpointer,readBufferHTML.c_str());
    
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
    
    string total;

    for(int i = 0; i < strlen(myhtmlpage); ++i)
    {
        html_parser_char_parse(hsp, myhtmlpage[i]);
        if (html_parser_cmp_tag(hsp, "a", 1))
            if (html_parser_cmp_attr(hsp, "href", 4))
                if (html_parser_is_in(hsp, HTML_VALUE_ENDED))
                {
                    val[html_parser_val_length(hsp)] = '\0';
                    if(val[0] == '/')
                    {
                        char temp[strlen(val)];
                        strcpy(temp, val); //strcpy to assign char arrays!
                        strcpy(val, myurl);
                        strcat(val, temp); //strcat is kinda "append"
                    }
                    strcat(val, "\n");
                    total.append(val);
                    cout << val;
                }
    }
    html_parser_cleanup(hsp);
    free(myhtmlpage);
    
    char *LINKSpointer = (char*)malloc(total.size());
    strcpy(LINKSpointer, total.c_str());
    
    if(strlen(LINKSpointer) != 0)
    {
        return LINKSpointer;
    }
    else
    {
        return 0;
    }
}

char *GetImageLinksFromWebPage(char *myhtmlpage, char *myurl)
{
    //Done with GetLinksFromWebPage. Probably at if value[0] == any character? check --> http://www.liacs.nl
    return 0;
}


int main(int argc, const char * argv[]) {
    
    curl_global_init( CURL_GLOBAL_ALL );
    
    char url[128];
    cout << "Type URL to find links:" << endl;
    cin >> url;
    
    //char url[] = "http://www.liacs.nl/~mlew";
    char *htmlpage = GetWebPage(url);
    GetLinksFromWebPage(htmlpage, url);
    
    cout << " \n";
    return 0;
}



