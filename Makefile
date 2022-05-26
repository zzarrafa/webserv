NAME = webserv

SRCS = webserv.cpp tools/tools.cpp parser/parsefile.cpp parser/server.cpp parser/connection.cpp request.cpp

all: $(NAME)

$(NAME): $(SRCS)
		@c++ -std=c++98 -Wall -Wextra -Werror  -fsanitize=address -g $(SRCS) -o $(NAME)
		
clean:
	@rm -rf $(NAME)

fclean: clean

start: $(NAME)
	@./$(NAME) config.conf

re: fclean all

.PHONY: all clean fclean re