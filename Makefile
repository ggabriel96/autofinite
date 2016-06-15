all: *.h *.cpp
	g++ *.cpp -std=c++11 -Wall -O2

clean:
	rm a.out
