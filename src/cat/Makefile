FLAGS = -Wall -Werror -Wextra -std=c11

SOURCES := s21_cat.c test.c

OBJECTS := s21_cat test

all: s21_cat test

.c.o: 
	$(FLAGC)  $< -o $@

rebuild: clean all

s21_cat: s21_cat.c
	gcc $(FLAGS) s21_cat.c -o s21_cat

test: $(OBJECTS)
	gcc $(FLAGS) test.c -o test
	./test

style:
	cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	python3 ../../materials/linters/cpplint.py --extension=c *.c *.h

leaks:
	leaks -atExit -- s21_cat reflect 1

clean:
	rm -rf $(OBJECTS) CPPLINT.cfg result_1 result_2
