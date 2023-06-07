//
// Created by Lowell Zima on 3/23/23.
//

#ifndef CONFIG_PARSER_PARSER_HPP
#define CONFIG_PARSER_PARSER_HPP

//HOMEMADE
class Types;
class Response;

//STD
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Containers
#include <vector>
#include <map>
#include "Response.hpp"

#define AUTOINDEX_TRUE 1
#define AUTOINDEX_FALSE 2


// Single Server config
class Config {
public:
/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

	Config();
	~Config();

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

	void
	setNameServer(std::string											name);
	void
	setIpServer(std::string												ip);
	void
	setPortServer(std::string&											port);
	void
	setBodyLength(std::string											bodylength);
	void
	setUploadFolder(std::string											input);
	void
	setMimeMap(std::map<std::string,std::string>						mime_type);
	void
	setErrorPagesMap(std::map<int,std::string>							error_pages);
	void
	setErrorNamesMap(std::map<int,std::string>							error_names);
	void
	setHost(const std::string &											host);

/* weirdo setters */
	void
	setRawfile(std::vector<std::string>::iterator first_bracket, std::vector<std::string>::iterator last_bracket);

	void
	setPathToFile (std::string &URIraw, std::map<std::string, std::string> *singleLocationContent);

	void
	setBinCgi(const std::string &binCgi);


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string 												&getNameServer();
	std::string 												&getIpServer();
	std::vector<int> 											&getPortServer();
	int			 												&getAmountPortServer();
	std::vector<std::map<std::string, std::string> >			&getAllLocations();
	const std::string 											&getPath() const;
	std::string													getType(const std::string& format);
	std::string													getErrorPages(int error_pages);
	std::string													getErrorNames(int error_names);
	std::string													getDefaultErrorDescription(int error);
	const std::string 											&getHost();


	/* weirdo getters */
	std::string
	getPath_of_URI(const std::string& URIraw,
				   std::vector<std::map<std::string, std::string> >::iterator &i,
				   std::map<std::string, std::string>::iterator &im);

	std::string
	getRoot(std::map<std::string,
			std::string> &singleMapLocation );

	std::string
	getReturn(std::map<std::string,
			std::string> &singleMapLocation );

	std::string
	getMethod(std::map<std::string,
			std::string> &singleMapLocation );

	std::string
	getDefault(std::map<std::string,
			std::string> &singleMapLocation );

	std::string
	getAutoindex(std::map<std::string,
			std::string> &singleMapLocation );

		std::string
	getLocation(std::map<std::string,
			std::string> &singleMapLocation );

	std::map<std::string, std::string> *
	getSingleMapLocation(const std::string &folderLocation);

	std::string
	getFolderLocationFromURI(std::string URIraw);

	static std::string
	getVarLocation(std::map<std::string,
				   std::string> &singleMapLocation,
				   const std::string& var );

	int
	getBodyLength() const;

	std::string
	getUploadFolder() const;

	const std::string &
	getBinCgi() const;


/**********************************************************************************************************************/
/***************************                       Utils Vars		                           ************************/
/**********************************************************************************************************************/

	std::string
	isPathToFile(const std::string &PathToFile);

	bool
	IsMethodAllowed(std::string Method, std::map<std::string, std::string> &singleMapLocation);

	std::string
	eraseFolderLocationAndSlashFromURI(std::string URIraw, std::map<std::string, std::string> &singleMapLocation);


/**********************************************************************************************************************/
/***************************                       Private Vars		                           ************************/
/**********************************************************************************************************************/

private:
	std::string														_name;				// could be empty
	std::string														_host;				// could be empty
	std::string														_binCGI;			//
	int																_bodylength;
	std::string														_uploadfolder;
	std::vector<int>												_ports; 			// protected in @fn Config::setPortServer
	int																_amount_ports;		//
	std::vector<std::string> 										_serverconfig;		//
	std::vector<std::string> 										_rawfile;			//
	std::vector<std::map<std::string, std::string> > 				_locs;				// protected in @fn grabLocation
	std::map<std::string, std::string>								_mime_types;		// protected in @fn save_mime_type
 	std::map<int, std::string>										_error_pages;		// not protected for incorrect input
	std::map<int, std::string>										_error_names;
	std::string 													_path_to_file;		//protected
	std::string														_root;				//protected with no '/' at the end and empty
	std::map<int, std::string>										_DefaultErrorMap;

};

#endif //CONFIG_PARSER_PARSER_HPP
