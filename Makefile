################################################################################
#
# WEBSERV
#
NAME		= webserv

# source files
include srcs.mk

# compilation parameters
CC			= clang++
CCFLAGS		= -Wall -Werror -Wextra -std=c++98
INC			= -Iinclude/

SRCDIR		=	srcs/

# objects
OBJDIR = obj/
OBJ = $(addprefix $(OBJDIR),$(SRCS:.cpp=.o))

################################################################################

all:
	@printf "[ "$(B_GREEN)"building"$(_END) 
	@printf " ] \t|" $(B_WHITE)"\t\t"$(NAME)$(_END) 
	@$(MAKE) $(NAME)
	@printf $(_END)$(B_WHITE)"| \n[ "$(B_GREEN)$(NAME)$(_END)$(B_WHITE)" created with success ]\n"$(_END)


$(NAME): $(OBJDIR) $(OBJ)
	$(CC) $(CCFLAGS) $(INC) $(OBJ) -o $(NAME)

# create an obj/ directory with all the *.o files
$(OBJDIR):
	mkdir -p $@

# compile the *.o
$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CCFLAGS) $(INC) -c $< -o $@
	@printf $(BK_GREEN)" "$(_END)

clean:
	@rm -rf $(OBJDIR)
	@echo "[ "$(B_PURPLE)"deleted"$(_END) "]" $(B_WHITE)"\t\tobjects"$(_END)

fclean: clean
	@rm -rf $(NAME)
	@echo "[ "$(B_PURPLE)"deleted"$(_END) "]" $(B_WHITE)"\t\t"$(NAME)$(_END)

re:
	@$(MAKE) -s fclean
	@$(MAKE) -s

find_srcs:
	@./find_srcs.sh

.PHONY		: all clean re

#	COLORS	#
_END		= "\033[0m"
#	bold	#
B_GREEN		= "\033[1;32m"
B_PURPLE	= "\033[1;35m"
B_WHITE		= "\033[1;37m"
#	background	#
BK_GREEN	= "\033[42m"

################################################################################