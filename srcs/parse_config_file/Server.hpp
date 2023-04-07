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
//	void setLocation(std::string			  								loc);

/* weirdo setters */
	void setRawfile(std::vector<std::string>::iterator first_bracket, std::vector<std::string>::iterator last_bracket);


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string 												&getNameServer(void);
	std::string 												&getIpServer(void);
	std::string 												&getPortServer(void);
	std::vector<std::string> 									&getAllServerConfig (void);
	std::vector<std::map<std::string, std::string> >			&getAllLocations(void);

	// getInLocationValue("/", "root" ) = ./www/
	bool getInLocationValue(std::string key, std::string LocationpPath);
	std::string &getKeyTemp(void);
	std::string &getValueTemp(void);




/**********************************************************************************************************************/
/***************************                       Vars		            		               ************************/
/**********************************************************************************************************************/

private:
	std::string														_name;
	std::string														_ip;
	std::string														_port;
	std::vector<std::string> 										_serverconfig;
	std::vector<std::string> 										_rawfile;
	std::vector<std::map<std::string, std::string> > 				_locs;

public:
	std::string														_key_temp;
	std::string														_value_temp;

//	std::map<std::string, std::string>::iterator					ItMap_temp;
//	std::map<std::string, std::string>								temp_map;

//	std::vector<std::map<std::string, std::string> >::iterator		Vec_Map_temp;


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

public:

	bool printMap(std::string s);
	void printAllMap();

};

void main_parsing(char **av, std::vector<Server> &all_server);



#endif //CONFIG_PARSER_PARSER_HPP
