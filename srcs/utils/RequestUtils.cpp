#include "../../includes/Webserv_Includes.hpp"

void process_multipart_header(std::string &raw_header, std::string header_name, std::map<std::string,std::string> &headers_map)
{
	std::string temp;
	std::string first;
	std::string second;

	std::istringstream raw(raw_header);
	while (std::getline(raw >> std::ws, temp, ';'))
	{
		std::istringstream stemp(temp);
		if(std::getline(stemp >> std::ws, first, '=') && std::getline(stemp, second))
		{
			second.erase(std::remove(second.begin(), second.end(), '\"'), second.end());
			headers_map.insert(std::make_pair(header_name + '-' + first, second));
		}
	}
}

std::map<std::string,std::string> process_headers(std::istringstream &file)
{
	std::map<std::string,std::string> headers_map;
	std::string temp;
	std::string first;
	std::string second;

	while (std::getline(file, temp))
	{
		if(temp.empty())
			break;
		std::istringstream stemp(temp);
		if(std::getline(stemp >> std::ws, first, ':'))
		{
			if(std::getline(stemp >> std::ws, second))
			{
				if(second.find(";") != second.npos && first != "User-Agent" && first != "Accept" && first != "Accept-Language")
				{
					temp = second;
					second = temp.substr(0, temp.find(";"));
					temp.erase(0, temp.find(";") + 1);
					process_multipart_header(temp, first, headers_map);
				}
				headers_map.insert(std::make_pair(first, second));
			}
		}
	}
	return (headers_map);
}

std::vector<RequestContent> process_contents(std::istringstream &file, std::map<std::string,std::string> &headers_map)
{
	std::vector<RequestContent> content_list;
	std::string boundary;
	std::string raw_content;
	std::string line;

	if(headers_map.find("Content-Type-boundary") != headers_map.end())
		boundary = headers_map.find("Content-Type-boundary")->second;

	while (std::getline(file, line))
	{
		if(line.find(boundary) != line.npos)
		{
			if(!raw_content.empty())
			{
				content_list.push_back(RequestContent(raw_content));
				raw_content.clear();
			}
		}
		else
			raw_content += line + '\n';
	}
	if(!raw_content.empty())
		content_list.push_back(RequestContent(raw_content));
	return (content_list);
}
