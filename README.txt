simulationE requires c++17, thus scl enable devtoolset-9 bash is required to compile

to compile all the files:
make all

to compile the files individually:
producer-consumer:
make simulationA

cigarette-smoker:
make simulationE

to clean:
make clean

to run:
producer-consumer:
./simulationA

cigarette-smoker:
./simulationE