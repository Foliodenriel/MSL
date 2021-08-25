NAME = msl.exe

MAIN = main.cpp

SRC = MSLNumber.cpp

SRC_FOLDER = src/

all: (NAME)

(NAME):

	g++ -std=c++11 $(MAIN) $(addprefix $(SRC_FOLDER), $(SRC))