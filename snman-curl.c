#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

#include "snman-curl.h"

int curl_cmd_host(char *hostname, char *get_path)
{
	CURL *curl;
	CURLcode res;
 
	/* In windows, this will init the winsock stuff */ 
	curl_global_init(CURL_GLOBAL_ALL);
 
	/* get a curl handle */ 
	curl = curl_easy_init();
	if(curl) {
		/*
		 * First set the URL that is about to receive our POST.
		 * This URL can just as well be a https:// URL if that is what
		 * should receive the data.
		 */ 
		char full_url[255];
		strcpy(full_url, hostname);
		strcat(full_url, get_path);
		curl_easy_setopt(curl, CURLOPT_URL, full_url);
		/* Now specify to GET */ 
		curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
		/* Now specify to use port 8096 */ 
		curl_easy_setopt(curl, CURLOPT_PORT, 8096L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 2L);
 
		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
 
		/* always cleanup */ 
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();

	return 0;
}
