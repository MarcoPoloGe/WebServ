//
// Created by Lowell Zima on 4/11/23.
//

#ifndef R5_WEBSERV_TYPES_HPP
#define R5_WEBSERV_TYPES_HPP

# include <map>
# include <iostream>
# include <vector>
# include "Server.hpp"
//# include "../../includes/Webserv_Includes.hpp"


class Types {
public:

	Types();
	~Types();

	std::map<std::string, std::string>								mime_types;

	std::string
	getPathFormat(std::string format);

	void
	insert_mime_type(std::string input);

	bool
	save_mime_type(
			std::vector<std::string>::iterator 	first_bracket,
			std::vector<std::string>::iterator 	last_bracket);


};


#endif //R5_WEBSERV_TYPES_HPP
