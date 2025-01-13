CC = gcc
CFLAGS = -I ../include

all: display server client

# 編譯 libtool.c -> libtool.o
libtool.o: libtool.c
	$(CC) $(CFLAGS) -c libtool.c -o libtool.o

# 編譯 libipc.c -> libipc.o
libipc.o: libipc.c
	$(CC) $(CFLAGS) -c libipc.c -o libipc.o

# 連結產生 display
display: libipc.o display.c
	$(CC) $(CFLAGS) libipc.o display.c -o display

# 連結產生 server
server: libtool.o libipc.o server.c
	$(CC) $(CFLAGS) libtool.o libipc.o server.c -o server

# 連結產生 client
client: client.c
	$(CC) $(CFLAGS) client.c -o client

# 清理目標檔與執行檔
clean:
	rm -f client server display libipc.o libtool.o

