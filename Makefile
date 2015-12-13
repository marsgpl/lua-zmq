#

NAME = zmq
INSTALL_DIR = /home/pd/lib/lua

CC = gcc
LD = gcc
CCFLAGS = -c -fPIC -m64 -std=gnu99 -O2 -Wall -Werror
LDFLAGS = -shared
INCS = -I/home/pd/inc
LIBS = -L/home/pd/lib -lzmq

MAIN_T = $(NAME).so
MAIN_O = lua_$(NAME).o

build: $(MAIN_T)

clean:
	rm -f $(MAIN_T) $(MAIN_O)

install:
	cp $(MAIN_T) $(INSTALL_DIR)

$(MAIN_T): $(MAIN_O)
	$(LD) $(LDFLAGS) -o $@ $^ $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) -o $@ $< $(INCS)

lua_$(NAME).o: lua_pd.h lua_$(NAME).h lua_$(NAME)_flags.c
