//
// Created by Lowell Zima on 4/12/23.
//

#include "../../includes/CGI.hpp"


/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/

CGI::CGI() {
	std::cout << "++CGI" << std::endl;
}
//CGI::CGI(
//		Request &Req,
//		Network &Net )
//{
//	Request test;
//	test.setHttpVersion("HTTP/1.1");
//	test.setType("");
//	std::cout << "++CGI + Request + Network" << std::endl;
//}

CGI::~CGI() { std::cout << "--CGI" << std::endl; }


/**********************************************************************************************************************/
/***************************                       Getters	            		               ************************/
/**********************************************************************************************************************/

//std::string CGI::getBin() {
//	_bin = "/usr/bin/python3";
//	return (_bin);
//}
//
//void CGI::setENV(Request &req, Network &net, Config &conf) {
//	(void)req;
//	(void)net;
//	(void)conf;
//	return;
//}


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/

/*
 * pos = token[1].find("?");
					if (pos != std::string::npos)
					{
						setUrl(token[1].substr(0, pos));
						setQuery(token[1].substr(pos + 1));
					}
*/



std::string CGI::CGIstore(Response &rep){
	//TODO fileName = Catch scriptPath like ("website/cgi/cgi.py") and link to Request object
	const std::string& fileName = rep.getUriPathClean();

	std::vector<std::string> rawfile;
	std::cout <<W<< "@fn CGI::CGIstore() \n"
	<<B<< "Store: UriPathClean{" << rep.getUriPathClean() << "}" <<RE<< std::endl;
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


std::string CGI::IsQuery(const std::string &URIraw) {
	unsigned long pos;
	if ((pos = URIraw.find('?')) != std::string::npos) {
		return (URIraw.substr(pos + 1));
	}
	return ("");
}

//TODO Catch scriptPath like ("website/cgi/cgi.py") and link to Request object
std::string CGI::execute(Request &request, Response &rep, Config &conf)
{
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
	/* Infos:
	 * F_SETFL            Set descriptor status flags to arg.
	 *
	 * O_NONBLOCK   Non-blocking I/O; if no data is available to a read
                        call, or if a write operation would block, the read or
                        write call returns -1 with the error EAGAIN.
	 */

	//TODO pars Query form Request object
//	std::vector<char *> path = preparePath(request.getQuery());
//	std::map<std::string, std::string> env = prepareEnv(request);

	//TODO Protection: scriptPath arg openning
//	std::string pathstring = "/website/cgi/cgi.py";
//	if (access(pathstring.c_str(), X_OK))
//			throw(std::exception());

	std::string _location = "/usr/bin/python3";
	std::string _scriptPath = "./website/cgi/cgi.py";

	//TODO set up _location like "/usr/bin/python3" and scriptPath arg like ""website/cgi/cgi.py""
	std::vector<char *> path;
	path.push_back(const_cast<char *>(_location.c_str()));
	path.push_back(const_cast<char *>(_scriptPath.c_str()));

	//TODO if Request object have a '?', so query need to be handle
//	if (!query.empty())
//		path.push_back(const_cast<char *>(_query.c_str()));
	path.push_back(NULL);

	pid_t pid = fork();

	//TODO set up env variables from Request and Config
	char *const *envtest = NULL;
	if (pid == 0)
	{
		close(p_out[0]);
		dup2(p_out[1], STDOUT_FILENO);
		close(p_out[1]);

		close(p_in[1]);
		dup2(p_in[0], STDIN_FILENO);
		close(p_in[0]);
		execve(path[0], &path[0], envtest);
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
