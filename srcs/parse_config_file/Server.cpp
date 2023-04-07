//
// Created by Lowell Zima on 3/23/23.
//

#include "Server.hpp"
#include <vector>

/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/


Server::Server() 	{ std::cout <<W<< "++server" <<RE<< std::endl; }
Server::~Server() 	{ std::cout <<W<< "--server" <<RE<< std::endl; }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/


void Server::setNameServer		(std::string name) 										{ this->_name = name; }
void Server::setIpServer		(std::string ip)				 						{ this->_ip = ip; }
void Server::setPortServer		(std::string port)										{ this->_port = port; }
void Server::setServerConfig	(std::vector<std::string> config)						{ this->_serverconfig = config; }
void Server::setAllLocation		(std::vector<std::map<std::string, std::string> > locs)	{ this->_locs = locs; }

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


//todo
//std::vector<std::string>::iterator meth = getLocation("/methods");

//void Server::setLocPath(std::vector<std::string>::iterator			loc)				{
//	std::vector<std::vector<std::string> >::iterator	it_locs = getAllLocation().begin();
//	while(this->)
//}


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

std::string 										&Server::getNameServer(void) 			{ return(this->_name); }
std::string 										&Server::getIpServer(void) 				{ return(_ip); }
std::string 										&Server::getPortServer(void) 			{ return(_port); }
std::vector<std::string> 							&Server::getAllServerConfig (void)		{ return (_serverconfig); }
std::vector<std::map<std::string, std::string> > 	&Server::getAllLocations(void)			{ return (_locs); }


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/


void Server::printAllMap()
{
	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;
	for (itv = _locs.begin(); itv != _locs.end(); itv++)
	{
		for (itm = (*itv).begin(); itm != (*itv).end(); itm++)
			std::cout <<G<< itm->first << " | "<< itm->second << RE <<  std::endl;
		std::cout << "----------------------------------" << std::endl;
	}
}

bool Server::printMap(std::string s) {

	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;

	for (itv = _locs.begin(); itv != _locs.end(); itv++) {

		itm = (*itv).find("location");
		if (itm != (*itv).end()) {
			if (itm->second == s) {
				std::cout << G << "Location path: '" << RE << Y << s << RE << G << "' found" << std::endl;
				std::cout << G << "Location Key: '" << itm->first << RE << Y << "' \t | Value: '" << itm->second <<RE<< std::endl;
				std::cout << G << "Content Location '" <<RE << std::endl;
				for (itm = (*itv).begin(); itm != (*itv).end(); itm++)
					std::cout << itm->first << RE << Y << "' \t | Value: '" << itm->second <<RE<< std::endl;
			}
		}
		else {
			std::cerr << R << "Location path: '" << RE << Y << s << RE << R << "' not found" << std::endl;
			return (false);
		}
	}
	return(true);
}

bool Server::getInLocationValue(std::string LocationPath, std::string key)
{
	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;

	for (itv = _locs.begin(); itv != _locs.end(); itv++)
	{
		itm = (*itv).find("location");
		if (itm != (*itv).end())
		{
			if (itm->second == LocationPath)
			{
				std::cout <<C<< \
					">> Location \n" <<RE<<G<<
					"Key: \t'" << itm->first << RE << Y << \
					"' \n"
					"Value: \t'" << itm->second <<RE<< std::endl;
				itm = (*itv).find(key);
				if (itm != (*itv).end()) {
					std::cout <<C<< \
						">> Get \n" <<RE<<G<<
						"Key: \t'" << itm->first <<RE<< "' | stored in key_temp: " <<&key_temp << Y << \
						"\n"
						"Value: \t'" << itm->second <<RE<< " | store in value_temp: " << &value_temp << std::endl;
				}
			}
		}
		else {
			std::cerr << R << "Location path: '" << RE << Y << LocationPath << RE << R << "' not found" << std::endl;
			return (false);
		}
	}
	return(true);
}
