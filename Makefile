NAME = webserv

SRCS = webserv.cpp tools/tools.cpp parser/parsefile.cpp parser/server.cpp parser/connection.cpp

all: $(NAME)

$(NAME): $(SRCS)
		@c++ -std=c++98 -Wall -Wextra -Werror  -fsanitize=address -g $(SRCS) -o $(NAME)
		
clean:
	@rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re