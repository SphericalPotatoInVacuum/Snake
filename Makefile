#OBJS specifies which files to compile as part of the project
OBJS = src/*.cpp 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -Wall -O3 -std=c++17

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Build/SnakeGame

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) -I/usr/include/  $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

clean : 
	rm *.o
