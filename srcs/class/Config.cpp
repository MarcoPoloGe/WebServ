//
// Created by Lowell Zima on 3/23/23.
//

#include "../../includes/Webserv_Includes.hpp"



/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

Config::Config()
{
	/*std::cout <<W<< "++config" <<RE<< std::endl;*/
	_DefaultErrorMap.insert( std::make_pair(400, "Bad Request") );
	_DefaultErrorMap.insert( std::make_pair(401,"Unauthorized") );
    _DefaultErrorMap.insert( std::make_pair(402,"Payment Required") );
    _DefaultErrorMap.insert( std::make_pair(403,"Forbidden") );
    _DefaultErrorMap.insert( std::make_pair(404,"Not Found") );
    _DefaultErrorMap.insert( std::make_pair(405,"Method Not Allowed") );
    _DefaultErrorMap.insert( std::make_pair(406,"Not Acceptable") );
    _DefaultErrorMap.insert( std::make_pair(407,"Proxy Authentication Required") );
    _DefaultErrorMap.insert( std::make_pair(408,"Request Time-out") );
    _DefaultErrorMap.insert( std::make_pair(409,"Conflict") );
    _DefaultErrorMap.insert( std::make_pair(410,"Gone") );
    _DefaultErrorMap.insert( std::make_pair(411,"Length Required") );
    _DefaultErrorMap.insert( std::make_pair(412,"Precondition Failed") );
    _DefaultErrorMap.insert( std::make_pair(413,"Request Entity Too Large") );
    _DefaultErrorMap.insert( std::make_pair(414,"Request-URI Too Long") );
	_DefaultErrorMap.insert( std::make_pair(415,"Unsupported Media Type") );
    _DefaultErrorMap.insert( std::make_pair(416,"Requested range unsatisfiable") );
    _DefaultErrorMap.insert( std::make_pair(417,"Expectation failed") );
    _DefaultErrorMap.insert( std::make_pair(418,"I’m a teapot") );
    _DefaultErrorMap.insert( std::make_pair(419,"Page expired") );
    _DefaultErrorMap.insert( std::make_pair(421,"Bad mapping / Misdirected Request") );
    _DefaultErrorMap.insert( std::make_pair(422,"Unprocessable entity") );
    _DefaultErrorMap.insert( std::make_pair(423,"Locked") );
    _DefaultErrorMap.insert( std::make_pair(424,"Method failure") );
    _DefaultErrorMap.insert( std::make_pair(425,"Too Early") );
    _DefaultErrorMap.insert( std::make_pair(426,"Upgrade Required") );
    _DefaultErrorMap.insert( std::make_pair(428,"Precondition Required") );
    _DefaultErrorMap.insert( std::make_pair(429,"Too Many Requests") );
    _DefaultErrorMap.insert( std::make_pair(431,"Request Header Fields Too Large") );
    _DefaultErrorMap.insert( std::make_pair(449,"Retry With") );
    _DefaultErrorMap.insert( std::make_pair(450,"Blocked by Windows Parental Controls") );
    _DefaultErrorMap.insert( std::make_pair(451,"Unavailable For Legal Reasons") );
    _DefaultErrorMap.insert( std::make_pair(456,"Unrecoverable Error") );

    _DefaultErrorMap.insert( std::make_pair(444,"No Response") );
    _DefaultErrorMap.insert( std::make_pair(495,"SSL Certificate Error") );
    _DefaultErrorMap.insert( std::make_pair(496,"SSL Certificate Required") );
    _DefaultErrorMap.insert( std::make_pair(497,"HTTP Request Sent to HTTPS Port") );
    _DefaultErrorMap.insert( std::make_pair(498,"Token expired/invalid") );
    _DefaultErrorMap.insert( std::make_pair(499,"Client Closed Request") );

    _DefaultErrorMap.insert( std::make_pair(500,"Internal Server Error") );
    _DefaultErrorMap.insert( std::make_pair(501,"Not Implemented") );
    _DefaultErrorMap.insert( std::make_pair(502,"Bad Gateway ou Proxy Error") );
    _DefaultErrorMap.insert( std::make_pair(503,"Service Unavailable") );
    _DefaultErrorMap.insert( std::make_pair(504,"Gateway Time-out") );
    _DefaultErrorMap.insert( std::make_pair(505,"HTTP Version not supported") );
    _DefaultErrorMap.insert( std::make_pair(506,"Variant Also Negotiates") );
    _DefaultErrorMap.insert( std::make_pair(507,"Insufficient storage") );
    _DefaultErrorMap.insert( std::make_pair(508,"Loop detected") );
    _DefaultErrorMap.insert( std::make_pair(509,"Bandwidth Limit Exceeded") );
    _DefaultErrorMap.insert( std::make_pair(510,"Not extended") );
    _DefaultErrorMap.insert( std::make_pair(511,"Network authentication required") );

    _DefaultErrorMap.insert( std::make_pair(520,"Unknown Error") );
    _DefaultErrorMap.insert( std::make_pair(521,"Web Server Is Down") );
    _DefaultErrorMap.insert( std::make_pair(522,"Connection Timed Out") );
    _DefaultErrorMap.insert( std::make_pair(523,"Origin Is Unreachable") );
    _DefaultErrorMap.insert( std::make_pair(524,"A Timeout Occurred") );
    _DefaultErrorMap.insert( std::make_pair(525,"SSL Handshake Failed") );
    _DefaultErrorMap.insert( std::make_pair(526,"Invalid SSL Certificate") );
    _DefaultErrorMap.insert( std::make_pair(527,"Railgun Error") );
}

