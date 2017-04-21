CC = gcc

CFLAGS += \
	-Wall \
	-Wextra \
	-I../src \
	-g \
	-fprofile-arcs -ftest-coverage \


LDFLAGS += \
	-ggdb \


LDLIBS += \
	-lgcov \


OBJ_EXAMPLE = \
	../src/cat.o \
	lib-test.o \
	lib.o \


TARGETS = lib-test

lib-test: $(OBJ_EXAMPLE)

clean:
	rm -fr $(TARGETS) $(OBJ_EXAMPLE)
