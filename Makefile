CC = g++
CFLAGS = -std=c++11 -Wall -g3

all: merak16-as

merak16-as: merak16-as.cpp
	$(CC) $(CFLAGS) -o merak16-as merak16-as.cpp

clean:
	rm -f merak16-as
