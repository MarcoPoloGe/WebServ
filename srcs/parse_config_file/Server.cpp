//
// Created by Lowell Zima on 3/23/23.
//

#include "Server.hpp"


/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

Server::Server() { std::cout <<W<< "++server" <<RE<< std::endl; }
Server::~Server() 	{ std::cout <<W<< "--server" <<RE<< std::endl; }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

void Server::setNameServer		(std::string name) 										{ this->_name = name; }
void Server::setIpServer		(std::string ip)				 						{ this->_ip = ip; }
void Server::setPortServer		(std::string port)										{ this->_port = port; }
void Server::setServerConfig	(std::vector<std::string> config)						{ this->_serverconfig = config; }
void Server::setAllLocation		(std::vector<std::map<std::string, std::string> > locs)	{ this->_locs = locs; }
void Server::setKeyTemp			(std::string key_temp) 									{ this->_key_temp = key_temp; }
void Server::setValueTemp		(std::string value_temp) 								{ this->_value_temp = value_temp; }

/* weirdo setters */
void Server::setRawfile	(
		std::vector<std::string>::iterator first_bracket,
		std::vector<std::string>::iterator last_bracket)
{
	while (first_bracket != last_bracket)
	{
		this->_rawfile.push_back(*first_bracket);
		first_bracket++;
	}
}

/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

std::string 										&Server::getNameServer(void) 			{ return(this->_name); }
std::string 										&Server::getIpServer(void) 				{ return(_ip); }
std::string 										&Server::getPortServer(void) 			{ return(_port); }
std::vector<std::string> 							&Server::getAllServerConfig (void)		{ return (_serverconfig); }
std::vector<std::map<std::string, std::string> > 	&Server::getAllLocations(void)			{ return (_locs); }

std::string &Server::getKeyTemp(void) {return (_key_temp); }
std::string &Server::getValueTemp(void) {return (_value_temp); }


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

// Print all content of a location | ex: s0->printAllLocation();;
void
Server::printAllLocation()
{
	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;

	std::cout << "***********************************************************************************" << std::endl;
	for (itv = _locs.begin(); itv != _locs.end(); itv++)
	{
		for (itm = (*itv).begin(); itm != (*itv).end(); itm++)
			std::cout <<G<< itm->first << " | "<<RE<<Y<< itm->second <<RE<<  std::endl;
		std::cout << "***********************************************************************************" << std::endl;
	}
}
// Print all content of a location | ex: s0->printAllContentsLocation("/methods");
bool
Server::printAllContentsLocation(std::string pathLocation) {

	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;

	for (itv = _locs.begin(); itv != _locs.end(); itv++) {

		itm = (*itv).find("location");
		if (itm != (*itv).end()) {
			if (itm->second == pathLocation) {

				std::cout << G << \
				"Location path: '" <<RE<<Y<< pathLocation <<RE<<G<< "' found" \
				<<RE<< std::endl;

				std::cout << G << \
				"Location Key: '" << itm->first <<RE<<Y<< "' \t | Value: '" << itm->second \
				<<RE<< std::endl;

				std::cout << G << \
				"Content Location '" \
				<<RE<< std::endl;

				for (itm = (*itv).begin(); itm != (*itv).end(); itm++) {
					std::cout << \
                    "Key : '" << itm->first <<RE<<Y<< "' \t | " \
                    "Value: '" << itm->second << "'" <<RE<< std::endl;
				}
			}
		}
		else {
			std::cerr <<R<< "Location path: '" <<RE<<Y<< pathLocation <<RE<<R<< "' not found" << std::endl;
			return (false);
		}
	}
	return(true);
}

// In a specific location, search for a key and if found, set key_temp and value_temp to corresponding variables
// ex: s0->getInLocationValue("/methods", "root");
bool
Server::getInLocationValue(std::string PathLocation, std::string key)
{
	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;

	for (itv = _locs.begin(); itv != _locs.end(); itv++)
	{
		itm = (*itv).find("location");
		if (itm != (*itv).end())
		{
			if (itm->second == PathLocation)
			{
/*print*/
//				std::cout <<C<< \
//					">> Location \n" <<RE<<G<<
//					"Key: \t'" << itm->first << RE << Y << \
//					"' \n"
//					"Value: \t'" << itm->second <<RE<< std::endl;
				itm = (*itv).find(key);
				if (itm != (*itv).end()) {
/*print*/
//					std::cout <<C<< \
//						">> Get \n" <<RE<<G<<
//						"Key: \t'" << itm->first <<RE<< "' | stored in _key_temp: " <<&_key_temp << Y << \
//						"\n"
//						"Value: \t'" << itm->second <<RE<< " | store in _value_temp: " << &_value_temp << std::endl;
					setKeyTemp(itm->first);
					setValueTemp(itm->second);
				}
			}
		}
		else {
			std::cerr << R << "Location path: '" << RE << Y << PathLocation << RE << R << "' not found" << std::endl;
			return (false);
		}
	}
	return(true);
}
