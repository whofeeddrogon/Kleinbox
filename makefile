all: compile move

move: Kleinbox
		mv Kleinbox $(HOME)

compile: Kleinbox.c
		gcc Kleinbox.c -o Kleinbox
