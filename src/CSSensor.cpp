/**
 * @file CSSensor.cpp
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


#include <iostream>
#include <json_spirit_reader.h>

#include <CSSensor.h>

using namespace cs;
using namespace std;

static const string Value_type_str[] = { "obj_type", "array_type", "str_type", "bool_type", "int_type", "real_type", "null_type" };

//#define DEBUG

/*
 * When we create a sensor we retrieve a JSON string back. We have to parse this JSON
 * string to retrieve the sensor id we got assigned.
 *
 * Errors are reported using "value type" which is an integer, or more specific, an
 * enumeration:
 *     enum Value_type{ obj_type, array_type, str_type, bool_type, int_type, real_type, null_type };
 * So, an error "what():  value type is 0 not 1" means that something was an object, not an array.
 */
CSSensor::CSSensor(std::string json) {
#ifdef DEBUG
	cout << "Parse (\"created sensor\") json string:" << endl << json << endl;
#endif
	// First parse the string which is expected to be a "json object"
	json_spirit::Value value;
	if (!read(json, value)) {
		valid = false;
		return;
	}
	if (value.type() != json_spirit::obj_type) {
		cerr << "[1] Unexpected object type \"" << Value_type_str[value.type()] << "\"";
		cerr << " instead of \"" << Value_type_str[json_spirit::obj_type] << "\"" << endl;
		return;
	}

	// a "json_spirit object" is - by default - a vector of json pairs
	json_spirit::Object dataobj = value.get_obj();

	// we assume that there is at least one pair
	json_spirit::Pair& pair = dataobj[0];

	// the name of the pair should be "sensor"
	string& name  = pair.name_;
	cout << "Retrieve json object with name \"" << name << "\"" << endl;

	// the value of this pair is NOT an array, it is directly a series of objects
	json_spirit::Value& value2 = pair.value_;
	if (value2.type() != json_spirit::obj_type) {
		cerr << "[2] Unexpected object type \"" << Value_type_str[value2.type()] << "\"";
		cerr << " instead of \"" << Value_type_str[json_spirit::obj_type] << "\"" << endl;
		return;
	}
	json_spirit::Object obj = value2.get_obj();

	// iterate over the array
	for( json_spirit::Object::size_type i = 0; i != obj.size(); ++i ) {
		const json_spirit::Pair& pair = obj[i];
		const string& name  = pair.name_;
#ifdef DEBUG
		cout << "Retrieve json object with name \"" << name << "\"" << endl;
#endif
		const json_spirit::Value&  value = pair.value_;

		if (name == "id") {
			istringstream buffer(value.get_str());
			buffer >> sensor_id;
		} else if (name == "sensor_name") {
			istringstream buffer(value.get_str());
			buffer >> sensor_name;
		} else if (name == "sensor_type") {
			istringstream buffer(value.get_str());
			buffer >> sensor_type;
		} else if (name == "data_type_id") {
			istringstream buffer(value.get_str());
			buffer >> data_type_id;
		} else if (name == "pager_type") {
			istringstream buffer(value.get_str());
			buffer >> pager_type;
		} else if (name == "display_name") {
			istringstream buffer(value.get_str());
			buffer >> display_name;
		} else if (name == "use_data_storage") {
			use_data_storage = value.get_bool();
		} else if (name == "data_type") {
			istringstream buffer(value.get_str());
			buffer >> data_type;
		} else if (name == "data_structure") {
			this->data_structure = value;
		}
	}
	valid = true;
}

