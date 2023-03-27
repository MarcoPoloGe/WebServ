//
// Created by Lowell Zima on 3/22/23.
//

#include "Parser.hpp"


// Utils
void Read_and_Stock_FileName(const std::string& fileName, std::vector<std::string> &stock) {
	std::string line;
	std::ifstream file;

	// Try open file
	file.open(fileName.c_str());
	if (!file.is_open() || file.bad())
		throw std::bad_exception();

	// Read line by line
	while (std::getline(file, line))
		stock.push_back(line);
	file.close();
}

// Class Fonctions
Parser::Parser() {
	std::cout <<W<< "++parser" <<RE<< std::endl;
}

 Parser::Parser(std::string fileName) {
	std::cout <<W<< "++parser + fileName" <<RE<< std::endl;
	 try {
		 Read_and_Stock_FileName(fileName, _stock);
	 }
	 catch (...){
		 std::cerr << "Error: readFileName" << std::endl;
	 }
}
Parser::~Parser() {
	std::cout <<W<< "--parser" <<RE<< std::endl;
}

std::vector<std::string> &Parser::getStock() {
	return _stock;
}
