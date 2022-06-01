NAME = webserv

SRCS =	webserv.cpp\
		http/connection.cpp\
		http/request.cpp\
		http/response.cpp\
		http/server.cpp\
		parser/parsefile.cpp\
		tools/tools.cpp\

all: $(NAME)

$(NAME): $(SRCS)
		@c++ -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g $(SRCS) -o $(NAME)

clean:
	@rm -rf $(NAME)

remove:
	@rm -rf tmp/*

fclean: clean

start: $(NAME)
	@./$(NAME) config.conf

re: fclean all

.PHONY: all clean fclean re