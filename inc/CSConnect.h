/**
 * @file CSConnect.h
 * @brief CommonSense platform connector
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
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#ifndef CSCONNECT_H_
#define CSCONNECT_H_

#include <string>
#include <vector>

namespace cs {

/**
 *  CSData struct, this is just one data field. For an entire sensor you need a vector
 *  of CSData
 */
struct CSDataField {
	std::string label;
	std::string value;
};

struct CSData {
	std::vector < CSDataField* > fields;
};

//! CSLogin struct for identification
struct CSLogin {
	std::vector<std::string> value;
};

/**
 * General interface to class that knows how to create sensors, data, retrieve sensors,
 * and retrieve data.
 */
class CSConnect {
public:
	CSConnect() {};

	virtual ~CSConnect() {};

	//! Initialisation
	virtual void Init() = 0;

	//! We need to login to access data/sensors
	virtual void Login(CSLogin & id) = 0;

	//! We can add a sensor to the platform, sensor_id needs to be returned
	virtual void AddSensor(CSLogin & id, std::string name, std::string display_name, std::string data_type,
			std::string device_type, int & sensor_id) = 0;

	//! Add data items for a specific sensor
	virtual void AddData(CSLogin & id, int sensor_id, std::vector<CSData> & data) = 0;

	//! Add data item for a specific sensor
	virtual void AddData(CSLogin & id, int sensor_id, CSData data, int unxTimeStamp=0) = 0;

	//! Add data items for a specific sensor
	virtual void AddData(CSLogin & id, int sensor_id, std::string jsondata) = 0;

	//! Get a list of sensors
	virtual std::string GetSensors(CSLogin & id) = 0;

	//! Get data of a specific sensor
	virtual std::string GetData(CSLogin & id, int sensor_id, int unixTimeStamp=0) = 0;
};

}

#endif /* CSCONNECT_H_ */
