/**
 * @file CSOAuthConnector.cpp
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

#include <CSOAuthConnector.h>
#include <ConfigFile.h>
#include <CSCurl.h>

extern "C" {
#include "oauth.h"
}

#include <string>
#include <sstream>
#include <time.h>

using namespace cs;
using namespace std;

CSOAuthConnector::CSOAuthConnector():  CSConnect() {

}

CSOAuthConnector::~CSOAuthConnector() {
}

void CSOAuthConnector::Init() {
	cout << "Initialize the OAuth connector" << endl;
}

/**
 * Login with oauth. For now we assume that there are a dedicated series of strings stored
 * in the CSLogin struct. As you might now, oauth is divided into a request "step", and an
 * access "step". In the request step (oauth0) we expect only "oauth" to be in the CSLogin
 * struct. In the access step (oauth1) we expect "step", "verifier", "secret", and "token".
 * The secret and token are the (temporary) ones retrieved before in the request step (but
 * will need to be stored in user-space and will not be maintained by this lib). This is
 * because multiple people need to be able to login using this library and each of them can
 * be at a different step.
 */
void CSOAuthConnector::Login(CSLogin & id) {
//	cout << "Logging in" << endl;
//	curl->SetRequestMethod(REQUEST_METHOD_POST);
	string token_key, token_secret;
	string step = id.value.back();
	if (step == "oauth0") {
		if (id.value.size() != 1) {
			cerr << "Wrong amount of authorisation fields (1)" << endl;
			return;
		}
		id.value.clear();
		if (OAuthTempCredentials(token_key, token_secret)) {
			id.value.push_back(token_key);
			id.value.push_back(token_secret);
		} else {
			cerr << "Login failed!" << endl;
		}
	} else if (step == "oauth1") {
		if (id.value.size() != 4) {
			cerr << "Wrong amount of authorisation fields (4)" << endl;
			return;
		}
		token_secret = "";
//		cout << "step: " << step << endl;
		id.value.pop_back();
		string verifier = id.value.back();
//		cout << "verifier: " << verifier << endl;
		id.value.pop_back();
		token_secret = id.value.back();
//		cout << "secret: " << token_secret << endl;
		id.value.pop_back();
		token_key = id.value.back();
//		cout << "token: " << token_key << endl;
		id.value.clear();
//		cout << "Call OAuthCredentials(" << verifier << "," << token_key << "," << token_secret << ")" << endl;
		if (OAuthCredentials(verifier, token_key, token_secret)) {
			id.value.push_back(token_key);
			id.value.push_back(token_secret);
			cout << "Key and secret " << token_key << " and " << token_secret << endl;
		} else {
			cout << "Login failed (second step)!" << endl;
		}
	} else {
		cerr << "Login failed, unknown step" << endl;
	}
}


//! We can add a sensor to the platform, sensor_id needs to be returned
void CSOAuthConnector::AddSensor(CSLogin & id, std::string name, std::string display_name, std::string data_type,
		std::string device_type, int & sensor_id) {
	string token_key, token_secret;
	ParseOAuthTokens(id, token_key, token_secret);
	string result, data;

	data = "{\"sensor\":{\"name\":\"" + string(name) + "\",\"display_name\":\"" + string(display_name) +
			"\",\"data_type\":\"" + string(data_type) + "\",\"device_type\":\"" + string(device_type) + "\"}}";
	result = OAuth("http://api.sense-os.nl/sensors.json", data, "POST", token_key, token_secret); //create sensor
	cout << result << endl;
}

//! Add data items for a specific sensor
void CSOAuthConnector::AddData(CSLogin & id, int sensor_id, std::vector<CSData> & datav) {
	string token_key, token_secret;
	ParseOAuthTokens(id, token_key, token_secret);
	string result;
	ostringstream oss;
	oss.clear(); oss.str("");
	oss << "http://api.sense-os.nl/sensors/" << sensor_id << "/data.json";
	ostringstream datas;
	datas.clear(); datas.str("");
	datas << "{\"data\":[";
	for (int i = 0; i < datav.size(); ++i) {
		datas << "{";
		for (int j = 0; j < datav[i].fields.size(); ++j) {
			CSDataField *csf = datav[i].fields[j];
			datas << "\"" << csf->label << "\":\"" << csf->value << "\"";
			if (j < datav[i].fields.size() - 1) datas << ",";
		}
		datas << "}";
		if (i < datav.size() - 1) datas << ",";
	}
	datas << "]}";
	result = OAuth(oss.str(), datas.str(), "POST", token_key, token_secret);
}


