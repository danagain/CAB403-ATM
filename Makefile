CCFLAGS = -ggdb -pthread

server: server.c 
	gcc $(CCFLAGS) server.c -o server
	
client: client.c
	gcc -ggdb client.c -o client
	
clean:
	rm -rf server client 
