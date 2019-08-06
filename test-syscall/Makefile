cs300_testapp_OBJ:= cs300_testapp.o
process_ancestors_OBJ:= process_ancestors.o
array_stats_OBJ:= array_stats_test.o

all = cs300_testapp test_process_ancestors test_array_stats

CFLAGS = -std=c99 -g -Wall -Werror -D _GNU_SOURCE -static
CC_C = gcc

%.o : %.c
	$(CC) -c $(CFLAGS) $<

cs300_testapp: $(cs300_testapp_OBJ)
	$(CC_C) $(CFLAGS) $(cs300_testapp_OBJ) -o$@ $<
test_process_ancestors: $(process_ancestors_OBJ)
	$(CC_C) $(CFLAGS) $(process_ancestors_OBJ) -o$@ $<
test_array_stats: $(array_stats_OBJ)
	$(CC_C) $(CFLAGS) $(array_stats_OBJ) -o$@ $<

clean:
	rm -f *.o

transfer:
	scp -P 2222 $(all) root@localhost:$(all)

killqemu:
	killall qemu-system-x86_64

.PHONY: all clean
