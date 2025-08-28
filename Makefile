NAME = scop

CXX = g++ -g -fsanitize=address
CC = gcc
CXXFLAGS = -Wall -Wextra -Werror -I./include
CFLAGS = -Wall -Wextra -Werror -I./include
LDFLAGS =

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin) # macOS
	CXXFLAGS += -I/usr/local/include -I/opt/homebrew/include
	CXXFLAGS += -Wno-deprecated-declarations
    LDFLAGS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    LDFLAGS += -L/usr/local/lib -lglfw -L/opt/homebrew/lib
else ifeq ($(UNAME_S),Linux) # Linux
	LDFLAGS += -lglfw -lGL -lX11 -lXi -lXrandr -lXxf86vm -lpthread -ldl -lrt -lXcursor
endif

SRCS_CPP = src/scop.cpp \
		src/math.cpp \
		src/model.cpp \
		src/objectLoader.cpp \
		src/shader.cpp \

SRCS_C = src/glad.c

OBJS_CPP = $(SRCS_CPP:.cpp=.o)
OBJS_C = $(SRCS_C:.c=.o)
OBJS = $(OBJS_CPP) $(OBJS_C)

all: $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(NAME)
clean:
	rm -rf $(OBJS)
fclean: clean
	rm -rf $(NAME)
re: fclean all
.PHONY: all clean fclean re