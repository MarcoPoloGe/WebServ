//
// Created by Lowell Zima on 3/23/23.
//

#ifndef CONFIG_PARSER_PARSER_HPP
#define CONFIG_PARSER_PARSER_HPP

//COLORS
//#define	B		"\033[1m\033[30m" 		// Bold Black
//#define R		"\033[1m\033[31m"		// Bold Red
//#define G		"\033[1m\033[32m"		// Bold Green
//#define Y		"\033[1m\033[33m"		// Bold Yellow
//#define W		"\033[1m\033[37m"		// Bold White
//#define C		"\033[36m"
//#define	RE   	"\033[0m"


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
#include "ClassResponse.hpp"


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
	setServerConfig(std::vector<std::string> 							config);
	void
	setAllLocation(std::vector<std::map<std::string, std::string> > 	locs);
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


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string 												&getNameServer();
	std::string 												&getIpServer();
	std::vector<int> 											&getPortServer();
	int			 												&getAmountPortServer();
	std::vector<std::string> 									&getAllServerConfig ();
	std::vector<std::map<std::string, std::string> >			&getAllLocations();
	std::map<std::string,std::string>							&getMimeMap();
	std::map<int,std::string>									&getErrorPagesMap();
	std::map<int,std::string>									&getErrorNamesMap();



	std::string 												&getKeyTemp();
	std::string 												&getValueTemp();


	std::string													getType(const std::string& format);
	std::string 												getContentType(const std::string& format);
	std::string													getErrorPages(int error_pages);
	std::string													getErrorNames(int error_names);


	Response 													IsLocation(const std::string& URIraw, const std::string &Method);

	/* weirdo getters */

//	s0->getInLocationValue("/methods", "root");
	bool
	getInLocationValue(std::string key, std::string LocationPath);

	std::string
	getPath_of_URI(const std::string& URIraw,
				   std::vector<std::map<std::string, std::string> >::iterator &i,
				   std::map<std::string, std::string>::iterator &im);


/**********************************************************************************************************************/
/***************************                       Private Vars		                           ************************/
/**********************************************************************************************************************/

private:
	std::string														_name;
	std::string														_ip;
	std::vector<int>												_ports;
	int																_amount_ports;
	std::vector<std::string> 										_serverconfig;
	std::vector<std::string> 										_rawfile;
	std::vector<std::map<std::string, std::string> > 				_locs;
	std::map<std::string, std::string>								_mime_types;
	std::map<int, std::string>										_error_pages;
	std::map<int, std::string>										_error_names;

	std::string														_key_temp;
	std::string														_value_temp;
//	std::map<std::string, std::string>								_loc_temp;

/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

public:
//	s0->printAllContentsLocation("/methods");
	bool
	printAllContentsLocation(std::string pathLocation);

//	s0->printAllLocation();
	void
	printAllLocation();

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

void
printVector(std::vector<std::string> &x );

void
getOnlyChar(std::string &s);

void
insert_mime_type(
		std::string input, std::map<std::string, std::string> &mime_types);

std::map<std::string, std::string>
save_mime_type(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket);


void
insert_error(
		std::string input,
		std::map<std::string, std::string> &error);

void
save_error(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket,
		std::map<std::string, std::string> &error);


#endif //CONFIG_PARSER_PARSER_HPP
