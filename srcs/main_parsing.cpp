#include "../includes/Webserv_Includes.hpp"

/**********************************************************************************************************************/
/***************************                       Main		            		           ************************/
/**********************************************************************************************************************/

void
main_parsing(
		char **av,
		std::vector<Config> &all_config)
{

	std::string fileName = av[1];
	std::vector<std::string> rawfile;
	std::cout <<W<< "Parsing config file" <<RE<< std::endl;

	std::string line;
	std::ifstream file;

	// Try open file
	file.open(fileName.c_str());
	if (!file.is_open() || file.bad())
		throw std::bad_exception();

	// Read line by line and stock in rawfile
	while (std::getline(file, line))
		rawfile.push_back(line);
	file.close();

	// Set up Servers from config.conf file in all_Config
	serverConfig(rawfile, rawfile.begin(), all_config);
}

void
main_parsing(
		std::string			fileName,
		std::vector<Config> &all_config)
{

	std::vector<std::string> rawfile;
	std::cout <<W<< "Parsing config file" <<RE<< std::endl;

	std::string line;
	std::ifstream file;

	// Try open file
	file.open(fileName.c_str());
	if (!file.is_open() || file.bad())
		throw std::bad_exception();

	// Read line by line and stock in rawfile
	while (std::getline(file, line))
		rawfile.push_back(line);
	file.close();

	// Set up Servers from config.conf file in all_Config
	serverConfig(rawfile, rawfile.begin(), all_config);
}



/**********************************************************************************************************************/
/***************************            Server Config: parsing all config file          	   ************************/
/**********************************************************************************************************************/


void
insert_error(
		std::string input,
		std::map<int, std::string> &error)
{
	std::string tmp_key;
	int key;
	std::string value;
	unsigned long pos;

	if((pos = input.find('=')) != std::string::npos)
	{
		tmp_key = input.substr(0, pos);
		getOnlyChar(tmp_key);
		key = std::atoi(tmp_key.c_str());
		input.erase(0, pos + 1);
		value = input;
		getOnlyChar(value);
//		std::cout <<G<< "error->key = " << key << " | value = " << value <<RE<< std::endl;
		error.insert(std::pair<int, std::string>(key, value));
	}
	else {
		std::cerr<<R<< "Error: @fn insert_error \nDelimiter '" << "=" << "' not found in " << input <<RE<< std::endl;
	}
}

void
save_error(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket,
		std::map<int, std::string> &error)
{
	std::map<std::string, std::string> mime_types;

	while (first_bracket != last_bracket)
	{
		if (((*first_bracket).find("="))!= std::string::npos)
			insert_error(*first_bracket, error);
		first_bracket++;
	}
//	std::cout <<G<< "error_* successfully saved " <<RE<< std::endl;
}

// In .conf vector, take first - server '{' - and launch parsing until the next corresponding '}'
bool
serverConfig(
		std::vector<std::string> &stock,
		std::vector<std::string>::iterator it,
		std::vector<Config> &all_config)
{
	std::vector<std::string>::iterator 					first_bracket;
	std::vector<std::string>::iterator 					last_bracket;

	std::vector<Config>::iterator 						vit;

	std::map<std::string,std::string> mime_type;
	std::map<int,std::string> error_pages_map;
	std::map<int,std::string> error_names_map;

	bool get_in = false;
	bool server = false;
	bool mimes = false;
	bool error_pages = false;
	bool error_names = false;

	int count = 0;


	for(it = stock.begin(); it != stock.end(); it++)
	{
		if (((*it).find("server"))!= std::string::npos)
			server = true;
		if (((*it).find("mime_type"))!= std::string::npos)
			mimes = true;
		if (((*it).find("error_pages"))!= std::string::npos)
			error_pages = true;
		if (((*it).find("error_names"))!= std::string::npos)
			error_names = true;
		if (((*it).find("{"))!= std::string::npos) {
			count += 1;
			if (count == 1) {
				first_bracket = it;
			}
		}
		else if (((*it).find("}")) != std::string::npos)
		{
			if (count == 1) {
				last_bracket = it + 1;
				get_in += 1;
			}
			count -= 1;
		}
		if (get_in && count == 0 && server)
		{
			Config s;
			s.setRawfile(first_bracket, last_bracket);
			setUpServer(first_bracket, last_bracket, s);
			all_config.push_back(s);
			get_in = false;
			server = false;
		}
		if (get_in && count == 0 && mimes)
		{
			mime_type =	save_mime_type(first_bracket, last_bracket);
			get_in = false;
			mimes = false;
		}
		if (get_in && count == 0 && error_pages)
		{
			save_error(first_bracket, last_bracket, error_pages_map);
			get_in = false;
			error_pages = false;
		}
		if (get_in && count == 0 && error_names)
		{
			save_error(first_bracket, last_bracket, error_names_map);
			get_in = false;
			error_names = false;
		}
	}

	if(mime_type.size() <= 0)
		throw std::invalid_argument("input file does not contain mime_types data.");
	for (std::vector<Config>::iterator itera = all_config.begin();
		 itera != all_config.end() ; itera++)
	{
		itera->setMimeMap(mime_type);
		itera->setErrorPagesMap(error_pages_map);
		itera->setErrorNamesMap(error_names_map);
	}

	return (true);
}




