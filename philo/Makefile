NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -fsanitize=thread
RM = rm -f

SRCS = main.c \
		routine.c \
		utils.c \
		utils_two.c \
		utils_three.c
OBJS = $(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "Compilation complete. Executable: $(NAME)"
	@echo "To run the program, use: ./$(NAME) [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]"

all: $(NAME)
clean:
	$(RM) $(OBJS)
	@echo "Object files cleaned."
fclean: clean
	$(RM) $(NAME)
	@echo "Executable cleaned."
re: fclean all
	@echo "Recompiled all files."
.PHONY: all clean fclean re
