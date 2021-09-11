.default: all

all: 

clean:
	rm -rf A E *.o *.dSYM

A: producerconsumer.cpp
	g++ -Wall -Werror -pedantic -std=c++11 -g -O -o $@ -pthread $^

E: agentsmoker.cpp
	g++ -Wall -Werror -pedantic -std=c++11 -g -O -o $@ -pthread $^


