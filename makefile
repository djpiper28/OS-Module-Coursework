FLAGS=$(CFLAGS) -g -std=gnu99 -Wall -Wpedantic -Werror -Wno-unused-variable
SRC_FILES=`find src | grep "\\.c$$" | grep -v "main"`
TEST_FILES=`find test | grep "\\.c$$"`
CC=gcc
ASTYLE_OPTS=--formatted --style=kr

main-src: format
	$(CC) $(FLAGS) $(SRC_FILES) src/main.c -o dlist

test: main-src
	$(CC) $(FLAGS) --coverage $(SRC_FILES) $(TEST_FILES) -o dlist-test
	./dlist-test
	# Generate coverage report
	gcovr --html gcovr.html -x gcovr.xml -s -j -p -r . -e main.c -f "src/*"
	# Run mem checks with valgrind now
	python3 mem_test.py

format:
	find src test | grep "\\.[ch]$$" | xargs astyle $(ASTYLE_OPTS)
	ls . | grep "\\.[ch]$$" | xargs astyle ${ASTYLE_OPTS}

step1: format
	${CC} ${FLAGS} ${SRC_FILES} step_1.c -o step1
	./step1

step2: format
	${CC} ${FLAGS} ${SRC_FILES} step_2.c -o step2
	./step2

step3: format
	${CC} ${FLAGS} ${SRC_FILES} step_3.c -o step3
	./step3

clean:
	rm -f dlist
	rm -f dlist-test
	rm -f *.gcda
	rm -f *.gcno
	rm -f gcovr.html
	rm -f gcovr.xml
	rm -f src/*.orig
	rm -f test/*.orig
	rm -f *.orig
	