Config::~Config() 	{ /*std::cout <<W<< "--config" <<RE<< std::endl;*/ }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

void Config::setNameServer		(std::string name) 										{ this->_name = name; }
void Config::setIpServer		(std::string ip)				 						{ this->_ip = ip; }
//void Config::setServerConfig	(std::vector<std::string> config)						{ this->_serverconfig = config; }
//void Config::setAllLocation		(std::vector<std::map<std::string, std::string> > locs)	{ this->_locs = locs; }
void Config::setKeyTemp			(std::string key_temp) 									{ this->_key_temp = key_temp; }
void Config::setValueTemp		(std::string value_temp) 								{ this->_value_temp = value_temp; }
void Config::setMimeMap			(std::map<std::string,std::string> mime_type)			{ this->_mime_types = mime_type; }
void Config::setErrorPagesMap	(std::map<int, std::string> error_pages)				{ this->_error_pages = error_pages; }
void Config::setErrorNamesMap	(std::map<int, std::string> error_names)				{ this->_error_names = error_names; }
void Config::setBinCgi			(const std::string &binCgi) 							{ this->_binCGI = binCgi; }

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
			throw std::invalid_argument("@fn Config::setPortServer(std::string& input)\nError: port isn't an int");
	}
	if (!input.empty())
	{
		if (is_number(input)) {
			_amount_ports += 1;
			port = std::atoi(input.c_str());
			_ports.push_back(port);
		}
		else
			throw std::invalid_argument("@fn Config::setPortServer(std::string& input)\nError: port isn't an int");
	}
}

//std::string Config::getPathToFile(std::string URIraw, std::map<std::string, std::string> singleLocationContent)
//{
//	if ( ft_get_extension(URIraw) == "" )
//	{
//
//		return (this->getRoot(singleLocationContent));
//	}
//	else
//		return (this->getRoot(singleLocationContent) + this->getFileFromURI(URIraw));
//}

//bool
//Config::checkAutoIndex(std::map<std::string, std::string> *singleLocationContent)
//{
//	if(this->getAutoindex(*singleLocationContent) == "true")
//		return (true);
//	return false;
//}


void
Config::setPathToFile (std::string &URIraw, std::map<std::string, std::string> *singleLocationContent)
{

	std::string minusfolder = this->eraseFolderLocationAndSlashFromURI(URIraw, *singleLocationContent);
	// erase from URI :
	// "/" return ""
	// "/img" return ""
	// "/img/" return ""
	// "/index.html return "index.html"
	// "uploads/uploaded_files/downloaded.txt" return "uploaded_files/downloaded.txt"
//	_path_to_file = this->getRoot(*singleLocationContent) + this->getDefault(*singleLocationContent);
	_path_to_file = this->getRoot(*singleLocationContent) + minusfolder;

}


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

//std::string 										&Config::getNameServer(void) 			{ return(this->_name); }
std::string 										&Config::getIpServer(void) 				{ return(_ip); }
std::vector<int> 									&Config::getPortServer(void) 			{ return(_ports); }
int 												&Config::getAmountPortServer(void) 		{ return(_amount_ports); }
//std::vector<std::string> 							&Config::getAllServerConfig (void)		{ return (_serverconfig); }
std::vector<std::map<std::string, std::string> > 	&Config::getAllLocations(void)			{ return (_locs); }
//std::map<std::string,std::string>					&Config::getMimeMap(void)				{ return(_mime_types); }
//std::map<int, std::string>							&Config::getErrorPagesMap(void)			{ return(_error_pages); }
//std::map<int, std::string>							&Config::getErrorNamesMap(void)			{ return(_error_names); }
const std::string 									&Config::getBinCgi() const 				{ return (_binCGI); }
const std::string 									&Config::getPath() const 				{ return (_path_to_file); }

