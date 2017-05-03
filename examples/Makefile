CC = gcc

CFLAGS += \
	-Wall \
	-Wextra \
	-I../src \
	-I./src/ \
	-g \
	-fprofile-arcs -ftest-coverage \


LDFLAGS += \
	-ggdb \


LDLIBS += \
	-lgcov \


OBJ_EXAMPLE_1 = \
	../src/cat.o \
	./tests/test-example-1.o \


OBJ_EXAMPLE_2 = \
	../src/cat.o \
	./tests/test-example-2.o \


OBJ_EXAMPLE_3 = \
	../src/cat.o \
	./tests/test-example-3.o \
	./src/prod.o


OBJ_EXAMPLE_4 = \
	../src/cat.o \
	./tests/test-example-4.o


OBJ_EXAMPLE_5 = \
	../src/cat.o \
	./tests/test-example-5.o


TARGETS = \
	test-example-1 \
	test-example-2 \
	test-example-3 \
	test-example-4 \
	test-example-5 \


all: $(TARGETS)

test-example-1: $(OBJ_EXAMPLE_1)
	$(CC) $(LDFLAGS) $(OBJ_EXAMPLE_1) $(LDLIBS) -o $@

test-example-2: $(OBJ_EXAMPLE_2)
	$(CC) $(LDFLAGS) $(OBJ_EXAMPLE_2) $(LDLIBS) -o $@

test-example-3: $(OBJ_EXAMPLE_3)
	$(CC) $(LDFLAGS) $(OBJ_EXAMPLE_3) $(LDLIBS) -o $@

test-example-4: $(OBJ_EXAMPLE_4)
	$(CC) $(LDFLAGS) $(OBJ_EXAMPLE_4) $(LDLIBS) -o $@

test-example-5: $(OBJ_EXAMPLE_5)
	$(CC) $(LDFLAGS) $(OBJ_EXAMPLE_5) $(LDLIBS) -o $@

clean:
	rm -fr $(TARGETS) $(OBJ_EXAMPLE_1) $(OBJ_EXAMPLE_2) \
		$(OBJ_EXAMPLE_3) $(OBJ_EXAMPLE_4) \
		tests/*.gcda tests/*.gcno \
		src/*.gcda src/*.gcno \
		../src/*.gcda ../src/*.gcno \


.PHONY: all clean
