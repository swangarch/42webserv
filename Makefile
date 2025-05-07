CPPC = c++

CFLAG = -Wall -Wextra -Werror -std=c++98

INCLUDES = -I./includes -I.

SRCS = srcs/main.cpp \
	srcs/server/log.class.cpp \
	srcs/server/webserv.class.cpp \
	srcs/server/reqParser.class.cpp \
	srcs/server/connection.class.cpp \
	srcs/server/infopage.class.cpp \
	srcs/server/parseTools.class.cpp \
	srcs/server/cgi.class.cpp \
	srcs/server/file.class.cpp \
	srcs/server/time.class.cpp \
	srcs/server/request.class.cpp \
	srcs/config/AnyErrorException.cpp \
	srcs/config/CheckFileInfo.cpp \
	srcs/config/ConfigManager.cpp \
	srcs/config/Location.cpp \
	srcs/config/ServerInfo.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = webserv

all: $(NAME)

$(NAME): $(OBJS)
	$(CPPC) $(CFLAG) $(OBJS) -o $(NAME)

%.o:%.cpp
	$(CPPC) $(INCLUDES) $(CFLAG) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

debug: CFLAG += -DDEBUG_SHOW -g
debug: fclean all

.PHONY: all clean fclean re