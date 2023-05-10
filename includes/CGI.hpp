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
	std::string 									execute(Request &request, Response &rep, Config &conf, int port);
	std::string 									CGIstore(Response &rep);
	static std::map<std::string, std::string> 		setUpEnvVariablesCGI(Request &request, Config &conf, int port);
	bool											check(Request &r);

};

//	if (cgi.check(request))
//		cgi.execute(request, response, _config, _port);


#endif //R5_WEBSERV_CGI_HPP
