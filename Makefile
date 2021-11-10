default:
	build

build:
	gcc src/main.c -o snake_game -l ncurses

run:
	./snake_game

clean:
	rm snake_game

