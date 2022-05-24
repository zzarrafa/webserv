NAME = webserv

SRCS = webserv.cpp tools/tools.cpp parser/parsefile.cpp parser/server.cpp request.cpp

all: $(NAME)

$(NAME): $(SRCS)
		@c++ -std=c++98 -Wall -Wextra -Werror $(SRCS) -o $(NAME)
		
clean:
	@rm -rf $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re