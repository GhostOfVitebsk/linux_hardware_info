MAKEFLAGS += --silent

CC = g++
LIBS = -ludev

lhi: source.cpp
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

.PHONY: clean install

clean:
	rm -f lhi 

install:
	cp lhi /usr/local/bin
	rm -f lhi