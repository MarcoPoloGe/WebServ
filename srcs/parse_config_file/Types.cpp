//
// Created by Lowell Zima on 4/11/23.
//

#include "Types.hpp"

Types::Types() {
	std::cout << "++Types" << std::endl;
}

Types::~Types() {
	std::cout << "--Types" << std::endl;
}

std::string Types::getPathFormat(std::string format) {
	std::map<std::string, std::string>::iterator itm;
	itm = mime_types.find(format);
	if (itm != mime_types.end()){
		return (itm->second);
	}
	else
		std::cerr<<R<< "Error: getPathFormat: "<< format << " not found." <<RE<< std::endl;
	return (format);
}


/**********************************************************************************************************************/
/***************************                  Set Up mime_type vars		      		           ************************/
/**********************************************************************************************************************/

void
Types::insert_mime_type(
		std::string input)
{
	std::string tmp_keys;
	std::string key;
	std::string value;

	unsigned long pos;
	unsigned long start;
	unsigned long end;

	if((pos = input.find("=")) != std::string::npos)
	{
		// Get value | ex: text/html
		value = input.substr(0, pos);
		getOnlyChar(value);
		input.erase(0, pos + 1);
		// Get key | ex: .css
		while ((start = input.find(".")) != std::string::npos)
		{
			// erase .
			input.erase(start, 1);
			// ex: .html.htm.shtml -> insert html=text/html | htm=text/html | shtml=text/html
			if ((end = input.find(".")) != std::string::npos){
				key = input.substr(start, end - start );
				getOnlyChar(key);
				/*print*/
//				std::cout <<G<< "mime_type->key = " << key << " | value = " << value <<RE<< std::endl;
				mime_types.insert(std::pair<std::string, std::string>(key, value));
				input.erase(start, end);
			}
				// ex: .css -> insert css=text/css
			else
			{
				key = input;
				getOnlyChar(key);
//				std::cout <<G<< "mime_type->key = " << key << " | value = " << value <<RE<< std::endl;
				mime_types.insert(std::pair<std::string, std::string>(key, value));
			}
		}
	}
	else {
		std::cerr<<R<< "Error: @fn insert_mime_type \nDelimiter '" << "=" << "' not found in " << input <<RE<< std::endl;
	}
}

bool
Types::save_mime_type(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket)
{
	while (first_bracket != last_bracket)
	{
		if (((*first_bracket).find("="))!= std::string::npos)
			insert_mime_type(*first_bracket);
		first_bracket++;
	}
	std::cout <<G<< "mime_types sucessfully saved " <<RE<< std::endl;
	return (true);
}

