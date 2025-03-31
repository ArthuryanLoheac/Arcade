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

FLAGS = -I./include \
		-I./src \
		-I./src/core \
		-I./src/interfaces \
		-MMD -MP $(FLAGS_LIB) \

FLAGS_SDL = $(FLAGS_LIB) -lSDL2 -lSDL2_image \
			-I./src/interfaces \
			-I./include \
			-I./src/core \

FLAGS_TEST = $(FLAGS) -lcriterion --coverage \

FLAGS_LIB = -std=c++20 -Wall -Wextra -Werror

CXXFLAGS += -I/usr/include/SDL2 -I./src/interfaces -I./include -I./src/core
LDFLAGS += -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer

# ============= NAMES ============= #

NAME_LIB	= \

NAME	=	arcade

NAME_SDL = lib/arcade_sdl2.so

# ============= SOURCES ============= #

SRC_LIB	=	\

SRC_MAIN	=	main.cpp \

SRC	= 	src/core/Core.cpp \

SRC_SDL = src/interfaces/SDL/SDLDisplay.cpp \
		 src/interfaces/SDL/SDL2.cpp \

SRC_TESTS	= 	tests/test_1.cpp \

# ============= RULES ============= #

all: $(NAME) $(NAME_LIB)

$(NAME): $(OBJ_SRC) $(OBJ_MAIN)
	$(COMPILER) -o $(NAME) $(OBJ_SRC) $(OBJ_MAIN) $(FLAGS)

$(NAME_LIB): $(OBJ)
	ar rc $(NAME_LIB) $(OBJ)

graphicals:
	$(COMPILER) -o $(NAME_SDL) -shared -fPIC $(SRC_SDL) $(FLAGS_SDL)

# ============= CLEANS ============= #

clean:
	rm -rf $(OBJ_DIR)
	rm -f *.gcda *.gcno

fclean: clean
	rm -f $(NAME) $(NAME_LIB) unit_tests

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
