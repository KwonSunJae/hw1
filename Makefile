all: hw1.c 
	gcc -o newhw1 newh1.c -lsqlite3
clean:
	rm *.db
	rm hw1
