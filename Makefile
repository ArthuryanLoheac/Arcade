##
## EPITECH PROJECT, 2023
## makefile
## File description:
## makefile
##

# ============= COLORS =============

GREEN=\033[0;32m
RED=\033[0;31m
YEL=\033[01;33m
BLUE=\e[1;34m
NC=\033[0m

# ============= OBJECT ============= #

OBJ_DIR = obj

OBJ = $(SRC_LIB:.$(EXTENSION)=$(OBJ_DIR)/%.o)

OBJ_SRC = $(SRC:%.cpp=$(OBJ_DIR)/%.o)

OBJ_MAIN = $(SRC_MAIN:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(OBJ_SRC:.o=.d) $(OBJ_MAIN:.o=.d)

# ============= PARAMETERS ============= #

COMPILER = g++

EXTENSION = cpp

# ============= FLAGS ============= #

FLAGS_INCLUDE = -I./include \
		-I./src \
		-I./src/core \
		-I./src/interfaces \

FLAGS = -I./include \
		-I./src \
		-I./src/core \
		-I./src/interfaces \
		-MMD -MP $(FLAGS_LIB) -ldl \

FLAGS_GAMES = -shared -fPIC \
		$(FLAGS_INCLUDE)

FLAGS_SDL = $(FLAGS_LIB) -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer \
            $(FLAGS_INCLUDE)

FLAGS_NCURSE = $(FLAGS_LIB) -lncurses \
			$(FLAGS_INCLUDE)

FLAGS_SFML = $(FLAGS_LIB) -lsfml-graphics -lsfml-window -lsfml-system \
			$(FLAGS_INCLUDE)

FLAGS_TEST = $(FLAGS) -lcriterion --coverage \

FLAGS_LIB = -std=c++20 -Wall -Wextra -Werror

# ============= NAMES ============= #

NAME_LIB	= \

NAME	=	arcade

NAME_SDL = lib/arcade_sdl2.so

NAME_NCURSE = lib/arcade_ncurse.so

NAME_SFML = lib/arcade_sfml.so

NAME_MINESWEEP = lib/arcade_minesweeper.so

# ============= SOURCES ============= #

SRC_LIB	=	\

SRC_MAIN	=	main.cpp \

SRC	= 	src/core/Core.cpp \
		src/core/CoreMenu.cpp \

SRC_SDL = src/displays/SDL/SDLDisplay.cpp \
		 src/displays/SDL/SDL2.cpp \

SRC_NCURSE	=	src/displays/NCurse/NCurseDisplay.cpp \
				src/displays/NCurse/NCurseWrapper.cpp

SRC_SFML	=	src/displays/SFML/SFMLDisplay.cpp \

SRC_MINESWEEP	=	src/games/MineSweepGame.cpp

SRC_TESTS	= 	tests/test_1.cpp \

# ============= RULES ============= #

all: core graphicals games

$(NAME): $(OBJ_SRC) $(OBJ_MAIN)
	$(COMPILER) -o $(NAME) $(OBJ_SRC) $(OBJ_MAIN) $(FLAGS)

$(NAME_LIB): $(OBJ)
	ar rc $(NAME_LIB) $(OBJ)

core: $(OBJ_SRC) $(OBJ_MAIN)
	$(COMPILER) -o $(NAME) $(OBJ_SRC) $(OBJ_MAIN) $(FLAGS)

graphicals:
	$(COMPILER) -o $(NAME_SDL) -shared -fPIC $(SRC_SDL) $(FLAGS_SDL)
	$(COMPILER) -o $(NAME_NCURSE) -shared -fPIC $(SRC_NCURSE) $(FLAGS_NCURSE)
	$(COMPILER) -o $(NAME_SFML) -shared -fPIC $(SRC_SFML) $(FLAGS_SFML)

games:
	$(COMPILER) -o $(NAME_MINESWEEP) $(SRC_MINESWEEP) $(FLAGS_GAMES)

# ============= CLEANS ============= #

clean:
	rm -rf $(OBJ_DIR)
	rm -f *.gcda *.gcno
	rm -f tests/*.gcda tests/*.gcno

fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_LIB)
	rm -f $(NAME_SDL)
	rm -f $(NAME_NCURSE)
	rm -f $(NAME_MINESWEEP)
	rm -f unit_tests
	rm -f lib/*.so lib/*.d

# ============= COMPILATION ============= #

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) -c $(FLAGS) $< -o $@

-include $(DEPS)

# ============= OTHERS ============= #

re: fclean all

run: all
	./$(NAME)

# ============= TESTS ============= #

unit_tests:
	@mkdir -p $(OBJ_DIR)
	$(COMPILER) -o $(OBJ_DIR)/unit_tests $(SRC_TESTS) $(SRC) $(FLAGS_TEST)
	cp $(OBJ_DIR)/unit_tests unit_tests

tests_run: unit_tests
	./$(OBJ_DIR)/unit_tests --verbose

tests_run_coverage: tests_run
	gcovr -r . -e tests/

tests_clean_run: fclean tests_run

tests_clean_run_coverage: tests_clean_run
	gcovr -r . -e tests/
