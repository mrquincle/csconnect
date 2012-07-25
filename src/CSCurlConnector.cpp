/**
 * @file CSCurlConnector.cpp
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

#include <CSCurlConnector.h>
#include <ConfigFile.h>
#include <CSCurl.h>

#include <string>
#include <sstream>
#include <time.h>

#include <CSSensor.h>

using namespace cs;
using namespace std;

CSCurlConnector::CSCurlConnector():  CSConnect() {

}

CSCurlConnector::~CSCurlConnector() {
	delete curl;
}

void CSCurlConnector::Init() {
	cout << "Initialize the Curl connector" << endl;
	curl = new CSCurl();
	curl->Init();
}

void CSCurlConnector::Login(CSLogin & id) {
	cerr << "A separate login is not necessary for a curl connector" << endl;
}

//#define DEBUG

//! We can add a sensor to the platform, sensor_id needs to be returned
void CSCurlConnector::AddSensor(CSLogin & id, std::string name, std::string display_name, std::string data_type,
		std::string device_type, int & sensor_id) {
	curl->SetRequestMethod(REQUEST_METHOD_GET);
	string login = CreateLogin(CreateService(SENSOR_SERVICE));
#ifdef DEBUG
	cout << "Used login: " << login << endl;
#endif
	curl->Login(login);
	string data = "{\"sensor\":{\"name\":\"" + string(name) + "\",\"display_name\":\"" + string(display_name) +
			"\",\"data_type\":\"" + string(data_type) + "\",\"device_type\":\"" + string(device_type) + "\"}}";
	curl->SetData(data);
	curl->Run();

	// this would normally return immediately, but now we want to have AddSensor return the value
	// even if this means waiting...
	string result = curl->GetResult();
#ifdef DEBUG
	cout << result << endl;
#endif

	// we want to set sensor_id, but have to parse the result for that
	CSSensor *sensor = new CSSensor(result);
	sensor_id = sensor->sensor_id;
#ifdef DEBUG
	cout << "Sensor id: " << sensor_id << endl;
#endif
	delete sensor;
}

//! Add data items for a specific sensor
void CSCurlConnector::AddData(CSLogin & id, int sensor_id, std::vector<CSData> & datav) {
	curl->SetRequestMethod(REQUEST_METHOD_POST);
	string login = CreateLogin(CreateService(DATA_SERVICE, sensor_id));
#ifdef DEBUG
	cout << "Used login: " << login << endl;
#endif
	curl->Login(login);
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

	string data = datas.str();
	curl->SetData(data);
	curl->Run();
}

//! Add data items for a specific sensor
void CSCurlConnector::AddData(CSLogin & id, int sensor_id, CSData dataf, int unixTimeStamp) {
	curl->SetRequestMethod(REQUEST_METHOD_POST);
	string login = CreateLogin(CreateService(DATA_SERVICE, sensor_id));
#ifdef DEBUG
	cout << "Used login: " << login << endl;
#endif
	curl->Login(login);
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

	string data = datas.str();
#ifdef DEBUG
	cout << "String to be send: " << data << endl;
#endif
	curl->SetData(data);
	curl->Run();

}

//! Add data items in the form of a json string
void CSCurlConnector::AddData(CSLogin & id, int sensor_id, std::string jsondata) {
	cerr << "Not implemented yet!" << endl;
}

//! Get a list of sensors
std::string CSCurlConnector::GetSensors(CSLogin & id) {
	cerr << "Not implemented yet!" << endl;
	return "{empty}";
}

//! Get data of a specific sensor
std::string CSCurlConnector::GetData(CSLogin & id, int sensor_id, int unixTimeStamp) {
	cerr << "Not implemented yet!" << endl;
	return "{empty}";
}


/**
 * The old-fashioned way to login to CommonSense. This only works from your local laptop
 * because it requires username and password to be stored in your home directory and is
 * absolutely not meant for a multi-user system.
 */
string CSCurlConnector::CreateLogin(string servicestr) {
	char *home = getenv("HOME");
	string filename = string(home) + "/.rur/config";
	ConfigFile config( filename );
	string username, password;
	config.readInto( username, "username" );
	config.readInto( password, "password" );
	string login = "http://api.sense-os.nl/" + servicestr + "?username=" + username + "&password=" + password;
	return login;
}

/**
 * Helper function for AddSensor and AddData
 */
string CSCurlConnector::CreateService(CSService service, int sensor_id) {
	ostringstream servicestr;
	switch (service) {
	case SENSOR_SERVICE: servicestr << "sensors" << ".json"; break;
	case DATA_SERVICE: servicestr << "sensors/" << sensor_id << "/data.json"; break;
	}
	return servicestr.str();
}




