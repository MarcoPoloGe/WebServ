//
// Created by Lowell Zima on 3/23/23.
//

#ifndef MAIN_PARSING_HPP
#define MAIN_PARSING_HPP

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

class Config;

void
main_parsing(char **av, std::vector<Config> &all_config);

void
main_parsing(std::string fileName, std::vector<Config> &all_config);

bool
serverConfig(
		std::vector<std::string> &stock,
		std::vector<std::string>::iterator it,
		std::vector<Config> &all_config);

Config &
setUpServer(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket,
		Config &s);

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

void
insert_mime_type(
		std::string input, std::map<std::string, std::string> &mime_types);

std::map<std::string, std::string>
save_mime_type(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket);

/**********************************************************************************************************************/
/***************************                      Utils		               ************************/
/**********************************************************************************************************************/

std::string
AfterEqual (std::string &input);

void
getOnlyChar(std::string &s);



#endif //CONFIG_PARSER_PARSER_HPP
