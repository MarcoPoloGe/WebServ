//
// Created by Lowell Zima on 3/23/23.
//

#include "Server.hpp"
#include <vector>

/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/


Server::Server() 	{ std::cout <<W<< "++server" <<RE<< std::endl; }
Server::~Server() 	{ std::cout <<W<< "--server" <<RE<< std::endl; }

/**********************************************************************************************************************/
/***************************                       Setters	            		               ************************/
/**********************************************************************************************************************/


void Server::setNameServer		(std::string name) 										{ this->_name = name; }
void Server::setIpServer		(std::string ip)				 						{ this->_ip = ip; }
void Server::setPortServer		(std::string port)										{ this->_port = port; }
void Server::setServerConfig	(std::vector<std::string> config)						{ this->_serverconfig = config; }
void Server::setAllLocation		(std::vector<std::map<std::string, std::string> > locs)	{ this->_locs = locs; }

/* weirdo setters */
void Server::setRawfile	(
		std::vector<std::string>::iterator first_bracket,
		std::vector<std::string>::iterator last_bracket)
{
	while (first_bracket != last_bracket)
	{
		this->_rawfile.push_back(*first_bracket);
		first_bracket++;
	}
}


//todo
//std::vector<std::string>::iterator meth = getLocation("/methods");

//void Server::setLocPath(std::vector<std::string>::iterator			loc)				{
//	std::vector<std::vector<std::string> >::iterator	it_locs = getAllLocation().begin();
//	while(this->)
//}


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

std::string 										&Server::getNameServer(void) 			{ return(this->_name); }
std::string 										&Server::getIpServer(void) 				{ return(_ip); }
std::string 										&Server::getPortServer(void) 			{ return(_port); }
std::vector<std::string> 							&Server::getAllServerConfig (void)		{ return (_serverconfig); }
std::vector<std::map<std::string, std::string> > 	&Server::getAllLocations(void)			{ return (_locs); }


int Server::getLocationPath(std::string locationPath, std::vector<std::map<std::string, std::string> >::iterator itv)
{
	std::map<std::string, std::string>::iterator itm;

	itm = (*itv).find(locationPath);
	if (itm != (*itv).end()) {
		ItMap_temp = itm;
		Vec_Map_temp = itv;
		return (1);
	}
	else {

		ItMap_temp = Map_empty.begin();
	}
	return (0);
}

//std::vector<std::map<std::string, std::string> >::iterator Server::getLocationMap(std::map<std::string, std::string>::iterator itmap)
//{
//
//}

//todo
//std::vector<std::vector<std::string> >::iterator 	&Server::getAllLocIt(void)				{ return (_locs.begin()); }



/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/


void Server::printAllMap()
{
	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;
	for (itv = _locs.begin(); itv != _locs.end(); itv++)
	{
		for (itm = (*itv).begin(); itm != (*itv).end(); itm++)
			std::cout <<G<< itm->first << " | "<< itm->second << RE <<  std::endl;
		std::cout << "----------------------------------" << std::endl;
	}
}

void Server::printMap(std::string s) {

	std::vector<std::map<std::string, std::string> >::iterator itv;
	std::map<std::string, std::string>::iterator itm;

	for (itv = _locs.begin(); itv != _locs.end(); itv++) {

		itm = (*itv).find("location");
		if (itm != (*itv).end()) {
			if (itm->second == s) {
				std::cout << G << "Location path: '" << RE << Y << s << RE << G << "' found" << std::endl;
				std::cout << G << "Location Key: '" << itm->first << RE << Y << "' \t | Value: '" << itm->second <<RE<< std::endl;
				std::cout << G << "Content Location '" <<RE << std::endl;
				for (itm = (*itv).begin(); itm != (*itv).end(); itm++)
					std::cout << itm->first << RE << Y << "' \t | Value: '" << itm->second <<RE<< std::endl;
			}
		}
		else {
			std::cerr << R << "Location path: '" << RE << Y << s << RE << R << "' not found" << std::endl;
		}
	}
}
//	std::map<std::string, std::string>::iterator i;
//	for (i = it; i != (*map).end(); i++)
//		std::cout <<G<< "Key : \t'" << it->first <<RE<<Y<< "'\t\t| Value: \t" << it->second <<RE<< std::endl;


//std::string	Server::searchValue (
//		std::string key,
//		std::vector<std::string> vect_source)
//{
//	std::vector<std::string>::iterator start = vect_source.begin();
//	std::vector<std::string>::iterator end = vect_source.end();
//	while(start != end) {
//		if (((*start).find(key))!= std::string::npos)
//			return((*start).substr(((*start).find("=")) + 1, (*start).length()));
//		start++;
//	}
//	std::cerr <<R<< "searchValue: Keyword " << key << "not found in source " <<RE<< std::endl;
//	return(NULL);
//}

//std::string Server::returnAfterEqual (
//		std::string key,
//		std::string source)
//{
//	std::string ret;
//	if ((source.find(key))!= std::string::npos)
//		{
//			ret = source.substr((source.find("=")) + 1, source.size());
//			return(ret);
//		}
//	std::cerr <<R<< "returnAfterEqual: Keyword " << key << "not found in source " <<RE<< std::endl;
//	return(ret);
//}
