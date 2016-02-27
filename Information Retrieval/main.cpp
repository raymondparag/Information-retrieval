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
#include <string>

#define MAXQSIZE 9000000
#define MAXURL 100000
#define MAXPAGESIZE 20000
#define MAXDOWNLOADS 300

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
        //cout << "Successfully obtained HTML!" << endl;
        char *HTMLpointer = (char*)malloc(readBufferHTML.size() + 1);
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
    
    char tag[1]; char attr[4]; char val[1024]; //<----- could create issue if val size too short
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
        //cout << "Success getting weblinks!" << endl;
        char *LINKSpointer = (char*)malloc(total.size() + 1);
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
    
    char tag[3]; char attr[3]; char val[1024]; //<----- could create issue if val size too short
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
        //cout << "Success getting imagelinks!" << endl;
        char *LINKSpointer = (char*)malloc(total.size() + 1);
        strcpy(LINKSpointer, total.c_str());
        return LINKSpointer; //Somewhere deallocate pointer at end of function when used --> free(...)
    }
    else
    {
        cout << "Failed getting imagelinks!" << endl;
        return 0;
    }
}

void AppendLinks(char *p, char *q, char *weblinks) //works
{
        long size = strlen(weblinks) + strlen(q) + 1;
        if(size < MAXQSIZE)
        {
            string total;
            
            for(int i = 0; i < strlen(weblinks); ++i)
            {
                if(weblinks[i] == '\n')
                {
                
                checkagain:
                    if(total.back() == '/') //wellicht ook #
                    {
                        total.pop_back();
                        goto checkagain;
                    }
                    
                    if(strstr(q, total.c_str()) != NULL)
                    {
                        //cout << "ALREADY EXISTS! " << total << endl;
                    }
                   
                    else
                    {
                        strcat(q, total.c_str());
                        strcat(q, "\n");
                    }
                    total.clear();
                }
                else
                {
                    total += *&weblinks[i];
                }
            }
            strcat(q, "\0");
            
            //    strcat(q, weblinks); //enkel dit goed
           //     strcat(q, "\0"); //enkel dit goed
        }
        else
        {
            cout << "Reached end of queue ... exiting" << endl;
            return exit(0);
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
    if(p == NULL || p[0] == '\0')
    {
        strcpy(myurl, "http://127.0.0.1");
        cout << "No URL found in p, exiting..." << endl;
        return 0;
    }
    
    for(int i = 0; i < MAXURL; ++i)
    {
        if(i > strlen(p)) //check end of queue?
        {
            cout << "i already passed p size, exiting..." << endl;
            return 0;
        }
        
        if(p[i] == '\n')
        {
            myurl[i] = '\0';
            return 1;
        }
        else
        {
            myurl[i] = p[i];
        }
    }
    return 0;
}

char *ShiftP(char *p, char *q) //works
{
    for(int i = 0; i < MAXURL; ++i)
    {
        if(i > strlen(p)) //check end of queue?
        {
            cout << "i already passed p size, exiting..." << endl;
            return 0;
        }
        
        if(p[i] == '\n')
        {
            if(i + 1 > strlen(p))
            {
                cout << "ShiftP not available, exiting..." << endl;
                return 0;
            }
            else
            {
                p = p + i + 1;
                return p;
            }
            
        }
    }
    return 0;
}

void parseWeblinkstoFile() //Dit wordt op het laatst 1 keer uitgevoerd!
{
    //CREATE PATH
    string path = __FILE__;
    size_t found = path.find_last_of("/\\");
    path = path.substr(0, found);
    path += "/webindex/";
    
    //TEST STRING
    char test[] = "http://www.liacs.nl/masters-education";
    char *pch = strtok(test, " http:.-/#"); //first exclude htt://
    
    FILE *fp;
    
    while (pch != NULL)
    {
        cout << pch << endl;
        
        string filename = path;
        filename += pch;
        filename += ".txt";
        cout << filename << endl;
        
        fp = fopen(filename.c_str(), "a");
        if(fp != NULL)
        {
            fprintf(fp,"%s\n", test); //test is ineens opgegeten HIER GEBLEVEN!
        }
        
        pch = strtok(NULL, " .-/#");
    }
    fclose(fp);
}

int main(int argc, const char * argv[]) {
    
    //Homework 2
    //char url[128];
    //cout << "Type URL to find links:" << endl;
    //cin >> url;
    
    //char url[] = "http://www.liacs.nl/~mlew";
    //char *htmlpage = GetWebPage(url);
    //char *weblinks = GetLinksFromWebPage(htmlpage, url);
    //char *imagelings = GetImageLinksFromWebPage(htmlpage, url);
    
    //-------------------------------------------------------------
    parseWeblinkstoFile();
    //Homework 3
    //char *url;
    //char url[MAXURL];
    /*char urlspace[MAXURL];
    char *htmlpage, *weblinks;
    int qs;
    long ql;
    
    char *q = (char*)malloc(MAXQSIZE + 1);
    q[0] = '\0';
    char *p = q;
    
    cout << "Type URL to find links:" << endl;
    cin >> urlspace;
    if(strstr(urlspace, "http") != NULL || strstr(urlspace, "www") != NULL)
    {
        cout << "Initial web URL: " << urlspace << endl;
    }
    else
    {
        cout << "Give a valid http link! exiting..." << endl;
        return 0;
    }
    
    strcat(q, urlspace);
    strcat(q, "\n");
    char *url = urlspace;
    
    for(int i = 0; i < MAXDOWNLOADS; ++i)
    {
        qs = QSize(q);
        ql = strlen(q);
        
        cout << "Download #: " << i << " Weblinks: " << qs << " Queue Size: " << ql << endl;
        if(GetNextURL(p, q, url) == 0)
        {
            return 0;
        }
        p = ShiftP(p, q);
        cout << " DWNLD URL: " << url << endl;
        
        if((strstr(url,"leidenuniv.nl") != NULL || (strstr(url,"liacs.nl")) != NULL || (strstr(url, "universiteitleiden.nl")) != NULL || (strstr(url, "leiden.edu")) != NULL || (strstr(url, "mastersinleiden.nl")) != NULL))
        {
            htmlpage = GetWebPage(url);
            if(htmlpage==NULL)
            {
                cout << "The downloaded webpage was NULL, skipping..." << endl;
                //return 0; //commented for skip dead links!
            }
            else
            {
                weblinks = GetLinksFromWebPage(htmlpage, url);
                if(weblinks != NULL)
                {
                    AppendLinks(p, q, weblinks);
                }
                else
                {
                    cout << "No weblinks found for the given htmlpage, skipping..." << endl;
                }
                free(weblinks);
            }
            free(htmlpage);
        }
        else
        {
            cout << "Not allowed in domains: " << url << " skipping..." << endl;
            //return 0; //commented for skip not valid domains!
        }
    }
    
    free(q);*/
    cout << " \n";
    return 0;
}



