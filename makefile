server: Server.o Writer.o Protocol.o
	gcc -o server Server.o Writer.o Protocol.o

serverDaemon: DaemonServer.o Writer.o Protocol.o
	gcc -o serverDaemon DaemonServer.o Writer.o Protocol.o

client: Client.o Protocol.o Monitor.o Sender.o 
	gcc -o client Client.o Protocol.o Monitor.o Sender.o

clientDaemon: DaemonClient.o Protocol.o Sender.o 
	gcc -o clientDaemon DaemonClient.o Protocol.o Sender.o

Server.o: Server.c Protocol.h
	gcc -c Server.c

DaemonServer.o: DaemonServer.c Protocol.h
	gcc -c DaemonServer.c

Client.o: Client.c
	gcc -c Client.c

DaemonClient.o: DaemonClient.c
	gcc -c DaemonClient.c

Writer.o: Writer.c Writer.h Protocol.h
	gcc -c Writer.c

Protocol.o: Protocol.c Protocol.h
	gcc -c Protocol.c

Monitor.o: Monitor.c
	gcc -c Monitor.c

Sender.o: Sender.c
	gcc -c Sender.c

clean:
	rm *.o server client