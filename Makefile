CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g

INCLUDES = -I includes

NAME = ft_shield

SRCS =	src/main.c\
		src/server.c\
		src/server_utils.c\
		src/list.c\
		src/list_node.c

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

all: $(NAME)

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lssl -lcrypto

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean run re

-include $(DEPS)
