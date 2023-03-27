//
// Created by Lowell Zima on 3/23/23.
//

#ifndef CONFIG_PARSER_PARSER_HPP
#define CONFIG_PARSER_PARSER_HPP

//COLORS
#define	B		"\033[1m\033[30m" 		// Bold Black
#define R		"\033[1m\033[31m"		// Bold Red
#define G		"\033[1m\033[32m"		// Bold Green
#define Y		"\033[1m\033[33m"		// Bold Yellow
#define W		"\033[1m\033[37m"		// Bold White
#define	RE   	"\033[0m"

//STD
#include <iostream>
#include <fstream>
#include <string>

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

	void setNameServer(std::string								name);
	void setIpServer(std::string								ip);
	void setPortServer(std::string								port);
	void setServerConfig(std::vector<std::string> 				config);
	void setAllLocation(std::vector<std::vector<std::string> > 	locs);
	void setLocation(std::string			  					loc);
	void setRawfile(std::vector<std::string> 					rawfile);


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string 									&getNameServer(void);
	std::string 									&getIpServer(void);
	std::string 									&getPortServer(void);
	std::vector<std::string> 						&getAllServerConfig (void);
	std::vector<std::vector<std::string> > 			&getAllLocations(void);



/**********************************************************************************************************************/
/***************************                       Vars		            		               ************************/
/**********************************************************************************************************************/

private:
	std::string								_name;
	std::string								_ip;
	std::string								_port;
	std::vector<std::string> 				_serverconfig;
	std::vector<std::string> 				_rawfile;
	std::vector<std::vector<std::string> > 	_locs;


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

public:
	std::string								searchValue(std::string key, std::vector<std::string> vect_source);
	std::string 							returnAfterEqual(std::string key, std::string source);
	std::vector<std::string>  				&getSpecificLoc(std::string way, Server s)	;

};


#endif //CONFIG_PARSER_PARSER_HPP
