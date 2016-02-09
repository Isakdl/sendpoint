//
//  main.c
//  CoordinateClient
//
//  Created by Isak on 09/02/2016.
//  Copyright © 2016 Isak. All rights reserved.
//

#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

void printInvalidCoord(char coord);
void printInstruction();
int parseCoordinate(char * str, long *val);
void sendPointData(char* url, char* data);

int main(int argc, char * argv[]) {
    
    if(argc == 4){

        char *jsonObj = calloc(1024, sizeof(char));
        char *url = calloc(1024, sizeof(char));
        
        long *x = calloc(1, sizeof(int));
        long *y = calloc(1, sizeof(int));
        
        if(parseCoordinate(argv[1], x)){
            printInvalidCoord('X');
            exit(EXIT_FAILURE);
        }
        if(parseCoordinate(argv[2], y)){
            printInvalidCoord('Y');
            exit(EXIT_FAILURE);
        }
        
        url = argv[3];
        
        sprintf(jsonObj, "{\"X\":%lu, \"Y\":%lu}", *x, *y);
        
        sendPointData(url, jsonObj);
        
        free(x);
        free(y);
        free(jsonObj);
        free(url);
    } else {
        printInstruction();
        exit(EXIT_FAILURE);
    }
    
    
     return EXIT_SUCCESS;

}

void printInvalidCoord(char coord){
    fprintf(stderr,"%c not a valid coordinate\n", coord);
    printInstruction();
}

void printInstruction(){
    fprintf(stderr,"usage: sendpoint [x] [y] [url]\n");
}


int parseCoordinate(char * str, long *val){
    
        int base = 10, returnValue = 0;
        char *endptr;
        
        errno = 0;
        *val = strtol(str, &endptr, base);
        
        if ((errno == ERANGE && (*val == LONG_MAX || *val == LONG_MIN))
            || (errno != 0 && *val == 0)) {
            returnValue = 1;
        }
        
        if (endptr == str) {
            returnValue = 1;
        }
        
        if (*endptr != '\0'){
            returnValue = 1;
        }
        
    return returnValue;
}




void sendPointData(char* url, char* data){
    CURL *curl;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "charsets: utf-8");
        
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            fprintf(stderr, "%s\n", curl_easy_strerror(res));
            exit(EXIT_FAILURE);
        }
        
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}

/**

#include <stdio.h>
#include <curl/curl.h>

int main(void)
{
    CURL *curl;
    CURLcode res;
    
    /* In windows, this will init the winsock stuff 
    curl_global_init(CURL_GLOBAL_ALL);
    
    /* get a curl handle
    curl = curl_easy_init();
    if(curl) {
        /* First set the URL that is about to receive our POST. This URL can
         just as well be a https:// URL if that is what should receive the
         data.
        //curl_easy_setopt(curl, CURLOPT_PORT, 8001);
        curl_easy_setopt(curl, CURLOPT_URL, "http://130.239.225.85:8001");
        /* Now specify the POST data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "Hej andra sidan :)");
        
        /* Perform the request, res will get the return code
        res = curl_easy_perform(curl);
        /* Check for errors
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        /* always cleanup
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
*/