//std::string 										&Config::getKeyTemp(void) 				{ return (_key_temp); }
//std::string 										&Config::getValueTemp(void)				{ return (_value_temp); }

std::string
Config::getType(const std::string& format) {
	std::map<std::string, std::string>::iterator itm ;
	itm = _mime_types.find(format);
	if (itm != _mime_types.end()){
		return (itm->second);
	}
	else
	{
		std::cerr<<R<< "@fn Config::getType(const std::string& format)\nError: getType: "<< format << " not found." <<RE<< std::endl;
		return (std::string());
	}
}


//std::string
//Config::getContentType(const std::string& format) {
//	std::map<std::string, std::string>::iterator itm ;
//
//	itm = _mime_types.find(format);
//	if (itm != _mime_types.end()){
//		return (itm->first);
//	}
//	else
//	{
//		std::cerr<<R<< "@fn Config::getContentType(const std::string& format)\nError: getType: "<< format << " not found." <<RE<< std::endl;
//		return (std::string());
//	}
//}


std::string
Config::getErrorPages(int error_pages) {
	std::map<int, std::string>::iterator itm;
	itm = _error_pages.find(error_pages);
	if (itm != _error_pages.end()){
		return (itm->second);
	}
	else
	{
		std::cerr << R << "@fn Config::getErrorPages(int error_pages)\nError: getErrorPages: " << error_pages << " not found." << RE << std::endl;
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
		std::cerr << R << "@fn Config::getErrorNames(int error_names) \nError: getErrorNames: " << error_names << " not found." << RE << std::endl;
		return (std::string());
	}
}



/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/


std::string Config::getVarLocation(std::map<std::string, std::string> &singleMapLocation, const std::string& var )
{
	std::map<std::string, std::string>::iterator im = singleMapLocation.begin();
	if ((im = singleMapLocation.find(var)) != singleMapLocation.end())
		return(im->second);
	return ("");
}

std::string Config::getRoot(std::map<std::string, std::string> &singleMapLocation ) {
	return (getVarLocation(singleMapLocation, "root"));
}

std::string Config::getReturn(std::map<std::string, std::string> &singleMapLocation ) {
	return (getVarLocation(singleMapLocation, "return"));
}
std::string Config::getDefault(std::map<std::string, std::string> &singleMapLocation ) {
	return (getVarLocation(singleMapLocation, "default"));
}

std::string Config::getAutoindex(std::map<std::string, std::string> &singleMapLocation ) {
	return (getVarLocation(singleMapLocation, "autoindex"));
}

std::string Config::getLocation(std::map<std::string, std::string> &singleMapLocation ) {
	return (getVarLocation(singleMapLocation, "location"));
}

std::string Config::getMethod(std::map<std::string, std::string> &singleMapLocation ) {
	return (getVarLocation(singleMapLocation, "method"));
}

std::map<std::string, std::string> *Config::getSingleMapLocation(const std::string &folderLocation)
{
	// folder example: /Not_In_Locations or /img
	std::vector<std::map<std::string, std::string> >::iterator i;
	std::map<std::string, std::string>::iterator im;
	for (i = _locs.begin(); i != _locs.end(); i++){
		if (((im = (*i).find("location")) != (*i).end()) && (im->second == folderLocation))
			return (&(*i));
	}
	return(nullptr);
}

bool Config::IsMethodAllowed(std::string Method, std::map<std::string, std::string> &singleMapLocation)
{
	std::string MethodAllowed = this->getMethod(singleMapLocation);
	if (MethodAllowed.find(Method) != std::string::npos)
		return(true);
	return (false);
}

std::string Config::isPathToFile(const std::string &PathToFile)
{
	std::ifstream to_open(PathToFile.c_str(), std::ios::in);
	if ( !to_open.is_open() ){
//		throw std::invalid_argument ("@fn Config::isPathToFile(std::string URIraw, std::map<std::string, std::string> *singleLocationContent)\nNo file found in location with URI");
		return ("");
	}
	else {
		to_open.close();
		return (PathToFile);
	}
}
//
//std::string Config::getFileInFolderFromURI(std::string URIraw, std::string Folder)
//{
//	unsigned long pos;
//	if (URIraw == Folder)
//		return("");
//	if ((pos = URIraw.find(Folder)) != std::string::npos)
//	{
//		std::cout << pos + Folder.size() << std::endl;
//		return (URIraw.substr( pos + Folder.size()));
//	}
//	return ("");
//}
//
//std::string Config::getFileFromURI(std::string URIraw)
//{
//	unsigned long pos;
//
//	if ((pos = URIraw.rfind('/')) != std::string::npos)
//	{
//		return (URIraw.substr( pos, URIraw.size()));
//	}
//	return ("");
//}

