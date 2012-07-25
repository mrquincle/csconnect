/**
 * @file test.cpp
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
 * @date    Jul 19, 2012
 * @project Replicator FP7
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#include <csconnect/CSConnect.h>
#include <csconnect/CSConnectFactory.h>
#include <iostream>
#include <sstream>

#include <sys/time.h>

using namespace cs;
using namespace std;

void oauthstep1(CSConnect *c) {
	cout << "Execute first step of authorisation" << endl;
	cout << "This will result in a temporary key and secret" << endl;
	CSLogin * id = new CSLogin();
	id->value.push_back("oauth0");
	c->Login(*id);
	if (id->value.size() != 2) {
		delete id;
		return;
	}
	string key = id->value[0];
	string secret = id->value[1];
	cout << "key: " << key << endl;
	cout << "secret: " << secret << endl;
	delete id;
}

void oauthstep2(CSConnect *c) {
	cout << "Execute second step of authorisation" << endl;
	cout << "We should now have obtained token, secret and verifier (somewhere online through user interaction)" << endl;
	cout << "This will result in a permanent key and secret" << endl;
	string oauth_token;
	string oauth_secret;
	string oauth_verifier;

	CSLogin * id = new CSLogin();
	id->value.push_back(oauth_token);
	id->value.push_back(oauth_secret);
	id->value.push_back(oauth_verifier);
	id->value.push_back("oauth1");
	c->Login(*id);
	if (id->value.size() != 2) {
		delete id;
		return;
	}
	string key = id->value[0];
	string secret = id->value[1];
	cout << "key: " << key << endl;
	cout << "secret: " << secret << endl;
	delete id;
}

void oauthlogin() {
	CSConnectFactory* fac = CSConnectFactory::Instance(CURL_CONNECTOR);
	CSConnect *c = fac->GetNewConnector();
	c->Init();
	//	teststep1(c);
	// observe that you cannot test step 2 properly without actually having a user in the loop online
	//	teststep2(c);
}

/**
 * We can also just login normally...
 */
void curllogin() {
	// nada, we do not need to login separately for the CURL CONNECTOR
}

void curladdsensordata() {
	CSConnectFactory* fac = CSConnectFactory::Instance(CURL_CONNECTOR);
	CSConnect *c = fac->GetNewConnector();
	c->Init();

	CSLogin * id = new CSLogin();

	int sensor_id = 0;
	c->AddSensor(*id, "hense", "Hense the Sense", "int", "virtual sensor", sensor_id);
	cout << "Added sensor with id " << sensor_id << endl;

	// create structure
	CSData data;
	CSDataField * datafield = new CSDataField();
	stringstream datastream;
	datafield->label = "value";
	data.fields.push_back(datafield);

	// fill structure
	datastream.clear(); datastream.str("");
	datastream << "23 2 3";
	datafield->value = datastream.str();

	// display how many fields we are gonna send
	cout << "The number of data fields added: " << data.fields.size() << endl;

	// current time does not need to be added...
	unsigned long int t = time(NULL);

	c->AddData(*id, sensor_id, data); //, t);
}

int main() {
	curladdsensordata();
}

