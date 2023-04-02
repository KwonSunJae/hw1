all: hw1.c 
	gcc -o hw1 hw1.c -lsqlite3
clean:
	rm *.db
	rm hw1
