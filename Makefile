SRCS		= main.cpp

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

.PHONY:		all clean fclean re