//! Add data item for a specific sensor
void CSOAuthConnector::AddData(CSLogin & id, int sensor_id, CSData dataf, int unixTimeStamp) {
	if (unixTimeStamp==0){
	  unixTimeStamp = time(NULL);
	  cout << "time is: "<<unixTimeStamp << endl;
	}
	string token_key, token_secret;
	ParseOAuthTokens(id, token_key, token_secret);
	string result;
	ostringstream oss;
	oss.clear(); oss.str("");
	oss << "http://api.sense-os.nl/sensors/" << sensor_id << "/data.json";
	ostringstream datas;
	datas.clear(); datas.str("");
	datas << "{\"data\":[";
	datas << "{";
	for (int j = 0; j < dataf.fields.size(); ++j) {
		CSDataField *csf = dataf.fields[j];
//		cout << "Label: " << csf->label << endl;
		datas << "\"" << csf->label << "\":\"" << csf->value << "\"";
		if (j < dataf.fields.size() - 1) datas << ",";
	}
	datas << ",";
	datas << "\"date\":\"" << unixTimeStamp << "\"";
	datas << "}";
	datas << "]}";
	result = OAuth(oss.str(), datas.str(), "POST", token_key, token_secret);
}


void CSOAuthConnector::AddData(CSLogin & id, int sensor_id, std::string jsondata) {
	string token_key, token_secret;
	ParseOAuthTokens(id, token_key, token_secret);
	string result;
	ostringstream oss;
	oss.clear(); oss.str("");
	oss << "http://api.sense-os.nl/sensors/" << sensor_id << "/data.json";
	result = OAuth(oss.str(), jsondata, "POST", token_key, token_secret);
}


//! Get a list of sensors in the form of a JSON string
string CSOAuthConnector::GetSensors(CSLogin & id) {
	string token_key, token_secret;
	ParseOAuthTokens(id, token_key, token_secret);
	string result, data;
	data.clear();
	result = OAuth("http://api.sense-os.nl/sensors.json", data, "GET", token_key, token_secret); //gets all sensors
	return result;
}

//! Get data of a specific sensor
string CSOAuthConnector::GetData(CSLogin & id, int sensor_id, int unixTimeStamp) {
	if (unixTimeStamp==0){
	  unixTimeStamp = time(NULL);
	  cout << "time is: "<<unixTimeStamp << endl;
	}
	string token_key, token_secret;
	ParseOAuthTokens(id, token_key, token_secret);
	string result, data;
	ostringstream oss;
	oss.clear(); oss.str("");
	oss << "http://api.sense-os.nl/sensors/" << sensor_id << "/data.json?last=1&end_date="<<unixTimeStamp;

	// get last data item
	result = OAuth(oss.str(), data, "GET", token_key, token_secret);
	return result;
}


/**
 * Parse oauth tokens (key and secret). They will be removed from the login id.
 */
void CSOAuthConnector::ParseOAuthTokens(CSLogin &id, std::string &token_key, std::string &token_secret) {
	token_secret = id.value.back();
	id.value.pop_back();
	token_key = id.value.back();
	id.value.clear();
}

/**
 * The multi-user solution of access to CommonSense. The system-wide ~/.rur/oauth file
 * is meant for everything that makes use of the CSConnect library. The function returns
 * token_key and token_secret if it is successful. The boolean returns false if it is
 * not successful. From the CommonSense oauth document:
 * <blockquote>
 *   3. Once those three elements have been set in the Consumer, the user can start the OAuth
 *   process by requesting a temporary token (request token) to the following URL using the
 *   consumer application: http://api.sense-os.nl/oauth/request_token with the following
 *   required parameters:
 *     oauth_callback
 *     oauth_consumer_key -> used to verify the identity of the consumer application
 *     oauth_nonce -> unique and random string
 *     oauth_signature
 *     oauth_signature_method -> must be HMAC-SHA1
 *     oauth_timestamp -> in seconds
 *     oauth_version -> must be 1.0
 *   These OAuth parameters must be sorted in alphabetical order.
 *   The signature must be calculated using the consumer secret and a base string.
 *   The base string is a reproducible concatenation of several of the HTTP request
 *   elements (HTTP method, the URL and the parameters -normalized-, each url-encoded
 *   and concatenated with &) into a single string (string construction: section 3.4.1). The
 *   oauth_signature must be excluded from the base string.
 *   oauth_signature = base64_encode(hash_hmac(sha1, baseString, key));
 *   4. The Provider verifies the Consumer and returns the temporary credential (token key and
 *   secret).
 * </blockquote>
 * Again, the arguments token_key, token_secret are returned (and not given!).
 */
