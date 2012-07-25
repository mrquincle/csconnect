/**
 * @file CSSensor.h
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
 * @date    Jul 25, 2012
 * @project Replicator FP7
 * @company Almende B.V.
 * @case    Artificial Intelligence Framework
 */

#ifndef CSSENSOR_H_
#define CSSENSOR_H_

#include <string>
#include <vector>

#include <json_spirit_value.h>

namespace cs {

/**
 * A sensor is represented in CommonSense in a certain way, for example:
 *  {
 *  "sensor":
 *    {
 *    "id":"151501",
 *    "name":"hense",
 *    "type":"0",
 *    "device_type":"virtual sensor",
 *    "data_type_id":"5",
 *    "pager_type":"",
 *    "display_name":"Hense the Sense",
 *    "use_data_storage":true,
 *    "data_type":"int",
 *    "data_structure":""
 *    }
 *  }
 *
 */
class CSSensor {
public:
	CSSensor(std::string json);

	// The unique identifier of a sensor, used to send data to it!
	int sensor_id;
	// The sensor name does not need to be unique
	std::string sensor_name;
	//
	int sensor_type;
	//
	std::string device_type;
	//
	int data_type_id;
	//
	std::string pager_type;
	//
	std::string display_name;
	//
	bool use_data_storage;
	// A string representation of the data type, e.g. "int"
	std::string data_type;
	// A representation of the structure of the data in this sensor
	json_spirit::Value data_structure;

	//! Status value to indicate that parsing went okay
	bool valid;
};

}


#endif /* CSSENSOR_H_ */