std::string Config::getFolderLocationFromURI(std::string URIraw)
{

	// /img || /img/kpoj || /img/   ||   /img  || /index.html
	if (URIraw == "/")
		return (URIraw);

	std::size_t	first_slash = URIraw.find('/');
	std::size_t	sec_slash = URIraw.find('/', first_slash + 1);

	if (sec_slash == std::string::npos)
	{
		std::string ext = ft_get_extension(URIraw);
		if (ext.empty())
			return (URIraw);
		return ("/");
	}
	return ( URIraw.substr( first_slash,  sec_slash) ); // return /img
}

std::string Config::eraseFolderLocationAndSlashFromURI(std::string URIraw, std::map<std::string, std::string> &singleMapLocation)
// /img/ || /img || /img/kittycat.jpg || /index.html || /uploads/uploaded_files/
{
	if (URIraw == "/")
		return ("");
	if (URIraw.find(getLocation(singleMapLocation)) != std::string::npos)
	{
		std::size_t	first_slash = URIraw.find('/');
		std::size_t	sec_slash = URIraw.find('/', first_slash + 1);

		if (sec_slash == std::string::npos)
			return (URIraw.erase(0, getLocation(singleMapLocation).size() ));
		else
			return (URIraw.erase(0, getLocation(singleMapLocation).size() + 1)); //kittycat.jpg

	}
	throw std::invalid_argument("Config::eraseFolderLocationAndSlashFromURI(std::string URIraw, std::map<std::string,"
								" std::string> &singleMapLocation): Something went wrong with the path");
}

//Response
//Config::IsLocation(const std::string& URIraw,
//				   const std::string &Method
//				   )
//{
//	// URIraw = "/"
//	// URI = "index.html"
//	// URI = "./website/index.html"
//	// folder = ""
//
//	std::vector<std::map<std::string, std::string> >::iterator i;
//	std::map<std::string, std::string>::iterator im;
//	std::string folder;
//	Response ret((*this));
//
//	unsigned long pos;
//	std::string URI = URIraw.substr(1, URIraw.size());
//	std::string path;
//
//	if ((pos = URI.find('/')) != std::string::npos || URI.empty()) //
//	{
//		folder = "/" + URI.substr(0, pos); // img/
//		for (i = _locs.begin(); i != _locs.end(); i++)
//		{
//			if (((im = (*i).find("location")) != (*i).end()) && (im->second == folder))
//			{
//				if (((im = (*i).find("method")) != (*i).end()) && (im->second.find(Method) != std::string::npos))
//				{
//					if (im->second.find(Method) != std::string::npos)
//					{
//						URI.erase(0, folder.size());
//						path = getPath_of_URI(URI, i, im);
//						if (path.empty())
//						{
//							ret.set_error_code(404); // file doesn't exist in folder from locations
//							return (ret);
//						}
//					}
//					else
//					{
//						ret.set_error_code(405);
//						return (ret); // Method not allowed
//					}
//				}
//				break;
//			}
//		}
//		if (i == _locs.end()) {
//			ret.set_error_code(404);
//			return (ret);// no folder location for ./website/folder/ in config
//		}
//	}
//	else
//	{
//		for (i = _locs.begin(); i != _locs.end(); i++)
//		{
//			if (((im = (*i).find("location")) != (*i).end()) && (im->second == "/"))
//			{
//				if (((im = (*i).find("method")) != (*i).end()) && (im->second.find(Method) != std::string::npos))
//				{
//					if (im->second.find(Method) != std::string::npos)
//					{
////						URI.erase(0, 1);
//						path = getPath_of_URI(URIraw, i, im); //  URIraw = "/index.html"
//						if (path.empty())
//						{
//							ret.set_error_code(404); // file doesn't exist in folder from locations
//							return (ret);
//						}
//					}
//					else
//					{
//						ret.set_error_code(405);
//						return (ret); // Method not allowed
//					}
//				}
//				break;
//			}
//		}
//	}
//	ret.set_path(path);
//	return (ret);
//}

