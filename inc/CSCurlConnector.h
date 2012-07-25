/**
 * @file CSCurlConnector.h
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

#ifndef CSCURLCONNECTOR_H_
#define CSCURLCONNECTOR_H_

#include <csconnect/CSConnect.h>
#include <csconnect/CSCurl.h>

namespace cs {

enum CSService { SENSOR_SERVICE, DATA_SERVICE };

class CSCurlConnector: public CSConnect {
public:
	CSCurlConnector();

	virtual ~CSCurlConnector();

	void Init();

	//! Login
	void Login(CSLogin & id);

	//! We can add a sensor to the platform, sensor_id needs to be returned
	void AddSensor(CSLogin & id, std::string name, std::string display_name, std::string data_type,
			std::string device_type, int & sensor_id);

	//! Add data items for a specific sensor
	void AddData(CSLogin & id, int sensor_id, std::vector<CSData> & data);

	//! Add data items for a specific sensor
	void AddData(CSLogin & id, int sensor_id, CSData data, int unixTimeStamp = 0);

	//! Add data items in the form of a json string
	void AddData(CSLogin & id, int sensor_id, std::string jsondata);

	//! Get a list of sensors
	std::string GetSensors(CSLogin & id);

	//! Get data of a specific sensor
	std::string GetData(CSLogin & id, int sensor_id, int unixTimeStamp=0);

private:
	//! Private curl object, this is the way we access libcurl functions
	CSCurl *curl;

	//! Helper function to AddSensor and AddData
	std::string CreateService(CSService service, int sensor_id = 0);

	//! Old-fashioned way to log in using username and password (deprecated)
	std::string CreateLogin(std::string servicestr);

};

}


#endif /* CSCURLCONNECTOR_H_ */
