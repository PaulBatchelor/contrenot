LDFLAGS=-lsporth -lsoundpipe -ljack -lsndfile -lm -lpthread -ldl

default: serial serial.so

serial.so: serial.c wtable.h
	$(CC) serial.c $(CFLAGS) $(LDFLAGS) -shared -fPIC -o $@

serial: serial.c
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -DSTANDALONE -o $@

clean: 
	rm -rf serial.so serial
