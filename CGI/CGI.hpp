//
// Created by Lowell Zima on 4/12/23.
//

#ifndef R5_WEBSERV_CGI_HPP
#define R5_WEBSERV_CGI_HPP


//HOMEMADE
#include "../includes/Webserv_Includes.hpp"

class CGI {

public:
/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

	CGI();
	CGI( Request &Request, Network &Network );
	~CGI();


/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/

	const std::string *setENV();
	void setENV(Request &req, Network &net, Config &conf);



/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

	std::string getBin();
	int getenv ();


/**********************************************************************************************************************/
/***************************                       Private Vars		                           ************************/
/**********************************************************************************************************************/

private:
	std::string _bin;
	std::map<std::string, std::string> ENV;

};




#endif //R5_WEBSERV_CGI_HPP
