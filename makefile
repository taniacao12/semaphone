all:
	gcc -o main.out main.c
run:
	./main.out
clean:
	rm -rf *~
	rm -rf *.out