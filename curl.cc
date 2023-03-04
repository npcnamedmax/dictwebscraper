#define CURL_STATICLIB

#include "/usr/include/x86_64-linux-gnu/curl/curl.h"

#include <cstring>

#include <iostream>

#include <string>

#include <fstream>


using namespace std;

struct response {

  char * buffer;

  size_t size; //size of buffer

};

static size_t callbk(void * content, size_t size, size_t nmem, void * userp)

{

  size_t realsize = size * nmem; //calc real size using size per block * no of block

  response * mem = (response * ) userp; //cast userp to response type

  void * ptr1 = realloc(mem -> buffer, mem -> size + realsize + 1); //return val of realloc is void*, must be casted to char* explicitly in the next line

  char * ptr = (char * ) ptr1; //cast void* to char*

  if (!ptr) {

    /* out of memory! */

    printf("not enough memory (realloc returned NULL)\n");

    return 0;

  }

  mem -> buffer = ptr; //realloc might change the pointer of first char

  memcpy( & (mem -> buffer[mem -> size]), content, realsize); //copy new data next to old data(append)

  mem -> size += realsize; //update total size of buffer

  mem -> buffer[mem -> size] = '\0'; //add null terminator

  return realsize;

}

int main()

{

  curl_global_init(CURL_GLOBAL_DEFAULT); //sets up resources needed for libcurl

  CURLcode stat; //status code for curl

  CURL * handle = curl_easy_init(); //CURL handle is an object that holds all the information necessary to perform a transfer with the libcurl library, such as the URL, the method, the headers, the data to send, etc.

  response data;

  void * ptr = malloc(0);

  data.buffer = (char * ) ptr;

  data.size = 0;

  if (handle) {

    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callbk); //callback function of libcurl is callbk, curlopt_writefunction specifies that callbk is the function to call if there is data received

    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void * ) & data); //specifies that data is where to store msg received

    curl_easy_setopt(handle, CURLOPT_URL, "https://www.merriam-webster.com/dictionary/test"); //finds the definition of test

    //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1L); to display additional information

    stat = curl_easy_perform(handle); //execute

    curl_easy_cleanup(handle);

    if (stat != CURLE_OK) { //if fails to get http/connect etc

      cerr << "CURL error: " << stat << '\n';

    }

  }

  ofstream of ; //write to tmp file

  of.open("/tmp.html", ios::out | ios::trunc); //path to tmp file

  if (of.is_open()) {

    of.write(data.buffer, data.size);

    of.close();

  } else {

    cerr << "Unable to open file tmp.html\n";

  }

  free(data.buffer);

  curl_global_cleanup();

}
