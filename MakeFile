all:
	gcc -pthread -o server server.c 
	gcc -o client client.c 

clientgdb:
	gcc -o client client.c 

servergdb:
	gcc -pthread -o server server.c 


clean:
	rm server
	rm client
