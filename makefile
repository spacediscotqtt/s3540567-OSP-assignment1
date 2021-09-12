.default: all

all: 
	g++ -Wall -Werror -pedantic -std=c++11 -g -O -o simulationA -pthread producerconsumer.cpp &
	g++ -Wall -Werror -pedantic -std=c++17 -g -O -o simulationE -pthread agentsmoker.cpp

clean:
	rm -rf simulationA simulationE *.o *.dSYM

simulationA: producerconsumer.cpp
	g++ -Wall -Werror -pedantic -std=c++11 -g -O -o $@ -pthread $^

simulationE: agentsmoker.cpp
	g++ -Wall -Werror -pedantic -std=c++11 -g -O -o $@ -pthread $^