/**
 * @file CSConnectFactory.cpp
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

#include <CSConnectFactory.h>

#include <CSCurlConnector.h>
#include <CSOAuthConnector.h>

using namespace cs;

// Global static pointer used to ensure a single instance of the class.
CSConnectFactory* CSConnectFactory::m_pInstance = 0;

CSConnectFactory* CSConnectFactory::Instance(Connector c)
{
	// Only allow one instance of class to be generated.
	if (!m_pInstance) {
		m_pInstance = new CSConnectFactory(c);
	}
	return m_pInstance;
}

CSConnect * CSConnectFactory::GetNewConnector() {
	if (connector == CURL_CONNECTOR) {
		return new CSCurlConnector();
	} else if (connector == OAUTH_CONNECTOR) {
		return new CSOAuthConnector();
	}

	// default also return Curl Connector
	return new CSCurlConnector();
}
