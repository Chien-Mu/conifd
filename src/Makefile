PROG := conifd

OBJS := main.o common.o ping.o uci_conifd.o script.o
CFLAGS += -Wall
#CFLAGS += -DDEBUG
LDFLAGS += -luci

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(PROG)