std::string
Config::getPath_of_URI(const std::string& URIraw,
					   std::vector<std::map<std::string, std::string> >::iterator &i,
					   std::map<std::string, std::string>::iterator &im
					   )
{
	//look into all locations map (location=/, location=/methods, ...)
	// find "root" in a location
	if ((im = (*i).find("root")) != (*i).end())
	{
		// take value of root=./website, so "./website", add URI, like "/kittycat.jpg"
		std::string all = im->second + URIraw; // URI = ./website/index.html
		// check if it exist
		std::ifstream to_open(all.c_str(), std::ios::in);
		if ( !to_open.is_open() )
			return ("");
		else {
			// if open() success = store and return
			// close fd if exist
			to_open.close();
			// store location map where path have been found
//				_loc_temp = (*i);
			// return string of path tested = ./website/kittycat.jpg";
			return (all);
		}
	}
	else
		throw std::invalid_argument ("@fn Config::getPath_of_URI(const std::string& URIraw)\nNo root found in location");
}

std::string	Config::getDefaultErrorDescription(int error)
{
	std::map<int, std::string>::iterator it = _DefaultErrorMap.find(error);

	if (it == _DefaultErrorMap.end() )
		return ("");
	return ((*it).second);
}


// In a specific location, search for a key and if found, set key_temp and value_temp to corresponding variables
// ex: s0->getInLocationValue("/methods", "root");
//bool
//Config::getInLocationValue(std::string PathLocation, std::string key)
//{
//	std::vector<std::map<std::string, std::string> >::iterator itv;
//	std::map<std::string, std::string>::iterator itm;
//
//	setKeyTemp("");
//	setValueTemp("");
//
//	for (itv = _locs.begin(); itv != _locs.end(); itv++)
//	{
//		itm = (*itv).find("location");
//		if (itm != (*itv).end())
//		{
//			if (itm->second == PathLocation)
//			{
//				itm = (*itv).find(key);
//				if (itm != (*itv).end()) {
//					setKeyTemp(itm->first);
//					setValueTemp(itm->second);
//				}
//			}
//		}
//		else {
//			std::cerr << W << "@fn Config::getInLocationValue(std::string PathLocation, std::string key)" <<RE<< std::endl;
//			std::cerr << R << "Location path: '" << RE << Y << PathLocation << RE << R << "' not found" << std::endl;
//			return (false);
//		}
//	}
//	return(true);
//}
//
//std::map<std::string, std::string> *Config::getLocationFolderContent(const std::string &folder){
//	std::vector<std::map<std::string, std::string> >::iterator itv;
//	std::map<std::string, std::string>::iterator itm;
//
//	for (itv = _locs.begin(); itv != _locs.end(); itv++)
//	{
//		if ((itm = (*itv).find("location")) != (*itv).end())
//			if (itm->second == folder)
//				return (&(*itv));
//	}
//	return (nullptr);
//}
//
//
//
//
//// Print all content of a location | ex: s0->printAllLocation();;
//void
//Config::printAllLocation()
//{
//	std::vector<std::map<std::string, std::string> >::iterator itv;
//	std::map<std::string, std::string>::iterator itm;
//
//	for (itv = _locs.begin(); itv != _locs.end(); itv++)
//	{
//		for (itm = (*itv).begin(); itm != (*itv).end(); itm++)
//			std::cout <<G<< itm->first << " | "<<RE<<Y<< itm->second <<RE<<  std::endl;
//	}
//}
//// Print all content of a location | ex: s0->printAllContentsLocation("/methods");
//bool
//Config::printAllContentsLocation(std::string pathLocation) {
//
//	std::cout <<W<< "@fn Config::printAllContentsLocation(std::string pathLocation)"<<RE<< std::endl;
//	std::vector<std::map<std::string, std::string> >::iterator itv;
//	std::map<std::string, std::string>::iterator itm;
//
//	for (itv = _locs.begin(); itv != _locs.end(); itv++) {
//
//		itm = (*itv).find("location");
//		if (itm != (*itv).end()) {
//			if (itm->second == pathLocation) {
//
//				std::cout << G << \
//				"Location path: '" <<RE<<Y<< pathLocation <<RE<<G<< "' found" \
//				<<RE<< std::endl;
//
//				std::cout << G << \
//				"Location Key: '" << itm->first <<RE<<Y<< "' \t | Value: '" << itm->second \
//				<<RE<< std::endl;
//
//				std::cout << G << \
//				"Content Location '" \
//				<<RE<< std::endl;
//
//				for (itm = (*itv).begin(); itm != (*itv).end(); itm++) {
//					std::cout << \
//                    "Key : '" << itm->first <<RE<<Y<< "' \t | " \
//                    "Value: '" << itm->second << "'" <<RE<< std::endl;
//				}
//			}
//		}
//		else {
//			std::cerr <<R<< "Location path: '" <<RE<<Y<< pathLocation <<RE<<R<< "' not found" << std::endl;
//			return (false);
//		}
//	}
//	return(true);
//}
