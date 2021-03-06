//
//  main.cpp
//  Information Retrieval
//
//  Created by Raymond Parag.
//  Copyright © 2016 Raymond Parag. All rights reserved.
//

#include <sys/stat.h>
#include <sys/types.h>
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
#define MAXDOWNLOADS 2000

using namespace std;

/*!
    @brief Callback function for getting HTML data with CURL
 */
static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    ((string*)userp)->append((char*)buffer, realsize);
    return realsize;
}

/*!
    @brief Function for getting HTML data from a given url
    @return Returns a pointer to the HTML data, or 0 if no
    HTML data is found
 */
char *GetWebPage(char *myurl)
{
    CURL *easyhandle = curl_easy_init();
    CURLcode results;
    string readBufferHTML;
    
    curl_easy_setopt(easyhandle, CURLOPT_URL, myurl);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBufferHTML);
    curl_easy_setopt(easyhandle, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(easyhandle, CURLOPT_TIMEOUT, 10);
    
    results = curl_easy_perform(easyhandle);
    curl_easy_cleanup(easyhandle);
    
    if(results == 0)
    {
        char *HTMLpointer = (char*)malloc(readBufferHTML.size() + 1);
        strcpy(HTMLpointer,readBufferHTML.c_str());
        return HTMLpointer;
    }
    else
    {
        cout << "Failed obtaining HTML!" << endl;
        return 0;
    }
}

/*!
    @brief Function that extracts weblinks from the given HTML data
    @return Returns a pointer to the weblinks, or 0 if no weblinks 
    are found
 */
char *GetLinksFromWebPage(char *myhtmlpage, char *myurl)
{
    HTMLSTREAMPARSER *hsp = html_parser_init();
    
    html_parser_set_tag_to_lower(hsp, 1);
    html_parser_set_attr_to_lower(hsp, 1);
    
    char tag[1]; char attr[4]; char val[MAXURL]; //
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
                       			
                        char end_char = myurl[strlen(myurl)-1];
                        if(end_char == '/')
                        {
                            myurl[strlen(myurl)-1] = '\0';
                        }
                        
                        strcpy(val, myurl);
                        strcat(val, temp); //strcat is kinda "append"
                    }
                    if(val[0] == '/' || val[0] == 'w' || val[0] == 'h')
                    {
                        strcat(val, "\n");
                    	total.append(val);
                    }
                }
    }
    html_parser_cleanup(hsp);
    
    if(total.length() != 0)
    {
        char *LINKSpointer = (char*)malloc(total.size() + 1);
        strcpy(LINKSpointer, total.c_str());
        return LINKSpointer;
    }
    else
    {
        cout << "Failed getting weblinks!" << endl;
        return 0;
    }
}

/*!
    @brief Function that extracts imagelinks from the given HTML data
    @return Returns a pointer to the imagelinks, or 0 if no imagelinks
    are found
 */
char *GetImageLinksFromWebPage(char *myhtmlpage, char *myurl)
{
    HTMLSTREAMPARSER *hsp = html_parser_init();
    
    html_parser_set_tag_to_lower(hsp, 1);
    html_parser_set_attr_to_lower(hsp, 1);
    
    char tag[3]; char attr[3]; char val[MAXURL]; //
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
                        
                        char end_char = myurl[strlen(myurl)-1];
                        if(end_char == '/')
                        {
                            myurl[strlen(myurl)-1] = '\0';
                        }
                        
                        strcpy(val, myurl);
                        strcat(val, temp); //strcat is kinda "append"
                    }
                    if(val[0] == '/' || val[0] == 'w' || val[0] == 'h')
                    {
                        strcat(val, "\n");
                        total.append(val);
                    }
                }
    }
    html_parser_cleanup(hsp);
    
    if(total.length() != 0)
    {
        char *LINKSpointer = (char*)malloc(total.size() + 1);
        strcpy(LINKSpointer, total.c_str());
        return LINKSpointer;
    }
    else
    {
        cout << "Failed getting imagelinks!" << endl;
        return 0;
    }
}

