/**
 * @file CSCurl.cpp
 * @brief Curl-aware file that connects to CommonSense platform
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common 
 * Hybrid Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from 
 * thread pools and TCP/IP components to control architectures and learning algorithms. 
 * This software is published under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless,
 * we personally strongly object against this software being used by the military, in the
 * bio-industry, for animal experimentation, or anything that violates the Universal
 * Declaration of Human Rights.
 *
 * Copyright © 2010 Anne van Rossum <anne@almende.com>
 *
 * @author  Anne C. van Rossum
 * @date    Nov 20, 2011
 * @project Replicator FP7
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */
#include <CSCurl.h>

#include <iostream>

using namespace cs;
using namespace std;

void CSCurl::Init() {
	cout << "Init CSCurl" << endl;
	InitHeader();
}

/**
 * The CommonSense server requires "application/json" as Content-Type. Check tcpdump
 * if this actually happens.
 */
void CSCurl::InitHeader() {
	static const char headerExpect[] = "Expect:";
	curlHeaderList = NULL;
	curlHeaderList = curl_slist_append(curlHeaderList, headerExpect);

	char contentType[1024];
	snprintf(contentType, sizeof(contentType)-1, "Content-Type: %s", "application/json");
	curlHeaderList = curl_slist_append(curlHeaderList, contentType);
}

void CSCurl::Login(std::string login) {
#ifdef DEBUG
	cout << "Login with " << login << endl;
#endif
	curlHandle = curl_easy_init();

	curl_easy_setopt(curlHandle, CURLOPT_URL, login.c_str());
}

void CSCurl::SetData(std::string data) {
#ifdef DEBUG
	cout << "Post data: " << data << endl;
#endif
	curl_easy_setopt(curlHandle, CURLOPT_POSTFIELDS, data.c_str());
}

size_t CSCurl::Handle(char * data, size_t size, size_t nmemb, void * p) {
	return static_cast<CSCurl*>(p)->HandleImpl(data, size, nmemb);
}

size_t CSCurl::HandleImpl(char * data, size_t size, size_t nmemb) {
#ifdef DEBUG
	cout << "Size is " << size << " and data length " << nmemb << endl;
	cout << "Data is " << data << endl;
#endif
	result = string(data);
	result_arrived = true;
	return -1;
}

//! Very ugly implementation
std::string CSCurl::GetResult() {
	while(!result_arrived) {
		sleep(1);
	}
	result_arrived = false;
	return result;
}

void CSCurl::Run() {
	if(!curlMultiHandle) {
		if(curl_global_init(CURL_GLOBAL_NOTHING) != 0) {
			printf("Failed to global init libcurl.\n");
		}

		if((curlMultiHandle = curl_multi_init()) == NULL) {
			printf("Failed to initialize libcurl multi-handle.\n");
		}
	}

	if (method == REQUEST_METHOD_GET) {
		curl_easy_setopt(curlHandle, CURLOPT_NOPROGRESS, 1);
		// Write data not to file but to a function "Handle"
		curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, &CSCurl::Handle);
		curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, this);
		curl_easy_perform(curlHandle);
		curl_easy_cleanup(curlHandle);
	}
	else {
		SetRequestMethod(method);
		RunWithoutCallback();
	}
}

int CSCurl::RunWithoutCallback() {
	int nofRunning = 1;
	int result = 0;

	if(curlHeaderList)
		curl_easy_setopt(curlHandle, CURLOPT_HTTPHEADER, curlHeaderList);

	if(curl_multi_add_handle(curlMultiHandle, curlHandle) != CURLM_OK) {
		printf("Failed to add the easy-handle to the multi-handle.\n");
		result = -1;
	}


	while(result == 0 && nofRunning) {
		CURLMcode code;
		do {
			code = curl_multi_perform(curlMultiHandle, &nofRunning);
			CURLMsg *msg;
			int nofMessages;
			while((msg = curl_multi_info_read(curlMultiHandle, &nofMessages)) != NULL) {
#ifdef DEBUG
				printf("\ncurl_runWithCurlHandle(): msg. handle:%p\n", msg->easy_handle);
#endif
			}
		}
		while(code == CURLM_CALL_MULTI_PERFORM);

		if(nofRunning) {
			fd_set fdread;
			fd_set fdwrite;
			fd_set fdexcep;
			int maxfd = -1;

			FD_ZERO(&fdread);
			FD_ZERO(&fdwrite);
			FD_ZERO(&fdexcep);

			if(curl_multi_fdset(curlMultiHandle, &fdread, &fdwrite, &fdexcep, &maxfd) != CURLM_OK) {
				result = -1;
				continue;
			}

			int nofFds = select(maxfd+1, &fdread, &fdwrite, &fdexcep, NULL);
			if(nofFds < 0) {
				result = -1;
				continue;
			}
		}
	}
	long responseCode;
	curl_easy_getinfo(curlHandle, CURLINFO_RESPONSE_CODE, &responseCode);

	if(curl_multi_remove_handle(curlMultiHandle, curlHandle) != CURLM_OK) {
		result = -1;
	}

	return result;
}

void CSCurl::SetRequestMethod(RequestMethod method) {
	switch(method) {
	case REQUEST_METHOD_PUT:
		curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
		break;
	case REQUEST_METHOD_GET:
		curl_easy_setopt(curlHandle, CURLOPT_HTTPGET, 1);
		break;
	case REQUEST_METHOD_POST:
		curl_easy_setopt(curlHandle, CURLOPT_POST, 1);
		break;
	case REQUEST_METHOD_DELETE:
		curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");
		break;
	case REQUEST_METHOD_HEAD:
		curl_easy_setopt(curlHandle, CURLOPT_CUSTOMREQUEST, "HEAD");
		break;
	default:
	case NOF_REQUEST_METHODS:
		printf("Bad request method: %d\n", method);
		break;
	}
	this->method = method;
}
