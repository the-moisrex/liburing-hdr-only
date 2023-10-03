CPPFLAGS ?=
override CPPFLAGS += -D_GNU_SOURCE -I../liburing/ -g -O0 -std=c17 -Wall -Wextra -Wpedantic
CFLAGS ?= -g -O0 -Wall
LDFLAGS ?=
override LDFLAGS += -L../liburing/

# include ../Makefile.quiet

# ifneq ($(MAKECMDGOALS),clean)
# include ../config-host.mak
# endif

LDFLAGS ?=
override LDFLAGS += -L../liburing/ -lpthread

example_srcs := \
	io_uring-close-test.c \
	io_uring-cp.c \
	io_uring-test.c \
	io_uring-udp.c \
	link-cp.c \
	poll-bench.c \
	send-zerocopy.c \
	rsrc-update-bench.c

all_targets :=


ifdef CONFIG_HAVE_UCONTEXT
	example_srcs += ucontext-cp.c
endif
all_targets += ucontext-cp

example_targets := $(patsubst %.c,%,$(patsubst %.cc,%,$(example_srcs)))
all_targets += $(example_targets)


all: $(example_targets)

%: %.c ../liburing
	$(QUIET_CC)$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	@rm -f $(all_targets)

.PHONY: all clean
