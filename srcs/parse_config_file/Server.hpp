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
#include "../../includes/Webserv_Includes.hpp"
#include "Types.hpp"

class Types;

//STD
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

// Containers
#include <vector>
#include <map>


// Single Server config
class Server {
public:
/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

	Server();
	~Server();


/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

	void setNameServer(std::string											name);
	void setIpServer(std::string											ip);
	void setPortServer(std::string											port);
	void setServerConfig(std::vector<std::string> 							config);
	void setAllLocation(std::vector<std::map<std::string, std::string> > 	locs);
	void setKeyTemp(std::string 											key_temp);
	void setValueTemp(std::string 											value_temp);

/* weirdo setters */
	void setRawfile(std::vector<std::string>::iterator first_bracket, std::vector<std::string>::iterator last_bracket);


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string 												&getNameServer();
	std::string 												&getIpServer();
	std::string 												&getPortServer();
	std::vector<std::string> 									&getAllServerConfig ();
	std::vector<std::map<std::string, std::string> >			&getAllLocations();

	std::string 												&getKeyTemp();
	std::string 												&getValueTemp();

/* weirdo getters */
	// getInLocationValue("/", "root" ) = ./www/
	bool getInLocationValue(std::string key, std::string LocationPath);


/**********************************************************************************************************************/
/***************************                       Private Vars		                           ************************/
/**********************************************************************************************************************/

private:
	std::string														_name;
	std::string														_ip;
	std::string														_port;
	std::vector<std::string> 										_serverconfig;
	std::vector<std::string> 										_rawfile;
	std::vector<std::map<std::string, std::string> > 				_locs;

	std::string														_key_temp;
	std::string														_value_temp;



/**********************************************************************************************************************/
/***************************                       Public Vars		            	           ************************/
/**********************************************************************************************************************/

public:
	std::map<std::string, std::string>								mime_types;


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

public:

	bool
	printAllContentsLocation(std::string pathLocation);
	void
	printAllLocation();

};
void
main_parsing(char **av, std::vector<Server> &all_server, Types &t);

void
getOnlyChar(std::string &s);

void
printVector(std::vector<std::string> &x );

std::string
AfterEqual (std::string &input);

#endif //CONFIG_PARSER_PARSER_HPP