/*!
    @brief Function that appends the weblinks to a queue and
    parses each url in the queue into words and saves it into
    files. The function checks whether a url is already in the queue
    before adding it to it. The function exits whenever the size of the
    weblinks is too big for the queue size.
 */
void AppendLinks(char *p, char *q, char *weblinks)
{
        long size = strlen(weblinks) + strlen(q) + 1;
        if(size < MAXQSIZE)
        {
            mkdir("webindex", 0700);
            
            /* Uncomment lines for OSX on OSX. Currently Linux is used. */
            
            //string path = __FILE__; //OSX
            //size_t found = path.find_last_of("/\\"); //OSX
            //path = path.substr(0, found); //OSX
            //path += "/webindex/"; //OSX
            string path = "webindex/"; //Linux
            
            FILE *fp;
            
            string total;
            
            for(int i = 0; i < strlen(weblinks); ++i) //iterate through weblinks
            {
                if(weblinks[i] == '\n') //if \n is found
                {                    
                    if(strstr(q, total.c_str()) != NULL) //check for duplicate
                    {
                        //cout << "ALREADY EXISTS! " << total << endl;
                    }
                   
                    else //add to queue and parse to file
                    {
                        strcat(q, total.c_str());
                        strcat(q, "\n");
                        
                        string stringcopy = total;
                        stringcopy += "\n";
                        
                        if(total.find_first_of("/") == 5 || total.find_first_of("/") == 6) //remove http or https of url for split
                        {
                            if(total.find_first_of("/") == 5)
                            {
                                total.erase(0,7);
                            }
                            else
                            {
                                total.erase(0,8);
                            }
                        }
                        
                        char temp_copy[total.size() + 1];
                        strcpy(temp_copy, total.c_str());
                        
                        char *split = strtok(temp_copy, " .,-/#&=?_%!;()@^~*+$<>");
                      
                        while(split != NULL)
                        {
                            string filename = path;
                            filename += split;
                            filename += ".txt";
                            
                            fp = fopen(filename.c_str(), "a");
                            if(fp!=NULL)
                            {
                                fprintf(fp,"%s\n", stringcopy.c_str());
                                fclose(fp);
                            }
                            split = strtok(NULL, " .,-/#&=?_%!;()@^~*+$<>");
                        }
                        
                    }
                    total.clear();
                }
                else
                {
                    total += weblinks[i]; //voorheen *& ervoor
                }
            }
            strcat(q, "\0");
        }
        else
        {
            cout << "Reached end of queue ... exiting" << endl;
            return exit(0);
        }
}

/*!
    @brief Function that returns the size of the queue
 */
int QSize(char *q)
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

/*!
    @brief Function that puts the next url from p into myurl. Exits if
    there are no more urls in p or p is empty
 */
int GetNextURL(char *p, char *q, char *myurl)
{
    if(p == NULL || p[0] == '\0')
    {
        strcpy(myurl, "http://127.0.0.1");
        cout << "No URL found in p, exiting..." << endl;
        return 0;
    }
    
    for(int i = 0; i < MAXURL; ++i)
    {
        if(i > strlen(p)) //check end of queue
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

/*!
    @brief Function that points the position of p to the next url. Exits if 
    there is nothing to point to or i gets bigger than the length of p
 */
char *ShiftP(char *p, char *q)
{
    for(int i = 0; i < MAXURL; ++i)
    {
        if(i > strlen(p)) //check end of queue
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

/*!
    @brief main function that uses the above functions to run the webspider.
    Links that are dead or are not in the domain are simply skipped. Function ends
    when MAXDOWNLOADS is satisfied.
 */
int main(int argc, const char * argv[]) {
    
    //Homework 2
    //char url[128];
    //cout << "Type URL to find links:" << endl;
    //cin >> url;
    
    //char *htmlpage = GetWebPage(url);
    //char *weblinks = GetLinksFromWebPage(htmlpage, url);
    //char *imagelings = GetImageLinksFromWebPage(htmlpage, url);
    
    //-------------------------------------------------------------
    
    //Homework 3
    char urlspace[MAXURL];
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
        }
    }
    
    free(q);
    cout << " \n";
    return 0;
}



