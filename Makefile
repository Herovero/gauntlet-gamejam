# The binary this exercise builds
NAME := game

# Compiler and the gauntlet's mandatory flags
CXX      := c++
CXXFLAGS := -Wall -Wextra -Werror -std=c++17

# Raylib and Linux system libraries
LDLIBS   := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# List every .cpp file in the exercise here
SRCS := main.cpp WauBulan.cpp SwingingKid.cpp Obstacle.cpp
OBJS := $(SRCS:.cpp=.o)

# `make` / `make all`: build the binary
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LDLIBS)

# Compile each .cpp into a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# `make clean`: remove object files
clean:
	rm -f $(OBJS)

# `make fclean`: remove objects AND the binary
fclean: clean
	rm -f $(NAME)

# `make re`: rebuild everything from scratch
re: fclean all

.PHONY: all clean fclean re