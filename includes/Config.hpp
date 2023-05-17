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
//	void
//	setServerConfig(std::vector<std::string> 							config);
//	void
//	setAllLocation(std::vector<std::map<std::string, std::string> > 	locs);
	void
	setKeyTemp(std::string 												key_temp);
	void
	setValueTemp(std::string 											value_temp);
	void
	setMimeMap(std::map<std::string,std::string>						mime_type);
	void
	setErrorPagesMap(std::map<int,std::string>							error_pages);
	void
	setErrorNamesMap(std::map<int,std::string>							error_names);




/* weirdo setters */
	void
	setRawfile(std::vector<std::string>::iterator first_bracket, std::vector<std::string>::iterator last_bracket);

	void
	setPathToFile (std::string &URIraw, std::map<std::string, std::string> *singleLocationContent);


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

//	std::string 												&getNameServer();
	std::string 												&getIpServer();
	std::vector<int> 											&getPortServer();
	int			 												&getAmountPortServer();
//	std::vector<std::string> 									&getAllServerConfig ();
	std::vector<std::map<std::string, std::string> >			&getAllLocations();
//	std::map<std::string,std::string>							&getMimeMap();
//	std::map<int,std::string>									&getErrorPagesMap();
//	std::map<int,std::string>									&getErrorNamesMap();

	const std::string 											&getPath() const;

//	std::string 												&getKeyTemp();
//	std::string 												&getValueTemp();


	std::string													getType(const std::string& format);
//	std::string 												getContentType(const std::string& format);
	std::string													getErrorPages(int error_pages);
	std::string													getErrorNames(int error_names);




//	Response 													IsLocation(const std::string& URIraw, const std::string &Method);

	std::string													getDefaultErrorDescription(int error);

	/* weirdo getters */

//	s0->getInLocationValue("/methods", "root");
//	bool
//	getInLocationValue(std::string key, std::string LocationPath);
//
//	std::map<std::string, std::string> *
//	getLocationFolderContent(const std::string &folder);


	std::string
	getPath_of_URI(const std::string& URIraw,
				   std::vector<std::map<std::string, std::string> >::iterator &i,
				   std::map<std::string, std::string>::iterator &im);


	std::string
	getRoot(std::map<std::string,
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

//	std::string
//	getFileFromURI(std::string URIraw);

//	std::string
//	getPathToFile(std::string URIraw, std::map<std::string, std::string> singleLocationContent);
//
//	std::string
//	getFileInFolderFromURI(std::string URIraw, std::string Folder);



	std::string
	isPathToFile(const std::string &PathToFile);

	bool
	IsMethodAllowed(std::string Method, std::map<std::string, std::string> &singleMapLocation);

//	bool
//	isOnlyFolder(const std::string &folder, const std::string &URIraw);

/**********************************************************************************************************************/
/***************************                       Private Vars		                           ************************/
/**********************************************************************************************************************/

private:
	std::string														_name;
	std::string														_ip;
	std::string														_binCGI;
public:
	void setBinCgi(const std::string &binCgi);

public:
	const std::string &getBinCgi() const;

private:
	std::vector<int>												_ports;
	int																_amount_ports;
	std::vector<std::string> 										_serverconfig;
	std::vector<std::string> 										_rawfile;
	std::vector<std::map<std::string, std::string> > 				_locs;
	std::map<std::string, std::string>								_mime_types;
	std::map<int, std::string>										_error_pages;
	std::map<int, std::string>										_error_names;

	std::string 													_path_to_file;

	std::string														_root;

	std::string														_key_temp;
	std::string														_value_temp;


	std::map<int, std::string>										_DefaultErrorMap;

/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

public:
//	s0->printAllContentsLocation("/methods");
//	bool
//	printAllContentsLocation(std::string pathLocation);

//	s0->printAllLocation();
//	void
//	printAllLocation();

//	bool
//	checkAutoIndex(std::map<std::string, std::string> *singleLocationContent);

	std::string
	eraseFolderLocationAndSlashFromURI(std::string URIraw, std::map<std::string, std::string> &singleMapLocation);



};

/*↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ ↓ */
/***************************                       main_parsing.cpp fn	    		           ************************/
/**********************************************************************************************************************/

void
main_parsing(char **av, std::vector<Config> &all_config);


/**********************************************************************************************************************/
/***************************            Server Config: parsing all config file          	   ************************/
/**********************************************************************************************************************/

bool
serverConfig(
		std::vector<std::string> &stock,
		std::vector<std::string>::iterator it,
		std::vector<Config> &all_config);


/**********************************************************************************************************************/
/***************************                  Set Up one Server config	      		           ************************/
/**********************************************************************************************************************/

Config &
setUpServer(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket,
		Config &s);


/**********************************************************************************************************************/
/***************************                   Get infos from one location {}     	           ************************/
/**********************************************************************************************************************/

void
insertMap_split_by_Delimiter(
		std::map<std::string,
				std::string> &map,
		const std::string& input,
		const std::string& delimiter);

std::vector<std::string>::iterator
grabLocation (
		std::vector<std::string>::iterator 	it,
		std::vector<std::string>::iterator 	last_bracket,
		Config &s);


/**********************************************************************************************************************/
/***************************                       main_parsing.cpp Utils		               ************************/
/**********************************************************************************************************************/

std::string
AfterEqual (std::string &input);

//void
//printVector(std::vector<std::string> &x );

void
getOnlyChar(std::string &s);

void
insert_mime_type(
		std::string input, std::map<std::string, std::string> &mime_types);

std::map<std::string, std::string>
save_mime_type(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket);


//void
//insert_error(
//		std::string input,
//		std::map<std::string, std::string> &error);

//void
//save_error(
//		std::vector<std::string>::iterator 	first_bracket,
//		std::vector<std::string>::iterator 	last_bracket,
//		std::map<std::string, std::string> &error);


#endif //CONFIG_PARSER_PARSER_HPP
