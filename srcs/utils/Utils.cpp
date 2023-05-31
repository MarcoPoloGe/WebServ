#include "../../includes/Webserv_Includes.hpp"

std::string ft_read_file(std::string file_name)
{
	std::ifstream html_file;
	std::string content;
	std::string line;
	unsigned long pos;
	while ((pos = file_name.find("//")) != std::string::npos)
		file_name.erase(pos, 1);

	html_file.open(file_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
	if(html_file.is_open())
	{
		std::cout <<W<< "@fn ft_read_file(std::string file_name)\n"<<G<<"☑️ OK  : " << file_name << "\n" <<RE;
	} else
	{
		std::cout <<W<< "@fn ft_read_file(std::string file_name)\n"<<G<<"🔘️ ERROR  : " << file_name << "\n" <<RE;
	}
	while (std::getline(html_file, line))
		content += line + '\n';
	std::cout <<B<< "The file " << file_name << " was about " << content.size()
		<< " bytes long\n" <<RE;
	return (content);
}

void	setnonblocking(int sock)
{
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		Ft_error	err("fcntl");

	return ;
}

std::string	ft_get_extension(std::string str)
{
	std::size_t last_point = str.rfind(".");

	if (last_point == std::string::npos)
	{
//		std::cout <<W<< "@fn ft_get_extension(std::string str)\n"
//		<<B<< "No particular extension in string [" << str << "]\n" <<RE; //DEBUG
		return ("");
	}
	else
	{
		if (str[last_point + 1] == '/')
			return ("");
		return ( str.substr(last_point + 1) );
	}
}

std::string ft_remove_ack(std::string str)
{
	std::string ret;
	int i = 0;
	int size = 0;

	for (int j = 0; str[j] != 0; j++)
	{
		if (str[j] != 6) 
			size++;
	}
	ret.resize(size);
	for (int it = 0; str[it] != 0; it++)
	{
		if (str[it] != 6)
			ret[i++] = str[it];
	}
	ret[i] = 0;
	return (ret);
}

std::string ft_generate_html_dir(std::string dir_path, int final_slash)
{
	unsigned long	pos;
    while ((pos = dir_path.find("//")) != std::string::npos)
        dir_path.erase(pos, 1);
	
	std::string directory = dir_path;
	dir_path.erase(dir_path.begin(), dir_path.begin() + 10); //veux int taille prepath

    // Création d'une chaîne de caractères contenant le code HTML
	std::ostringstream html;

    html << "<!DOCTYPE html>\n";
    html << "<html>\n";
    html << "<head>\n";
    html << "\t<title>Index of " << directory << "</title>\n";
    html << "</head>\n";
    html << "<body>\n";
    html << "\t<h1>Index of " << directory << "</h1>\n";
    html << "\t<table>\n";
    html << "\t\t<tr>\n";
    html << "\t\t\t<th>Filename</th>\n";
    html << "\t\t\t<th>Last Modified</th>\n";
    html << "\t\t\t<th>Size</th>\n";
    html << "\t\t</tr>\n";

    struct dirent* entry;

	DIR* dir = opendir(directory.c_str());
	if (dir == NULL) {
		std::cerr << "Impossible d'ouvrir le répertoire " << directory << std::endl;
		exit(1);
	}
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.')
		{
            continue; // Ignorer les fichiers cachés
        }

        // Récupération des métadonnées du fichier

//		std::cout << "direct= " << directory << std::endl; //DEBUG
		std::string path = directory + entry->d_name;
        struct stat file_info;
        stat(path.c_str(), &file_info);
        time_t mod_time = file_info.st_mtime;
        off_t size = file_info.st_size;

        // Génération du code HTML pour chaque ligne du tableau
        html << "\t\t<tr>\n";

		if (final_slash)
		{
        	html << "\t\t\t<td><a href=\"" << /*dir_path + "/" + */entry->d_name
				<< "\">" << /*dir_path + "/" + */entry->d_name << "</a></td>\n";
		}
		else
		{
			html << "\t\t\t<td><a href=\"" << dir_path  + entry->d_name
				<< "\">" << dir_path + entry->d_name << "</a></td>\n";
		}

        html << "\t\t\t<td>" << ctime(&mod_time) << "</td>\n";
        html << "\t\t\t<td>" << size << " bytes</td>\n";
        html << "\t\t</tr>\n";
    }

    html << "\t</table>\n";
    html << "</body>\n";
    html << "</html>\n";

    closedir(dir);

	return ( html.str() );
}

std::string ft_what_location(std::string URI)
{
	std::size_t	first_slash = URI.find("/");
	std::size_t	sec_slash = URI.find("/", first_slash + 1);

	if (sec_slash == std::string::npos) //only one slash
		return ( URI ); //a tester
	return ( URI.substr( first_slash,  sec_slash) );
}

std::string numberToString(unsigned long n)
{
	std::ostringstream str1;
	str1 << n;
	return (str1.str());
}

std::string IsQuery(const std::string &URIraw) {
	unsigned long pos;
	if ((pos = URIraw.find('?')) != std::string::npos) {
		return (URIraw.substr(pos + 1));
	}
	return ("");
}

