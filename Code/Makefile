all: lxx2021

lxx2021: lxx2021.o
	g++ -o $@ $^ `root-config --libs`
	
%.o : %.cpp
	g++ -o $@ -c $^ `root-config --cflags`

clean :
	rm -f *.o lxx2021
