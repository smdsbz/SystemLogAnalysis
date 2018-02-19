CC = clang++
PRODUCT = a.out
FLAGS = -Wall -std=c++1z -o $(PRODUCT)
SOURCE = ./utils/LogClass/*.cpp \
		./*.cpp
CALL_ARGS = ./test_env/modified_syslog.txt


main:
	clear
	$(CC) $(FLAGS) $(SOURCE)
	./$(PRODUCT) $(CALL_ARGS)
	rm $(PRODUCT)

debug:
	$(CC) $(FLAGS) -g $(SOURCE)
	lldb ./$(PRODUCT)

