CXX= c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3
CXXFLAGS = $(FLAGS) -MMD
RM = rm -rf

NAME = ircserv

BONUS = bot

SRC =	main.cpp Serveur.cpp Client.cpp  Channel.cpp utils.cpp

SRC_CMD =	cap.cpp error.cpp invite.cpp join.cpp kick.cpp mode.cpp nick.cpp\
			part.cpp pass.cpp privmsg.cpp ping.cpp quit.cpp topic.cpp user.cpp

SRC_BONUS = Bot.cpp Joueur.cpp

BONUS_DIR = ./bonus/
SRCS_BONUS = $(addprefix $(BONUS_DIR), $(SRC_BONUS))
OBJDIR_BONUS = ./obj_bonus/
OBJ_BONUS = $(addprefix $(OBJDIR_BONUS), $(notdir $(SRC_BONUS:.cpp=.o)))
DEPS_BONUS = $(addprefix $(OBJ_BONUS), $(notdir $(SRCS_BONUS:.cpp=.d)))

SRC_DIR = ./srcs/
SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJ_DIR = ./obj/
OBJS = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.cpp=.o)))
DEPS = $(addprefix $(OBJ_DIR), $(notdir $(SRCS:.cpp=.d)))


SRC_CDM_DIR = ./srcs/commande
SRCS_CMD = $(addprefix $(SRC_CMD_DIR), $(SRC_CMD))
OBJ_CMD_DIR = ./obj/commande/
OBJS_CMD = $(addprefix $(OBJ_CMD_DIR), $(notdir $(SRCS_CMD:.cpp=.o)))
DEPS_CMD = $(addprefix $(OBJ_CMD_DIR), $(notdir $(SRCS_CMD:.cpp=.d)))

OBJS += $(OBJS_CMD)
DEPS += $(DEPS_CMD)

all: $(NAME)

bonus:	$(BONUS)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(BONUS): $(OBJ_BONUS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR_BONUS)%.o: $(BONUS_DIR)%.cpp | $(OBJDIR_BONUS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR_BONUS):
	mkdir -p $@

-include $(DEPS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_CMD_DIR)

clean:
	@echo "objet = $(OBJS)"
	$(RM) $(OBJ_DIR)
	$(RM) $(OBJDIR_BONUS)

fclean: clean
	$(RM) $(BONUS) $(NAME)

re: fclean
	make -j

.PHONY: all bonus clean fclean re