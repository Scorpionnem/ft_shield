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

SRCS_BONUS =	src/bonus/main_bonus.c\
				src/server/server.c\
				src/server/server_utils.c\
				src/list/list_node.c\
				src/list/list.c\

OBJDIR = obj
OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.c=$(OBJDIR)/%.d)

OBJS_BONUS = $(SRCS_BONUS:%.c=$(OBJDIR)/%.o)
DEPS_BONUS = $(SRCS_BONUS:%.c=$(OBJDIR)/%.d)

TPUT 					= tput -T xterm-256color
_RESET 					:= $(shell $(TPUT) sgr0)
_BOLD 					:= $(shell $(TPUT) bold)
_ITALIC 				:= $(shell $(TPUT) sitm)
_UNDER 					:= $(shell $(TPUT) smul)
_GREEN 					:= $(shell $(TPUT) setaf 2)
_YELLOW 				:= $(shell $(TPUT) setaf 3)
_RED 					:= $(shell $(TPUT) setaf 1)
_GRAY 					:= $(shell $(TPUT) setaf 8)
_PURPLE 				:= $(shell $(TPUT) setaf 5)

OBJS_TOTAL	= $(words $(OBJS))
N_OBJS		:= $(shell find $(DIR) -type f -name $(OBJS) 2>/dev/null | wc -l)
OBJS_TOTAL	:= $(shell echo $$(( $(OBJS_TOTAL) - $(N_OBJS) )))
CURR_OBJ	= 0

all: $(NAME)
bonus: $(NAME_BONUS)

shield: all
sword: bonus

re: fclean all

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lssl -lcrypto
	@printf "$(_BOLD)$(NAME)$(_RESET) compiled $(_GREEN)$(_BOLD)successfully$(_RESET)\n\n"

$(NAME_BONUS): $(OBJS_BONUS)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ -lssl -lcrypto
	@printf "$(_BOLD)$(NAME_BONUS)$(_RESET) compiled $(_GREEN)$(_BOLD)successfully$(_RESET)\n\n"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@$(eval CURR_OBJ=$(shell echo $$(( $(CURR_OBJ) + 1 ))))
	@$(eval PERCENT=$(shell echo $$(( $(CURR_OBJ) * 100 / $(OBJS_TOTAL) ))))
	@printf "$(_GREEN)($(_BOLD)%3s%%$(_RESET)$(_GREEN)) $(_RESET)Compiling $(_BOLD)$(_PURPLE)$<$(_RESET)\n" "$(PERCENT)"

clean:
	@rm -rf $(OBJDIR)
	@printf "\n$(_BOLD)All objects are $(_GREEN)cleaned $(_RESET)! 🎉\n\n"

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(NAME_BONUS)
	@printf "Cleaned $(_BOLD)$(NAME)$(_RESET) !\n\n"
	@printf "Cleaned $(_BOLD)$(NAME_BONUS)$(_RESET) !\n\n"

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean run re bonus sword shield

-include $(DEPS) $(DEPS_BONUS)
