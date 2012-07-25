# CSConnect

The CSConnect library (libcsconnect.so) can be used to connect to the CommonSense database at http://common.sense-os.nl/

## Installation

You will need to install three dependencies: curl, oauth, and a json parser.

	sudo apt-get install libcurl-dev liboauth-dev libjson-spirit-dev

On Ubuntu libcurl-dev is a virtual package, so you will need to replace it with libcurl4-openssl-dev, libcurl4-nss-dev, or libcurl4-gnutls-dev, whatever floats your boat. 

The build process uses "cmake" and there is a default Makefile in the root, so just running "make" should be enough after you solved the dependencies.

## Deployment

In the case of using the "oauth" method of authentication with the CommonSense server, you will need to have the user "in the loop". If the user fills in his/her account data, the temporary tokens can be replaced by long-term tokens. This method of authentication is recommended if you are building your own third-party server that wants to be able to write things to the CommonSense database on the behalf of a user and where this server needs to do this for many users at the same time.

On your own system, however, you can just also use basic username+password authentication. The csconnect library expects the information to be on your harddrive in an unencrypted manner. (We strongly recommend to use oauth in case of production systems, and hence we did not care about encryption.) The given file should be: ~/.rur/config 

	# Configuration file for senseConnector using ConfigFile
	username = yourname@yourdomain.ext          # go to http://common.sense-os.nl to create an account
	password = fa6c8d757955fef1c036cae09e1b2397 # use MD5 generator like http://www.adamek.biz/md5-generator.php

## Copyrights
The copyrights (2012) belong to:

- Author: Anne van Rossum
- Author: Scott Huo
- Almende B.V., http://www.almende.com and DO bots B.V., http://www.dobots.nl
- Rotterdam, The Netherlands


