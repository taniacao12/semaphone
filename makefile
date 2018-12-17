all:
	gcc -o control.out control.c
	gcc -o main.out main.c
	./main.out
clean:
	rm -rf *~
	rm -rf *.out