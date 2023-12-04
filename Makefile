all:
	cc dns_packet.c load_config.c net_functions.c set.c main.c -o serverrun
clean:
	rm -f *.o *.mk
