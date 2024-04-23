###############################################################################
#                                  VARIABLES                                  #
###############################################################################
NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Werror -Wextra -pthread
RESET=\033[0m
RED=\033[1;31m
GREEN=\033[1;32m
BLUE=\033[1;34m
CYAN=\033[1;36m
HEADERS = ./
SRCS_DIR = ./sources/
SRCS =	parsing.c philosophers.c routine.c
SRCS_PATH = $(addprefix $(SRCS_DIR), $(SRCS))
OBJS_DIR = ./objects/
OBJS = $(SRCS:%.c=%.o)
OBJS_PATH = $(addprefix $(OBJS_DIR), $(OBJS))

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS_PATH)
	@$(CC) $(CFLAGS) $(OBJS_PATH) $(LIBFT) -o $(NAME)
	@echo "$(BLUE)PROGRAM $(GREEN)CREATED: $(CYAN)philo_bonus$(RESET)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_PATH): $(SRCS_PATH)
	@make -s $(OBJS)

$(OBJS):
	@$(CC) $(CFLAGS) -c -I $(HEADERS) $(SRCS_DIR)$(@:%.o=%.c) -o $(OBJS_DIR)$@

clean:
	@echo "$(BLUE)OBJECTS $(RED)DESTROYED$(RESET)"
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -f $(NAME)
	@echo "$(BLUE)PROGRAM $(RED)DESTROYED: $(CYAN)philo_bonus$(RESET)"

re: fclean all

.PHONY: all clean fclean re
