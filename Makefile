all : serveur client

serveur : serveur.c fonctions.h
	gcc  $< -o $@

clean :
	\rm -f serveur *.o
