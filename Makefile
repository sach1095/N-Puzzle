# Compilation
NAME		=	N-Puzzle
CC			=	g++
FLAGS		=	-std=c++17 -Wall -Wextra -Werror -g3 -O3
HEADER		=	-I$(PATH_INC)

# Color Code and template code
GREY = \033[2:49;39m
RED = \033[1;31m
GREEN = \033[3;32m
YELLOW = \033[3;49;33m
BLUE = \033[3;4;34m
BLUEE = \033[3;34m
PURPLE = \033[3;35m
CYAN = \033[3;36m
RESET = \033[0m
ICONOK = \033[1;32m
ICONKO = \033[1;31m
CO_DELET = \033[3;31m
CO_A_OUT = \033[96m
WHITE = \033[1;37m


# Path directories
PATH_INC	=	include
PATH_SRC	=	src
PATH_OBJ	=	obj

# Source
SRC		=	$(addprefix $(PATH_SRC)/, \
			GeneratorMap.cpp \
			SearchAlgo.cpp	\
			Heuristics.cpp	\
			Npuzzle.cpp		\
			Parse.cpp		\
			PuzzleExtraInfo.cpp	\
			)

OBJ		=	$(addprefix $(PATH_OBJ)/, $(SRC:.cpp=.o))
DEPENDS	= $(OBJ:%.o=%.d)

# Rules
all		:	$(NAME)

$(NAME)	:	$(OBJ)
	@ $(CC) $(FLAGS) $(HEADER) $(OBJ) -o $(NAME)
	@ printf "\033[2K\r$(BLUE)$(NAME)$(RESET)$(BLUEE): $(ICONOK)Compiled [√]$(RESET)\n"

$(PATH_OBJ)/%.o		: 	%.cpp
	@ mkdir -p $(dir $@)
	@ printf "\033[2K\r$(PURPLE)$<: $(CYAN)loading..$(RESET)"
	@ $(CC) $(FLAGS) $(HEADER) -c -MMD  -o $@ $<

-include $(DEPENDS)

clean	:
	@ rm -rf ${obj}
	@ rm -rf obj

fclean	:	clean
	@ rm -rf $(NAME)
	@ rm -rf solutionviewer.html
	@ printf "$(CYAN)'$(NAME)'$(RESET) and all .o has been $(RED)deleted. 🗑️\n$(RESET)"

re		:	fclean all

run : all
	@ ./$(NAME) --size 3

visu : all
	@ ./$(NAME) --size 3 --viewer

file: all
	@ ./$(NAME) --file maps/valids/3x3/3x3_comment_inline.txt

help	:
	@ printf "How run Npuzzle : \n\n"
	@ printf "make && ./N-Puzzle maps/[invalids or valids]/[one map .txt]\n"
	@ printf "or\n"
	@ printf "make run\n\n"
	@ printf "Command of the Makefile : \n\n"
	@ printf " - run :          run algo witch generated map 3x3 \n"
	@ printf " - file :         run algo witch map 4x4 solvable.txt \n"
	@ printf " - visu :         run algo witch generated map 3x3 and create file html\n"
	@ printf " - clean :        clean objs \n"
	@ printf " - fclean :       clean exec and objs\n"
	@ printf " - re :           clean and recompile\n"

.PHONY: all compil clean fclean re leaks run file help
