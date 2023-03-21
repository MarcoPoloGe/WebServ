SRCS		= main.cpp\
			  srcs/utils/Ft_error.cpp\
			  srcs/ClassRequest.cpp\

OBJS		= ${SRCS:.cpp=.o}

RM			= rm -f

NAME		= webserv

FLAGS		= -Wall -Wextra -Werror -std=c++98 -pedantic

CC			= g++

.cpp.o:
			${CC} ${FLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
			${CC} ${FLAGS} -o ${NAME} ${OBJS}

all:		${NAME}

clean:
			${RM} ${OBJS}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

good:		${NAME} clean

.PHONY:		all clean fclean re good
