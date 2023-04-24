//
// Created by Lowell Zima on 3/23/23.
//

#include "../../includes/Webserv_Includes.hpp"



/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

Config::Config() { std::cout <<W<< "++config" <<RE<< std::endl; }
Config::~Config() 	{ std::cout <<W<< "--config" <<RE<< std::endl; }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

void Config::setNameServer		(std::string name) 										{ this->_name = name; }
void Config::setIpServer		(std::string ip)				 						{ this->_ip = ip; }
void Config::setPortServer		(std::string port)										{ this->_port = port; }
void Config::setServerConfig	(std::vector<std::string> config)						{ this->_serverconfig = config; }
void Config::setAllLocation		(std::vector<std::map<std::string, std::string> > locs)	{ this->_locs = locs; }
void Config::setKeyTemp			(std::string key_temp) 									{ this->_key_temp = key_temp; }
void Config::setValueTemp		(std::string value_temp) 								{ this->_value_temp = value_temp; }
void Config::setMimeMap			(std::map<std::string,std::string> mime_type)			{ this->_mime_types = mime_type; }
void Config::setErrorPagesMap	(std::map<int, std::string> error_pages)				{ this->_error_pages = error_pages; }
void Config::setErrorNamesMap	(std::map<int, std::string> error_names)				{ this->_error_names = error_names; }

/* weirdo setters */
void Config::setRawfile	(
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

std::string 										&Config::getNameServer(void) 			{ return(this->_name); }
std::string 										&Config::getIpServer(void) 				{ return(_ip); }
std::string 										&Config::getPortServer(void) 			{ return(_port); }
std::vector<std::string> 							&Config::getAllServerConfig (void)		{ return (_serverconfig); }
std::vector<std::map<std::string, std::string> > 	&Config::getAllLocations(void)			{ return (_locs); }
std::map<std::string,std::string>					&Config::getMimeMap(void)				{ return(_mime_types); }
std::map<int, std::string>							&Config::getErrorPagesMap(void)			{ return(_error_pages); }
std::map<int, std::string>							&Config::getErrorNamesMap(void)			{ return(_error_names); }

std::string &Config::getKeyTemp(void) 				{return (_key_temp); }
std::string &Config::getValueTemp(void) 			{return (_value_temp); }

std::string
Config::getType(const std::string& format) {
	std::map<std::string, std::string>::iterator itm;
	itm = _mime_types.find(format);
	if (itm != _mime_types.end()){
		return (itm->second);
	}
	else
	{
		std::cerr<<R<< "Error: getType: "<< format << " not found." <<RE<< std::endl;
		return (std::string());
	}
}

std::string
Config::getErrorPages(int error_pages) {
	std::map<int, std::string>::iterator itm;
	itm = _error_pages.find(error_pages);
	if (itm != _error_pages.end()){
		return (itm->second);
	}
	else
	{
		std::cerr << R << "Error: getErrorPages: " << error_pages << " not found." << RE << std::endl;
		return (std::string());
	}
}


std::string
Config::getErrorNames(int error_names) {
	std::map<int, std::string>::iterator itm;
	itm = _error_names.find(error_names);
	if (itm != _error_names.end()){
		return (itm->second);
	}
	else
	{
		std::cerr << R << "Error: getErrorNames: " << error_names << " not found." << RE << std::endl;
		return (std::string());
	}
}

/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

// Print all content of a location | ex: s0->printAllLocation();;
void
Config::printAllLocation()
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
Config::printAllContentsLocation(std::string pathLocation) {

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
Config::getInLocationValue(std::string PathLocation, std::string key)
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