std::string str_toupper(std::string &str) {
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

char *const *maptoarray(std::map<std::string, std::string> &m)
{
	char** arr = new char*[m.size() + 1];
	std::memset(arr, 0, sizeof(char*) * (m.size() + 1));
	int i = 0;
	for (std::map<std::string, std::string>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		std::string s = it->first + "=" + it->second;
		arr[i] = new char[s.length() + 1];
		std::strcpy(arr[i], s.c_str());
		++i;
	}
	return arr;
}

std::string ft_generate_error_html(int error, Config config, std::string more)
{
	std::string	description = config.getDefaultErrorDescription(error);
	if (description == "")
		description = "Unknown error code";

    // Création d'une chaîne de caractères contenant le code HTML
	std::ostringstream html;

    html << "<!DOCTYPE html>\n";
    html << "<html lang=en>\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <title>" << error << " </title>\n";
    html << "<head>\n";
	html << "<body>\n";

	html << "<h2>Error " << error << ": " << description << "</h2>\n";

	html << "<h3> " << more << "</h3>\n";

	html << "</body>\n";
    html << "</html>\n";

	return ( html.str() );
}

std::string ft_generate_error_html(int error, Config config)
{
	std::string	description = config.getDefaultErrorDescription(error);
	if (description == "")
		description = "Unknown error code";

    // Création d'une chaîne de caractères contenant le code HTML
	std::ostringstream html;

    html << "<!DOCTYPE html>\n";
    html << "<html lang=en>\n";
    html << "<head>\n";
    html << "    <meta charset=\"UTF-8\">\n";
    html << "    <title>" << error << " </title>\n";
    html << "<head>\n";
	html << "<body>\n";

	html << "<h2>Error " << error << ": " << description << "</h2>\n";

	html << "</body>\n";
    html << "</html>\n";

	return ( html.str() );
}

std::string	ft_generate_success_delete(Request request)
{
	std::ostringstream	html;
	std::string			referer = request.get_header("Referer");

	std::string			filename = request.get_URI();
	std::size_t			last_slash = filename.rfind("/");
	
	filename = filename.substr(last_slash + 1);

	html << "<html>\n";
	html << "<head>\n";
	html << "<meta http-equiv=\"refresh\" content=\"4; URL=" << referer << "\">\n";
	html << "<style>\n";
	html << "body {\n";
	html << "    background-color: #f4f4f4;\n";
	html << "    display: flex;\n";
	html << "    justify-content: center;\n";
	html << "    align-items: center;\n";
	html << "    height: 100vh;\n";
	html << "}\n";
	html << ".container {\n";
	html << "    background-color: #ffffff;\n";
	html << "    border-radius: 10px;\n";
	html << "    padding: 10px;\n";
	html << "    display: flex;\n";
	html << "    flex-direction: column;\n";
	html << "    align-items: center;\n";
	html << "}\n";
	html << ".container p {\n";
	html << "    font-size: 30px;\n";
	html << "    color: red;\n";
	html << "    text-align: center;\n";
	html << "}\n";
	html << ".container img {\n";
	html << "    margin: 1px auto;\n";
	html << "}\n";
	html << "</style>\n";
	html << "</head>\n";
	html << "<body>\n";
	html << "<div class=\"container\">\n";
	html << "<p style=\"text-align: center;\">Your file has been deleted successfully</p>\n";
	html << "<h4 style=\"text-align: center; margin-bottom: 1px;\">"<< filename << " be like :</h4>";
	html << "<h4 style=\"text-align: center;\"><img src=\"https://media.tenor.com/GwZEshiH6jUAAAAM/disappearing.gif\"></h4>\n";
	html << "</div>\n";
	html << "</body>\n";
	html << "</html>\n";

	return ( html.str() );
}

std::string	ft_generate_success_upload(Request request)
{
	std::ostringstream	html;
	std::string			referer = request.get_header("Referer");

	//std::string			filename = request.get_URI();
	std::string			filename = request.get_content_header(0, "Content-Disposition-filename");
	std::size_t			last_slash = filename.rfind("/");

	filename = filename.substr(last_slash + 1);

	html << "<html>\n";
	html << "<head>\n";
	html << "<meta http-equiv=\"refresh\" content=\"4; URL=" << referer << "\">\n";
	html << "<style>\n";
	html << "body {\n";
	html << "    background-color: #f4f4f4;\n";
	html << "    display: flex;\n";
	html << "    justify-content: center;\n";
	html << "    align-items: center;\n";
	html << "    height: 100vh;\n";
	html << "}\n";
	html << ".container {\n";
	html << "    background-color: #ffffff;\n";
	html << "    border-radius: 10px;\n";
	html << "    padding: 10px;\n";
	html << "    display: flex;\n";
	html << "    flex-direction: column;\n";
	html << "    align-items: center;\n";
	html << "}\n";
	html << ".container p {\n";
	html << "    font-size: 30px;\n";
	html << "    color: red;\n";
	html << "    text-align: center;\n";
	html << "}\n";
	html << ".container img {\n";
	html << "    margin: 1px auto;\n";
	html << "}\n";
	html << "</style>\n";
	html << "</head>\n";
	html << "<body>\n";
	html << "<div class=\"container\">\n";
	html << "<p style=\"text-align: center;\">Your file has been uploaded successfully</p>\n";
	html << "<h4 style=\"text-align: center; margin-bottom: 1px;\">"<< filename << " be like :</h4>";
	html << "<h4 style=\"text-align: center;\"><img src=\"https://media.tenor.com/ByflbjcyUG4AAAAd/oh-no-oh-no-oh-yeh.gif\"></h4>\n";
	html << "</div>\n";
	html << "</body>\n";
	html << "</html>\n";

	return ( html.str() );
}

bool isFile(const std::string& path)
{
    std::ifstream file(path.c_str());
    return file.good();
}

bool isDirectory(const std::string& path)
{
    std::ifstream dir(path.c_str());
    return dir.good();
}
