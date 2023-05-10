//
// Created by Lowell Zima on 4/12/23.
//

#include "../../includes/CGI.hpp"


/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

CGI::CGI() { /*std::cout << "++CGI" << std::endl; */}

CGI::~CGI() { /*std::cout << "--CGI" << std::endl;*/ }

/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/


bool CGI::check(Request &r)
{
	if (r.get_URI().find('?') != std::string::npos)
		return true;
	return false;
}
//
//bool CGI::check(Request &r)
//{
//	if (r.get_URI().find('?') != std::string::npos)
//		return true;
//	return false;
//}

std::string CGI::CGIstore(Response &rep){
	//TODO fileName = Catch scriptPath like ("website/cgi/cgi.py") and link to Request object
//	const std::string& fileName = rep.getUriPathClean();

	(void)rep;
	const std::string& fileName = "./website/cgi/cgi.py"; //todo
	std::vector<std::string> rawfile;
//	std::cout <<W<< "@fn CGI::CGIstore() \n"
//	<<B<< "Store: UriPathClean{" << rep.getUriPathClean() << "}" <<RE<< std::endl;
	std::string rawcontent;
	std::string line;
	std::ifstream file;

	// Try open file
	file.open(fileName.c_str());
	if (!file.is_open() || file.bad())
		throw std::bad_exception();

	while (std::getline(file, line)){
		rawcontent.append(line);
	}

	file.close();
	return (rawcontent);
}


std::map<std::string, std::string> CGI::setUpEnvVariablesCGI(Request &request, Config &conf, int port)
{
	std::map<std::string, std::string> envmap;
	std::string binpath = "/usr/bin/python3";
//	std::string binpath = conf.getBinCgi();

	std::string URI = request.get_URI();

	size_t pos = URI.rfind('/');
	std::string nameScript = URI.substr(pos + 1, URI.size());

	request.get_header("Content-Type");

	envmap["AUTH_TYPE"] = "null";
	envmap["CONTENT_TYPE"] = request.get_header("Content-Type");
	envmap["CONTENT_LENGTH"] = numberToString(request.get_body().size());

	envmap["GATEWAY_INTERFACE"] = "CGI/1.1";
	envmap["PATH_INFO"] = binpath;
	envmap["PATH_TRANSLATED"] = binpath;
	envmap["REQUEST_URI"] = binpath;

	envmap["SERVER_NAME"] = conf.getIpServer();
	envmap["SERVER_PROTOCOL"] = request.get_HTTP_version();
	envmap["SERVER_PORT"] = numberToString(port);

	envmap["REQUEST_METHOD"] = request.get_type();
	envmap["SCRIPT_FILENAME"] = nameScript;
	envmap["SCRIPT_NAME"] = nameScript;
	envmap["QUERY_STRING"] = IsQuery(request.get_URI());

	envmap["REDIRECT_STATUS"] = "CGI";

	std::map<std::string, std::string> map = request.get_headers_map();

	for (std::map<std::string, std::string>::iterator it = map.begin(); it != map.end(); it++)
	{
		if (!it->second.empty())
		{
			std::string up = it->first;
			std::string header = "HTTP_" + str_toupper(up);
			std::replace(header.begin(), header.end(), '-', '_');
			envmap[header] = it->second;
		}
	}

	return (envmap);
}

//TODO Catch scriptPath like ("website/cgi/cgi.py") and link to Request object
std::string CGI::execute(Request &request, Response &rep, Config &conf, int port)
{
	(void)request;	//to
	(void)conf;		//remove

	int p_out[2];
	int p_in[2];
	int ret;
	char buffer[4096];
	std::string result = "";

	// test with path found by getPath_of_URI in @deal_with_data
	std::string rawcontent = CGIstore(rep);

	pipe(p_out);
	pipe(p_in);

	fcntl(p_out[0], F_SETFL, O_NONBLOCK);
	fcntl(p_out[1], F_SETFL, O_NONBLOCK);
	fcntl(p_in[0], F_SETFL, O_NONBLOCK);
	fcntl(p_in[1], F_SETFL, O_NONBLOCK);

	const std::string& binCGI = conf.getBinCgi();
//	const std::string& URIPathClean = rep.getUriPathClean();//todo
	const std::string& URIPathClean = "./website/cgi/cgi.py";

	std::vector<char *> path;
	path.push_back(const_cast<char *>(binCGI.c_str()));
	path.push_back(const_cast<char *>(URIPathClean.c_str()));

	std::map<std::string, std::string> envmap = setUpEnvVariablesCGI(request, conf, port);

	if (!envmap["QUERY_STRING"].empty()) {
		path.push_back(const_cast<char *>(envmap["QUERY_STRING"].c_str()));
	}
	path.push_back(NULL);

	pid_t pid = fork();

	if (pid == 0)
	{
		close(p_out[0]);
		dup2(p_out[1], STDOUT_FILENO);
		close(p_out[1]);

		close(p_in[1]);
		dup2(p_in[0], STDIN_FILENO);
		close(p_in[0]);
		execve(path[0], &path[0], maptoarray(envmap));
		exit(0);
	}
	else
	{
		close(p_in[0]);
		close(p_out[1]);
		write(p_in[1], rawcontent.c_str(), rawcontent.size());
		close(p_in[1]);
		if (waitpid(pid, &ret, 0) == -1)
			throw(std::exception());
		if (ret != 0) {
			result.append("@fn CGI::execute()\nStatus: 502 Error in CGI application\r\n");
		}

		do
		{
			bzero(&buffer, sizeof(buffer));
			ret = read(p_out[0], buffer, sizeof(buffer));
			if (ret > 0)
				result.append(std::string(buffer, ret));
		}
		while (ret == sizeof(buffer));
			close(p_out[0]);
	}
	std::cout <<Y<< result <<RE<< std::endl;
	return (result);
}
