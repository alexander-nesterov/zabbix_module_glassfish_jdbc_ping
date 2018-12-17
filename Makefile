NAME_LIBRARY=glassfish-jdbc.so
ROOT_SOURCES=src
SOURCES=$(ROOT_SOURCES)/module.c $(ROOT_SOURCES)/glassfish-jdbc.c
ZBX_INCLUDE=../../../include
CURL_INCLUDE=../../../include
PCRE_INCLUDE=../../../include
CFLAGS=-m64 -fPIC -shared -Wall
LDFLAGS=-lcurl -lpcre

all:
	gcc $(CFLAGS) $(LDFLAGS) -o $(NAME_LIBRARY) $(SOURCES) -I$(ZBX_INCLUDE) -I$(CURL_INCLUDE) -I$(PCRE_INCLUDE)
clean:
	rm -rf *.so