//
// Created by Lowell Zima on 3/23/23.
//

#ifndef CONFIG_PARSER_SERVER_HPP
#define CONFIG_PARSER_SERVER_HPP
#include "Parser.hpp"
#include "Server.hpp"



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

	void setNameServer(std::string								name) 				{ this->_name = name; };
	void setIpServer(std::string								ip) 				{ this->_ip = ip; };
	void setPortServer(std::string								port);
	void setServerConfig(std::vector<std::string> 				config)				{ this->_serverconfig = config; };
	void setAllLocation(std::vector<std::vector<std::string> > 	locs)				{ this->_locs = locs; };
//	void setLocation(std::string			  					loc)				{ _locs = loc); };


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string 									&getNameServer(void) 			{ return(_name); };
	std::string 									&getIpServer(void) 				{ return(_ip); };
	std::string 									&getPortServer(void) 			{ return(_port); };
	std::vector<std::string> 						&getAllServerConfig (void)		{ return (_serverconfig); };
	std::vector<std::vector<std::string> > 			&getAllLocations(void)			{ return (_locs); };



/**********************************************************************************************************************/
/***************************                       Vars		            		               ************************/
/**********************************************************************************************************************/

private:
	std::string								_name;
	std::string								_ip;
	std::string								_port;
	std::vector<std::string> 				_serverconfig;
	std::vector<std::vector<std::string> > 	_locs;


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

public:
	std::string								searchValue(std::string key, std::vector<std::string> vect_source);
	std::string 							&returnAfterEqual(std::string key, std::string source);
	std::vector<std::string>  				&getSpecificLoc(std::string way, Server s)	;

};


#endif //CONFIG_PARSER_SERVER_HPP
