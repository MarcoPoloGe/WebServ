SRCS		= main.cpp\
			  srcs/utils/Ft_error.cpp\
			  srcs/utils/Utils.cpp\
			  srcs/ClassRequest.cpp\
			  srcs/Network.cpp\
			  srcs/ClassResponse.cpp\
			  srcs/parse_config_file/main_parsing.cpp\
              srcs/parse_config_file/Server.cpp\

OBJS		= ${SRCS:.cpp=.o}

RM			= rm -f

NAME		= webserv

FLAGS		= -Wall -Wextra -Werror -std=c++98 -pedantic

CC			= g++

.cpp.o:
			${CC} ${FLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
			${CC} ${FLAGS} -o ${NAME} ${OBJS}
			@printf "\n✅ Compiled\n";
			@printf "\n"
			@printf "\n💽 Executable name: ";
			@printf "./"${NAME}
			@printf "\n"
			@printf "\n"
			@printf "🧀 Usage: select a configuration file in config folder."
			@printf "\n"
			@printf "\n"
			@printf "ex: ./"
			@printf ${NAME}
			@printf " config/config.conf"
			@printf "\n"
			@printf "\n"

all:		${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

good:		${NAME} clean

# PARSE_CONFIG MAKEFILE COMMAND

NAME_P		= parsing_webserv

SRCS_P		= srcs/parse_config_file/main_parsing.cpp\
              srcs/parse_config_file/Server.cpp\

OBJS_P		= ${SRCS_P:.cpp=.o}

${NAME_P}:	${OBJS_P}
			${CC} ${FLAGS} -o ${NAME_P} ${OBJS_P}
			@printf "\n✅ Compiled\n";
			@printf "\n"
			@printf "\n💽 Executable name: ";
			@printf "./"${NAME_P}
			@printf "\n"
			@printf "\n"
			@printf "🧀 Usage: select a configuration file in config folder."
			@printf "\n"
			@printf "\n"
			@printf "ex: ./"
			@printf ${NAME_P}
			@printf " config/config.conf"
			@printf "\n"
			@printf "\n"

parse:		${NAME_P}


cp:
			${RM} ${OBJS_P}
			@printf "🧹 Clean *.o\n";
fcp:		cp
			${RM} ${NAME_P}
			@printf "🧹 Clean *.o + executable \n";

rep:		fcp parse

.PHONY:		all clean fclean re good rep fcp cp parse
