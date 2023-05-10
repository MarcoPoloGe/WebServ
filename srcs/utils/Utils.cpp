#include "../../includes/Webserv_Includes.hpp"

std::string ft_read_file(std::string file_name)
{
	std::ifstream html_file;
	std::string content;
	std::string line;

	html_file.open(file_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
	if(html_file.is_open())
	{
		std::cout <<W<< "@fn ft_read_file(std::string file_name)\n"<<G<<"☑️ OK  : " << file_name << "\n" <<RE;
	} else
	{
		std::cout <<W<< "@fn ft_read_file(std::string file_name)\n"<<G<<"🔘️ ERROR  : " << file_name << "\n" <<RE;
	}
	while (std::getline(html_file, line))
	{
		content += line + '\n';
	}
	return (content);
}

unsigned short	ft_bswap16(unsigned short x)
{
	x = x >> 8 | x << 8;
	return (x);
}

unsigned int	ft_bswap32(unsigned int x)
{
	x = (x & BYTE_0) >> 24 | (x & BYTE_3) << 24 |
		(x & BYTE_1) >> 8  | (x & BYTE_2) << 8;
	return (x);
}

unsigned short	ft_htons(unsigned short s)
{
	if (BYTE_ORDER == BIG_ENDIAN)
		return (s);
	else if (BYTE_ORDER == LITTLE_ENDIAN)
		return (ft_bswap16(s));
	return (0);
}

unsigned int	ft_htonl(unsigned int l)
{
	if (BYTE_ORDER == BIG_ENDIAN)
		return (l);
	else if (BYTE_ORDER == LITTLE_ENDIAN)
		return (ft_bswap32(l));
	return (0);
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

std::string ft_remove_nonprintable(std::string str)
{
	std::string ret;
	int i = 0;
	int size = 0;

	for (int j = 0; str[j] != 0; j++)
	{
		if (31 < str[j] && str[j] < 127)
			size++;
	}
	ret.resize(size);
	for (int it = 0; str[it] != 0; it++)
	{
		if (31 < str[it] && str[it] < 127)
			ret[i++] = str[it];
	}
	ret[i] = 0;
	return (ret);
}

std::string ft_generate_html_dir(std::string dir_path)
{
	std::string directory = dir_path; // Répertoire à indexer

    DIR* dir = opendir(directory.c_str());
    if (dir == NULL) {
		std::cerr << "Impossible d'ouvrir le répertoire " << directory << std::endl;
        exit(1);
    }

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
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // Ignorer les fichiers cachés
        }

        // Récupération des métadonnées du fichier
		std::string path = directory + "/" + entry->d_name;
        struct stat file_info;
        stat(path.c_str(), &file_info);
        time_t mod_time = file_info.st_mtime;
        off_t size = file_info.st_size;

        // Génération du code HTML pour chaque ligne du tableau
        html << "\t\t<tr>\n";
        html << "\t\t\t<td><a href=\"" << /*dir_path + "/" + */entry->d_name
			<< "\">" << /*dir_path + "/" + */entry->d_name << "</a></td>\n";
        html << "\t\t\t<td>" << ctime(&mod_time) << "</td>\n";
        html << "\t\t\t<td>" << size << " bytes</td>\n";
        html << "\t\t</tr>\n";
    }

    html << "\t</table>\n";
    html << "</body>\n";
    html << "</html>\n";

    // Écriture du code HTML dans un fichier
/*	std::ofstream output_file("index.html");
    if (output_file.is_open()) {
        output_file << html.str();
        output_file.close();
		std::cout << "Page d'auto-indexage générée avec succès." << std::endl;
    } else {
		std::cerr << "Impossible de créer le fichier de sortie." << std::endl;
        exit(1);
    }*/

    closedir(dir);

	return ( html.str() );
}

std::string ft_what_location(std::string URI)
{
	std::size_t	first_slash = URI.find("/");
	std::size_t	sec_slash = URI.find("/", first_slash + 1);

	if (sec_slash == std::string::npos) //only one slash
		return ( URI ); //a tester
	if (sec_slash == URI.length() - 1) //final slash is cropped
		return ( URI.substr( 0, (URI.length() - 1) ) );
	return ( URI.substr( 0, (URI.length() - sec_slash) ) );
}
