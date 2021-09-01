NAME = msl.exe

MAIN = main.cpp

SRC = MSLNumber.cpp MSLArithmeticOperation.cpp

SRC_FOLDER = src/

all: (NAME)

(NAME):

	g++ -std=c++11 -g $(MAIN) $(addprefix $(SRC_FOLDER), $(SRC))