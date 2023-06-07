//
// Created by Lowell Zima on 3/23/23.
//

#include "../../includes/Webserv_Includes.hpp"



/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

Config::Config()
{
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

Config::~Config() 	{ }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

void Config::setNameServer		(std::string name) 										{ this->_name = name; }
void Config::setMimeMap			(std::map<std::string,std::string> mime_type)			{ this->_mime_types = mime_type; }
void Config::setErrorPagesMap	(std::map<int, std::string> error_pages)				{ this->_error_pages = error_pages; }
void Config::setErrorNamesMap	(std::map<int, std::string> error_names)				{ this->_error_names = error_names; }
void Config::setBinCgi			(const std::string &binCgi) 							{ this->_binCGI = binCgi; }
void Config::setHost			(const std::string &host) 								{ this->_host = host; }

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

void
Config::setBodyLength(std::string input)
{
	getOnlyChar(input);
	if (is_number(input)) {
		_bodylength = std::atoi(input.c_str());
	}
	else
		throw std::invalid_argument("@fn void\n"
									"Config::setBodyLength(std::string input)\n"
									"Not only number in client_max_body_size variable in Server");
	if (input.empty())
		throw std::invalid_argument("@fn void\n"
									"Config::setBodyLength(std::string input)\n"
									"No client_max_body_size variable in Server");
}

int Config::getBodyLength() const
{
	return (_bodylength);
}

void
Config::setUploadFolder(std::string input)
{
	_uploadfolder = input.c_str();
}

std::string Config::getUploadFolder() const
{
	return (_uploadfolder);
}



void
Config::setPathToFile (std::string &URIraw, std::map<std::string, std::string> *singleLocationContent)
{

	std::string minusfolder = this->eraseFolderLocationAndSlashFromURI(URIraw, *singleLocationContent);
	_path_to_file = this->getRoot(*singleLocationContent) + minusfolder;

}


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

std::string 										&Config::getNameServer(void) 			{ return(this->_name); }
std::vector<int> 									&Config::getPortServer(void) 			{ return(_ports); }
int 												&Config::getAmountPortServer(void) 		{ return(_amount_ports); }
std::vector<std::map<std::string, std::string> > 	&Config::getAllLocations(void)			{ return (_locs); }
const std::string 									&Config::getPath() const 				{ return (_path_to_file); }
const std::string 									&Config::getHost()		 				{ return (_host); }


/*weirdo getter*/

const std::string&
Config::getBinCgi() const
{
	if (_binCGI.empty())
		throw std::invalid_argument("@fn const std::string&\n"
									"Config::getBinCgi() const \n"
									"binCGI empty\n");
	return (_binCGI);
}

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
		return ("application/octet-stream"); // "default" type of MIME : just somes bytes
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
		std::cout << R << "getErrorPages: " << error_pages << " not found.\nThat page will be generated by the server\n" << RE << std::endl;
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
		return ("");
	}
	else {
		to_open.close();
		return (PathToFile);
	}
}

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
