TARGETS = clock_setres select-timeout sleep-t socket-t timeouts


all: $(TARGETS)

clean:
	rm -f $(TARGETS)
