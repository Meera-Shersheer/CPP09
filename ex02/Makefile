GREEN = \033[3;32m
YELLOW = \033[3;33m
MAGENTA = \033[3;35m
CYAN = \033[3;36m
NC = \033[0m

CPP = c++ 

CPPFLAGS = -Wall -Wextra -Werror  -std=c++98

NAME = PmergeMe
SRC =  main.cpp PmergeMe.cpp

OBJECTDIR = object_files
OBJ = $(addprefix $(OBJECTDIR)/, $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@echo "$(MAGENTA)Building $(NAME)...$(NC)"
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@echo "$(MAGENTA)Build complete.$(NC)"

$(OBJECTDIR)/%.o: %.cpp
	@mkdir -p $(OBJECTDIR)
	@$(CPP) $(CPPFLAGS)  -c $< -o $@

clean:
	rm -rf $(OBJECTDIR)
	@echo "$(CYAN)Clean is done.$(NC)"

fclean: 
	rm -rf $(OBJECTDIR)
	rm -rf $(NAME)
	@echo "$(CYAN)fClean is done.$(NC)"

re: fclean all

.PHONY: all clean fclean re