bool CSOAuthConnector::OAuthTempCredentials(string &token_key, string &token_secret) {
	char *home = getenv("HOME");
	bool onlyprint = false;
	string filename = string(home) + "/.rur/oauth";
	ConfigFile config( filename );
	string requestTokenURL, consumerKey, consumerSecret, callback;
	config.readInto( requestTokenURL, "RequestTokenURL" );
	config.readInto( consumerKey, "ConsumerKey" );
	config.readInto( consumerSecret, "ConsumerSecret" );
	config.readInto( callback, "RegisteredOAuthCallbackURL" );

	requestTokenURL = requestTokenURL + '?'+ "oauth_callback=" + callback; // + '&' + "oauth_"
	const char *request_token_uri = requestTokenURL.c_str();
	const char *req_c_key         = consumerKey.c_str();
	const char *req_c_secret      = consumerSecret.c_str();

	char *req_url = NULL;
	char *reply = NULL;
	bool success = false;

	string cb = "oauth_callback=" + callback;
	int cparg = 1;
	char ** parg = new char*[cparg];
	parg[0] = new char[cb.length() + 1];
	strcpy(parg[0], cb.c_str());
	parg[0][cb.length()] = '\0';

	req_url = oauth_sign_url2(request_token_uri, NULL, OA_HMAC, NULL, req_c_key, req_c_secret, NULL, NULL);

	printf("request URL:%s\n\n", req_url);
	if (onlyprint) {
		cerr << "HTTP request not send because of only-print mode" << endl;
	} else {
		reply = oauth_http_get(req_url,NULL);
	}

	char *res_t_key    = NULL; //< replied key
	char *res_t_secret = NULL; //< replied secret

	if (!reply || onlyprint)
		printf("HTTP request for an oauth request-token failed.\n");
	else {
		int rc;
		char **rv = NULL;

		printf("HTTP-reply: %s\n", reply);
		rc = oauth_split_url_parameters(reply, &rv);
		// do not qsort, because of optional(!) param callback_confirmed: position of params is unknown
		for (int i = 0; i < rc; ++i) {
			if (!strncmp(rv[i],"oauth_token_secret=",18) ) {
				res_t_secret=strdup(&(rv[i][19]));
			} else if (!strncmp(rv[i],"oauth_token=",11)) {
				res_t_key=strdup(&(rv[i][12]));
			}
		}
		printf("key: '%s'\nsecret: '%s'\n", res_t_key, res_t_secret);
		if (res_t_key != NULL && res_t_secret != NULL) {
			token_key = string(res_t_key);
			token_secret = string(res_t_secret);
			if (!token_key.empty() && (!token_secret.empty()))
				success = true;
		}
		if(rv) free(rv);
	}

	if(req_url) free(req_url);
	if(reply) free(reply);
	if(res_t_key) free(res_t_key);
	if(res_t_secret) free(res_t_secret);
	return success;
}

/**
 * The second step in the authorisation procedure.
 * <blockquote>
 *   9. The Consumer requests the access credential (access token) using the following data:
 *   http://api.sense-os.nl/oauth/access_token with the following required parameters:
 *     oauth_consumer_key
 *     oauth_nonce
 *     oauth_signature
 *     oauth_signature_method -> HMAC-SHA1
 *     oauth_timestamp
 *     oauth_token -> request token
 *     oauth_verifier -> must be the same to that one retrieved in the authorization step
 *     oauth_version -> 1.0
 *   At this point, the signature is calculated using the consumer key and token key as a
 *   secret and the base string.
 *   10. The Provider checks the request and returns the access credential (token key and
 *   secret).
 * </blockquote>
 * The token_key should have been retrieved via OAuthTempCredentials before. It will be returned
 * again.
 */
