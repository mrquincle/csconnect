/**
 * @file CSConnectFactory.h
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

#ifndef CSCONNECTFACTORY_H_
#define CSCONNECTFACTORY_H_

#include <csconnect/CSConnect.h>

namespace cs {

enum Connector { CURL_CONNECTOR, OAUTH_CONNECTOR };

class CSConnectFactory {
public:
	//! Get instance (using this connector)
	static CSConnectFactory* Instance(Connector c);

	//! Get connector
	CSConnect * GetNewConnector();

private:
	//! The connector
	Connector connector;

	//! Singleton pattern: private constructor
	CSConnectFactory(Connector c){ connector = c; };

	//! Singleton pattern: no copying by construction
	CSConnectFactory(CSConnectFactory const&){};

	//! Singleton pattern, no copying!!
	CSConnectFactory& operator=(CSConnectFactory const&){};

	//! Reference to single instance of factory
	static CSConnectFactory* m_pInstance;
};

}

#endif /* CSCONNECTFACTORY_H_ */
