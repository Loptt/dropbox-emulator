server: Server.o Writer.o Protocol.o
	gcc -o server Server.o Writer.o Protocol.o

client: Client.o Protocol.o
	gcc -o client Client.o Protocol.o

Server.o: Server.c Protocol.h
	gcc -c Server.c

Writer.o: Writer.c Writer.h Protocol.h
	gcc -c Writer.c

Protocol.o: Protocol.c Protocol.h
	gcc -c Protocol.c