bool CSOAuthConnector::OAuthCredentials(string verifier, string &token_key, string &token_secret) {
	char *home = getenv("HOME");
	bool onlyprint = false;
	string filename = string(home) + "/.rur/oauth";
	ConfigFile config( filename );
	string accessTokenURL, consumerKey, consumerSecret;
	config.readInto( accessTokenURL, "AccessTokenURL" );
	config.readInto( consumerKey, "ConsumerKey" );
	config.readInto( consumerSecret, "ConsumerSecret" );

//#define DEBUG

#ifdef DEBUG
	string overwrite_nonce = "QSPrnopwNv08uv7a6Pf2ig7";
	string overwrite_timestamp = "1329148471";
	verifier = "0c1b115c7b9c87d357e9";
	token_key = "OGNhMjNmYzc0MTVhNDcwZmEzY2M";
	accessTokenURL = accessTokenURL + '?' + "oauth_verifier=" + verifier;
	accessTokenURL = accessTokenURL + "&oauth_nonce=" + overwrite_nonce + "&oauth_timestamp=" + overwrite_timestamp;
#else
	accessTokenURL = accessTokenURL + '?' + "oauth_verifier=" + verifier;
#endif

	const char *access_token_uri = accessTokenURL.c_str();
	const char *req_c_key         = consumerKey.c_str();
	const char *req_c_secret      = consumerSecret.c_str();

	const char *t_key    = token_key.c_str(); //< access token key
	const char *t_secret = token_secret.c_str(); //< access token secret

	char *callback = NULL; // this time no callback function

	char *req_url = NULL;
	char *reply = NULL;
	bool success = false;

	req_url = oauth_sign_url2(access_token_uri, NULL, OA_HMAC, NULL, req_c_key, req_c_secret, t_key, t_secret);

	cout << "Request URL = " << req_url << endl;
	if (onlyprint) {
		cout << "HTTP request not send because of only-print mode" << endl;
	} else {
		reply = oauth_http_get(req_url,NULL);
	}

	char *res_t_key    = NULL; //< replied key
	char *res_t_secret = NULL; //< replied secret

	if (!reply || onlyprint) {
		printf("HTTP request for an oauth request-token failed.\n");
		if (!onlyprint)
			printf("HTTP-reply: %s\n", reply);
	} else {
		int rc;
		char **rv = NULL;

		printf("HTTP-reply: %s\n", reply);
		rc = oauth_split_url_parameters(reply, &rv);
		// can be used, but sort can include callback_confirmed or not
		//		qsort(rv, rc, sizeof(char *), oauth_cmpstringp);
		if (rc < 2) {
			cerr << "Not enough arguments returned" << endl;
			goto oauth_err;
		}
		for (int i = 0; i < rc; ++i) {
			if (!strncmp(rv[i],"oauth_token_secret=",18) ) {
				res_t_secret=strdup(&(rv[i][19]));
			} else if (!strncmp(rv[i],"oauth_token=",11)) {
				res_t_key=strdup(&(rv[i][12]));
			}
		}
		printf("key: '%s'\nsecret: '%s'\n", res_t_key, res_t_secret);
		if (res_t_key != NULL && res_t_secret != NULL) {
			token_key = string(res_t_key);
			token_secret = string(res_t_secret);
			if (!token_key.empty() && (!token_secret.empty()))
				success = true;
		}
oauth_err:
		if(rv) free(rv);
	}

	if(req_url) free(req_url);
	if(reply) free(reply);
	if(res_t_key) free(res_t_key);
	if(res_t_secret) free(res_t_secret);
	return success;
}

/**
 * Get custom URL with given content.
 */
string CSOAuthConnector::OAuth(string url, string content, string http_method, string &token_key, string &token_secret) {
	char *home = getenv("HOME");
	bool onlyprint = false;
	string filename = string(home) + "/.rur/oauth";
	ConfigFile config( filename );
	string consumerKey, consumerSecret;
	config.readInto( consumerKey, "ConsumerKey" );
	config.readInto( consumerSecret, "ConsumerSecret" );

	const char *req_c_key         = consumerKey.c_str();
	const char *req_c_secret      = consumerSecret.c_str();

	const char *t_key    = token_key.c_str(); //< access token key
	const char *t_secret = token_secret.c_str(); //< access token secret

	char *postargs = new char[content.size()+1];
	postargs[content.size()] = 0;
	memcpy(postargs, content.c_str(), content.size());
	char *req_url = NULL;
	char *reply = NULL;
	bool success = false;

	req_url = oauth_sign_url2(url.c_str(), NULL, OA_HMAC, http_method.c_str(), req_c_key, req_c_secret, t_key, t_secret);

	cout << "Request URL = " << req_url << endl;
	cout << "Data string = " << content << endl;
	if (onlyprint) {
		cout << "HTTP request not send because of only-print mode" << endl;
	} else {
		if (content.empty()) {
			reply = oauth_http_get(req_url, NULL);
		} else {
			string header = "Content-Type: application/json";
			reply = oauth_http_post2(req_url, content.c_str(), header.c_str());
		}
	}
	delete [] postargs;

	char *res_t_key    = NULL; //< replied key
	char *res_t_secret = NULL; //< replied secret

	if (!reply || onlyprint) {
		printf("HTTP request for an oauth request-token failed.\n");
		if (!onlyprint)
			printf("HTTP-reply: %s\n", reply);
	} else {
		int rc;
		printf("HTTP-reply: %s\n", reply);
	}

	if(req_url) free(req_url);
	// Scott thinks: if we free reply, we get a void message!
	string s_reply = string(reply);
	if(reply) free(reply);
	if(res_t_key) free(res_t_key);
	if(res_t_secret) free(res_t_secret);
	
	return s_reply;
}

