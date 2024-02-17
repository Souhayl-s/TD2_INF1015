#pragma once

#include "structures.hpp"       
#include <string>


using namespace std;


const int CAPACITE_NULLE = 0;
const int CAPACITE_1= 1;
const int DOUBLER_CAPACITE = 2;

/* Obtention  ptr et tableau (Acteur / Films )  */

// Obtenir Tableaux de pointeurs

Film** obtenirTableauFilme(const ListeFilms& listeDeFilmes) ;


Acteur** obtenirTableauActeur(const ListeActeurs& listeDeActeurs) ;


//  Obtenir Pointeur film a partir de l'index du tableau

Film* obtenirPtrFilme(const ListeFilms& listeDeFilmes, int index) ;


//  Obtenir Pointeur acteur a partir de l'index du tableau

Acteur* obtenirPtrActeur(const ListeActeurs& listeDeActeurs, int index);


/* Obtention des attributs d'un objet par pointeur */



//Obtenir la capacite d'une liste film
int obtenirCapaciteFilmDeActeur(Acteur * ptrActeur); 


//Obtenir la capacite d'une liste acteur

int obtenirCapaciteActeurDeFilm(Film* ptrFilm);

//Obtenir le nElements d'une liste film
int obtenirNelementsFilmDeActeur(Acteur* ptrActeur);

//Obtenir le nElements d'une liste acteur

int obtenirNelementsActeurDeFilm(Film* ptrFilm);


//Obtenir le tableau de pointeurs de films associe a un acteur

Film** obtenirListeSpecifiquePtrFilmes(Acteur* ptrActeur);

//Obtenir le tableau de pointeurs d'Acteurs / Film associe a un film

Acteur** obtenirListeSpecifiquePtrActeurs(Film* ptrFilm);


/* Copie d'un tableau  de pointeurs film / objets dans une ListeDeFilm / Liste Acteurs  */

void copierListeDeFilmes(const ListeFilms& listeDeFilmes, Film** (&nouveauTableauFilmes));

void copierListeDeActeurs(const ListeActeurs& listeDeActeurs, Acteur** (&nouveauTableauActeurs));


/* Trouver si un acteur est dans un film  */

// Permet de retourne le pointeur vers l'acteur mais aussi determiner avec le bool si l'acteur est present dans un film
Acteur* ObtenirPtrActeurDansFilme(Film* ptrFilme, string nomActeurCible) ;


/* Operation  memoire */

int determinerTailleAllocation(const int capacite, const int nElements);

void libererMemoireListeFilm(ListeFilms& ListeDeFilmes);


void libererMemoireListeActeurs(ListeActeurs& ListeDeActeurs);

