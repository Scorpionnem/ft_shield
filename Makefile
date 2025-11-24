CC = cc
CFLAGS = -Wall -Wextra -Werror -MMD -MP -g

NAME = ft_shield
NAME_BONUS = ft_sword

INCLUDES = -I includes -I includes/list -I includes/server -I includes/bonus

SRCS =	src/main.c\
		src/server/server.c\
		src/server/server_utils.c\
		src/server/server_hooks.c\
		src/list/list.c\
		src/list/list_node.c\
		src/logger.c\
		src/payload.c\
		src/ctx.c\
		src/sha256.c

SRCS_BONUS =	src/bonus/main.c\
				src/server/server.c\
				src/server/server_utils.c\
				src/list/list_node.c\
				src/list/list.c\

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

OBJS_BONUS = $(SRCS_BONUS:%.c=$(OBJDIR)/%.o)
DEPS_BONUS = $(SRCS_BONUS:%.c=$(OBJDIR)/%.d)

all: $(NAME)
bonus: $(NAME_BONUS)

shield: all
sword: bonus

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lssl -lcrypto

$(NAME_BONUS): $(OBJS_BONUS)
	@echo Compiling $(NAME_BONUS)
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
	@echo Cleaning $(NAME_BONUS)
	@rm -rf $(NAME)
	@rm -rf $(NAME_BONUS)

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean run re bonus sword shield

-include $(DEPS) $(DEPS_BONUS)
