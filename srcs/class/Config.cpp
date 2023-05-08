//
// Created by Lowell Zima on 3/23/23.
//

#include "../../includes/Webserv_Includes.hpp"



/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

Config::Config() { /*std::cout <<W<< "++config" <<RE<< std::endl;*/ }
Config::~Config() 	{ /*std::cout <<W<< "--config" <<RE<< std::endl;*/ }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

void Config::setNameServer		(std::string name) 										{ this->_name = name; }
void Config::setIpServer		(std::string ip)				 						{ this->_ip = ip; }
void Config::setServerConfig	(std::vector<std::string> config)						{ this->_serverconfig = config; }
void Config::setAllLocation		(std::vector<std::map<std::string, std::string> > locs)	{ this->_locs = locs; }
void Config::setKeyTemp			(std::string key_temp) 									{ this->_key_temp = key_temp; }
void Config::setValueTemp		(std::string value_temp) 								{ this->_value_temp = value_temp; }
void Config::setMimeMap			(std::map<std::string,std::string> mime_type)			{ this->_mime_types = mime_type; }
void Config::setErrorPagesMap	(std::map<int, std::string> error_pages)				{ this->_error_pages = error_pages; }
void Config::setErrorNamesMap	(std::map<int, std::string> error_names)				{ this->_error_names = error_names; }

/* weirdo setters */
void
Config::setRawfile	(
		std::vector<std::string>::iterator first_bracket,
		std::vector<std::string>::iterator last_bracket)
{
	while (first_bracket != last_bracket)
	{
		this->_rawfile.push_back(*first_bracket);
		first_bracket++;
	}
}

bool
is_number(
		const std::string& s)
{
	std::string::const_iterator it;
	for (it = s.begin(); it != s.end() ; it++){
		if (!std::isdigit(*it))
			return (false);
	}
	return (true);
}

void
Config::setPortServer(
		std::string& input)
{
	input.erase(0, input.find("=") + 1);

	unsigned long	pos;
	int 			port;

	_amount_ports = 0;
	while ((pos = input.find(',')) != std::string::npos)
	{
		_amount_ports += 1;
		std::string tmp_port = input.substr(0, pos);
		input.erase(0, pos + 1);
		getOnlyChar(tmp_port);
		if (is_number(tmp_port)) {
			port = std::atoi(tmp_port.c_str());
			_ports.push_back(port);
		}
		else
			throw std::invalid_argument("Error: port isn't an int");
	}
	if (!input.empty())
	{
		if (is_number(input)) {
			_amount_ports += 1;
			port = std::atoi(input.c_str());
			_ports.push_back(port);
		}
		else
			throw std::invalid_argument("Error: port isn't an int");
	}
}


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

std::string 										&Config::getNameServer(void) 			{ return(this->_name); }
std::string 										&Config::getIpServer(void) 				{ return(_ip); }
std::vector<int> 									&Config::getPortServer(void) 			{ return(_ports); }
int 												&Config::getAmountPortServer(void) 		{ return(_amount_ports); }
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


std::string
Config::getPath_of_URI(const std::string& URIraw){
	std::vector<std::map<std::string, std::string> >::iterator i;
	std::map<std::string, std::string>::iterator im;

	//take out / of URI
	std::string URI = URIraw.substr(1, URIraw.size());

	//for example: URI="/kittycat.jpg"
	//look into all locations map (location=/, location=/methods, ...)
	for( i = _locs.begin(); i != _locs.end() ; i++ )
	{
		// find "root" in a location
		if ((im = (*i).find("root")) != (*i).end())
		{
			// take value of root=./website, so "./website", add URI, like "/kittycat.jpg"
			std::string all = im->second + URI;
			// check if it can be open()
			int fd;
			if((fd = open(all.c_str(), O_CREAT | O_EXCL)) != -1) {
				continue;
			}
			else {
				// if open() success = store and return
				// close fd if exist
				close(fd);
				// store location map where path have been found
//				_loc_temp = (*i);
				// return string of path tested = ./website/kittycat.jpg";
//				std::cerr <<G<< all << "WEEEESHHHHH" << RE << std::endl;
				return (all);
			}

		}
		else
			throw std::invalid_argument ("No root found in location");
	}
	// if not file found, return empty string
	return (std::string(""));
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
