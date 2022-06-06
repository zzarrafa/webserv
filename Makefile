NAME = webserv
FLAGS = -std=c++98 -Wall -Wextra -Werror #-g -fsanitize=address

SRCS =	src/webserv.cpp\
		src/http/connection.cpp\
		src/http/request.cpp\
		src/http/response.cpp\
		src/http/server.cpp\
		src/parser/parsefile.cpp\
		src/tools/tools.cpp\
		src/cgi/cgi.cpp

all: $(NAME)

$(NAME): $(SRCS)
		@c++ $(FLAGS) $(SRCS) -o $(NAME)

clean:
	@rm -rf $(NAME)

remove:
	@rm -rf tmp/*

fclean: clean remove

start: $(NAME)
	@./$(NAME) configs/config.conf

re: fclean all

.PHONY: all clean fclean re