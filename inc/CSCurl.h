/**
 * @file CSCurl.h
 * @brief 
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

#ifndef CSCURL_H_
#define CSCURL_H_

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <sstream>
#include <time.h>

#include <csconnect/ConfigFile.h>

namespace cs {

//! Curl uses REST terminology, PUT, GET, POST, DELETE
enum RequestMethod {REQUEST_METHOD_PUT, REQUEST_METHOD_GET, REQUEST_METHOD_POST,
	REQUEST_METHOD_DELETE, REQUEST_METHOD_HEAD, NOF_REQUEST_METHODS };

class CSCurl {
private:
	//! Curl handle that can be used in parallel
	CURLM *curlMultiHandle;

	//! Default curl handle
	CURL *curlHandle;

	//! Set of entities send with the request such as Content-Type etc.
	struct curl_slist *curlHeaderList;

	//! Request method set
	RequestMethod method;
protected:
	//! Wrapper for callback (C versus C++)
	static size_t Handle(char * data, size_t size, size_t nmemb, void * p);
	//! Callback for get request
	size_t HandleImpl(char * data, size_t size, size_t nmemb);
public:
	void Init();

	void InitHeader();

	void Login(std::string login);

	void SetData(std::string data);

	void Run();

	int RunWithoutCallback();

	//! Set request method
	void SetRequestMethod(RequestMethod method);

	//! Authenticate as consumer
	void Authenticate();

	//! Get result
	std::string GetResult();

private:
	std::string result;

	bool result_arrived;
};

}

#endif /* CSCURL_H_ */
