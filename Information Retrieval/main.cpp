//
//  main.cpp
//  Information Retrieval
//
//  Created by Raymond Parag.
//  Copyright © 2016 Raymond Parag. All rights reserved.
//

#include <iostream>
#include <memory>
#include <stdlib.h>
#include <stdio.h>
#include "htmlstreamparser.h"
#include <curl/curl.h>

#define MAXQSIZE 9000000
#define MAXURL 100000
#define MAXPAGESIZE 20000
#define MAXDOWNLOADS 30

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
    
    if(results == 0)
    {
        cout << "Successfully obtained HTML!" << endl;
        char *HTMLpointer = (char*)malloc(readBufferHTML.size());
        strcpy(HTMLpointer,readBufferHTML.c_str());
        return HTMLpointer; //Somewhere deallocate pointer at end of function when used --> free(...)
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
    
    char tag[1]; char attr[4]; char val[512]; //<----- could create issue if val size too short
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
                    else if(val[0] != 'h') //extra check for anything else than just /
                    {
                        char temp[strlen(val)];
                        strcpy(temp, val);
                        
                        char temp2[strlen(myurl)];
                        strcpy(temp2, myurl);
                        strcat(temp2, "/");
                        
                        strcpy(val, temp2);
                        strcat(val, temp);
                    }
                    strcat(val, "\n");
                    total.append(val);
                    //cout << val;
                }
    }
    html_parser_cleanup(hsp);
    //free(myhtmlpage);
    
    if(total.length() != 0)
    {
        cout << "Success getting weblinks!" << endl;
        char *LINKSpointer = (char*)malloc(total.size());
        strcpy(LINKSpointer, total.c_str());
        return LINKSpointer; //Somewhere deallocate pointer at end of function when used --> free(...)
    }
    else
    {
        cout << "Failed getting weblinks!" << endl;
        return 0;
    }
}

char *GetImageLinksFromWebPage(char *myhtmlpage, char *myurl)
{
    HTMLSTREAMPARSER *hsp = html_parser_init();
    
    html_parser_set_tag_to_lower(hsp, 1);
    html_parser_set_attr_to_lower(hsp, 1);
    
    char tag[3]; char attr[3]; char val[512]; //<----- could create issue if val size too short
    html_parser_set_tag_buffer(hsp, tag, sizeof(tag));
    html_parser_set_attr_buffer(hsp, attr, sizeof(attr));
    html_parser_set_val_buffer(hsp, val, sizeof(val)-1);
    
    string total;
    
    for(int i = 0; i < strlen(myhtmlpage); ++i)
    {
        html_parser_char_parse(hsp, myhtmlpage[i]);
        if (html_parser_cmp_tag(hsp, "img", 3))
            if (html_parser_cmp_attr(hsp, "src", 3))
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
                    else if(val[0] != 'h') //extra check for anything else than just /
                    {
                        char temp[strlen(val)];
                        strcpy(temp, val);
                        
                        char temp2[strlen(myurl)];
                        strcpy(temp2, myurl);
                        strcat(temp2, "/");
                        
                        strcpy(val, temp2);
                        strcat(val, temp);
                    }
                    strcat(val, "\n");
                    total.append(val);
                    //cout << val;
                }
    }
    html_parser_cleanup(hsp);
    //free(myhtmlpage);
    
    if(total.length() != 0)
    {
        cout << "Success getting imagelinks!" << endl;
        char *LINKSpointer = (char*)malloc(total.size());
        strcpy(LINKSpointer, total.c_str());
        return LINKSpointer; //Somewhere deallocate pointer at end of function when used --> free(...)
    }
    else
    {
        cout << "Failed getting imagelinks!" << endl;
        return 0;
    }
}

int QSize(char *q) //works
{
    int total = 0;
    for(int i = 0; i < MAXQSIZE; ++i)
    {
        if(q[i] == '\n')
        {
            total++;
        }
        if(q[i] == '\0')
        {
            return total;
        }
    }
    return total;
}

int GetNextURL(char *p, char *q, char *myurl) //works
{
    for(int i = 0; i < MAXURL; ++i)
    {
        if(p[i] == '\n')
        {
            //myurl[i] = p[i];
            myurl[i] = '\0'; //previous it was myurl[i+1] = '\0';
            return 1;
        }
        else
        {
            myurl[i] = p[i];
            //cout << p[i] << endl;
        }
    }
    //strcpy(myurl, "http://127.0.0.1"); //DOESNT MAKE SENSE
    return 0;
}

char *ShiftP(char *p, char *q) //works
{
    for(int i = 0; i < MAXURL; ++i)
    {
        if(p[i] == '\n')
        {
            p = p + i + 1;
            return p;
        }
    }
    return 0;
}

int main(int argc, const char * argv[]) {
    
    curl_global_init( CURL_GLOBAL_ALL );
    
    //char url[128];
    //cout << "Type URL to find links:" << endl;
    //cin >> url;
    
    //char url[] = "http://www.liacs.nl/~mlew";
    //char *htmlpage = GetWebPage(url);
    //char *weblinks = GetLinksFromWebPage(htmlpage, url);
    //char *imagelings = GetImageLinksFromWebPage(htmlpage, url);
    
    //-------------------------------------------------------------
    
    //char *url;
    //char url[MAXURL];
    char urlspace[MAXURL];
    char *htmlpage, *weblinks;
    int qs, ql;
    
    char *q = (char*)malloc(MAXQSIZE);
    q[0] = '\0';
    char *p = q;
    
    cout << "Type URL to find links:" << endl;
    cin >> urlspace;
    
    cout << "Initial web URL: " << urlspace << endl;
    
    strcat(q, urlspace);
    strcat(q, "\n");
    char *url = urlspace;
    
    for(int i = 0; i < MAXDOWNLOADS; ++i)
    {
        qs = QSize(q);
        ql = strlen(q);
        
        cout << "Download #: " << i << " Weblinks: " << qs << " Queue Size: " << ql << endl;
        GetNextURL(p, q, url);
        p = ShiftP(p, q);
        
        if((strstr(url,"leidenuniv.nl") != NULL || (strstr(url,"liacs.nl")) != NULL))
        {
            htmlpage = GetWebPage(url);
            if(htmlpage==NULL)
            {
                cout << "The downloaded webpage was NULL" << endl;
                return 0;
            }
            else
            {
                weblinks = GetLinksFromWebPage(htmlpage, url);
                strcat(q, weblinks);
                strcat(q, "\0");
            }
        }
        else
        {
            cout << "Not allowed in domains: " << url << endl;
        }
    }
    
    //free(htmlpage);
    //free(weblinks);
    free(q);
    cout << " \n";
    return 0;
}



