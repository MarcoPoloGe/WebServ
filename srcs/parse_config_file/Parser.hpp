//
// Created by Lowell Zima on 3/22/23.
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

// Class
#include "Server.hpp"

//Main Class

class Parser {
public:

	Parser();

	explicit Parser(std::string fileName);

	~Parser();

	Parser(const Parser &p);

	Parser &operator = (const Parser &lhs);

	bool operator==(const Parser &rhs) const;

	bool operator!=(const Parser &rhs) const;

	std::vector<std::string> &getStock();

protected:
	std::vector<std::string> _stock;


};

//void Read_and_Stock_FileName(const std::string& fileName, std::vector<std::string> &stock);

// Utils


#endif //CONFIG_PARSER_PARSER_HPP
