//
// Created by Lowell Zima on 4/12/23.
//

#ifndef R5_WEBSERV_CGI_HPP
#define R5_WEBSERV_CGI_HPP


//HOMEMADE
#include "Webserv_Includes.hpp"

class CGI {

public:
/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

	CGI();
	~CGI();

/**********************************************************************************************************************/
/***************************                       Utils		                           ************************/
/**********************************************************************************************************************/

public:
	static std::string execute(Request &request, Config &conf, int port, const std::string &p);
	static std::string CGIstore(const std::string &p);
	static std::map<std::string, std::string> 		setUpEnvVariablesCGI(Request &request, Config &conf, int port);

};


#endif //R5_WEBSERV_CGI_HPP
