FLAGS=$(shell mysql_config --cflags) $(shell mysql_config --libs)
all:
	g++ ./main.cc $(FLAGS) -o weaponsCache 
clean:
	rm ./weaponsCache