/**********************************************************************************************************************/
/***************************                  Set Up one Server config	      		           ************************/
/**********************************************************************************************************************/

// CQFD
Config &
setUpServer(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket,
		Config &s)
{
	std::vector<std::string> 				in;
	std::vector<std::string> 				server_config;
	std::vector<std::vector<std::string> > 	locations;
	std::vector<std::string> 				loc;
	std::string								path;

	while (first_bracket != last_bracket)
	{
		if (((*first_bracket).find("location"))!= std::string::npos)
		{
			first_bracket = grabLocation(first_bracket, last_bracket, s);
			if(((*first_bracket).find("}"))!= std::string::npos)
				first_bracket++;
			continue;
		}
		if (((*first_bracket).find("name="))!= std::string::npos)
			s.setNameServer(AfterEqual(*first_bracket));
		if (((*first_bracket).find("ip="))!= std::string::npos)
			s.setIpServer(AfterEqual(*first_bracket));
		if (((*first_bracket).find("cgibin="))!= std::string::npos)
			s.setBinCgi(AfterEqual(*first_bracket));
		if (((*first_bracket).find("client_max_body_size="))!= std::string::npos)
			s.setBodyLength(AfterEqual(*first_bracket));
		if (((*first_bracket).find("upload_folder="))!= std::string::npos)
			s.setUploadFolder(AfterEqual(*first_bracket));

		if (((*first_bracket).find("port="))!= std::string::npos){

			std::string raw_ports = AfterEqual(*first_bracket);
			s.setPortServer(*first_bracket);
		}
		in.push_back(*first_bracket);
		first_bracket++;
	}
	return(s);
}


/**********************************************************************************************************************/
/***************************                   Get infos from one location {}     	           ************************/
/**********************************************************************************************************************/

// Insert in a map, an input with "=" as delimiter. ex: key= "location" | value="\methods"
void
insertMap_split_by_Delimiter(
		std::map<std::string,
		std::string> &map,
		const std::string& input,
		const std::string& delimiter)
{
	unsigned long pos;
	if((pos = input.find(delimiter)) != std::string::npos)
	{
		std::string key = input.substr(0, pos);
		getOnlyChar(key);
		std::string value = input.substr(pos + 1, input.length());
		getOnlyChar(value);
		map.insert(std::pair<std::string, std::string>(key, value));
	}
	else
		std::cerr<<R<< "Error: @fn insertMap_split_by_Delimiter \nDelimiter '" << delimiter \
		<< "' not found in " << input <<RE<< std::endl;
}

// Set _locs :  a location
std::vector<std::string>::iterator
grabLocation (
		std::vector<std::string>::iterator 	it,
		std::vector<std::string>::iterator 	last_bracket,
		Config &s)
{
	std::vector<std::string>::iterator 	first_bracket;
	std::map<std::string, std::string> loc_config;

	//Stock "location=(...) in a vector"
	if ((*it).find("location") != std::string::npos)
		insertMap_split_by_Delimiter(loc_config, *it, "=");

	while(it != last_bracket)
	{
		if (((*it).find("{"))!= std::string::npos)
		{
			it++;
			while (!(((*it).find("}")) != std::string::npos))
			{
				insertMap_split_by_Delimiter(loc_config, *it, "=");
				it++;
			}
			s.getAllLocations().push_back(loc_config);
			return (it);
		}
		it++;
	}
	if (it == last_bracket) {
		std::cerr<<R<< "Last '}' in location not found in config file" <<RE<< std::endl;
	}
	return(it);
}


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

// CQFD
std::string
AfterEqual (std::string &input) {
	return (input.substr((input.find('=')) + 1, input.length()));
}
// CQFD
void
printVector(std::vector<std::string> &x ) {
	for (std::vector<std::string>::iterator it = x.begin(); it != x.end(); it++)
		std::cout << *it << std::endl;
}
// CQFD
void
getOnlyChar(std::string &s) {
	s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
}

void
insert_mime_type(
		std::string input,
		std::map<std::string, std::string> &mime_types)
{
	std::string tmp_keys;
	std::string key;
	std::string value;

	unsigned long pos;
	unsigned long start;
	unsigned long end;

	if((pos = input.find('=')) != std::string::npos)
	{
		// Get value | ex: text/html
		value = input.substr(0, pos);
		getOnlyChar(value);
		input.erase(0, pos + 1);
		// Get key | ex: .css
		while ((start = input.find('.')) != std::string::npos)
		{
			// erase .
			input.erase(start, 1);
			// ex: .html.htm.shtml -> insert html=text/html | htm=text/html | shtml=text/html
			if ((end = input.find('.')) != std::string::npos){
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

std::map<std::string, std::string> save_mime_type(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket)
{
	std::map<std::string, std::string> mime_types;

	while (first_bracket != last_bracket)
	{
		if (((*first_bracket).find("="))!= std::string::npos)
			insert_mime_type(*first_bracket, mime_types);
		first_bracket++;
	}
//	std::cout <<G<< "mime_types successfully saved " <<RE<< std::endl;
	return (mime_types);
}
