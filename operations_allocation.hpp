#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS 

#include "structures.hpp"       

#include <string>
#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

/* Obtention  ptr et tableau (Acteur / Films )  */

// Obtenir Tableaux de pointeurs

Film** obtenirTableauFilme(const ListeFilms& listeDeFilmes) {
	return listeDeFilmes.elements;
}

Acteur** obtenirTableauActeur(const ListeActeurs& listeDeActeurs) {
	return listeDeActeurs.elements;
}

//  Obtenir Pointeur film a partir de l'index du tableau

Film* obtenirPtrFilme(const ListeFilms& listeDeFilmes, int index) {
	return listeDeFilmes.elements[index];
}


//  Obtenir Pointeur acteur a partir de l'index du tableau

Acteur* obtenirPtrActeur(const ListeActeurs& listeDeActeurs, int index) {
	return listeDeActeurs.elements[index];
}


/* Obtention des attributs d'un objet par pointeur */


// Retourner la structure ListeFilms d'un acteur a partir d'un pointeur




//Obtenir la capacite d'une liste film
int obtenirCapaciteActeur(Acteur* ptrActeur) {
	return (ptrActeur->joueDans).capacite;
}

//Obtenir la capacite d'une liste acteur

int obtenirCapaciteFilm(Film* ptrFilm) {
	return (ptrFilm->acteurs).capacite;
}

//Obtenir le nElements d'une liste film
int obtenirNelementsActeur(Acteur* ptrActeur) {
	return (ptrActeur->joueDans).nElements;
}

//Obtenir le nElements d'une liste acteur

int obtenirNelementsFilm(Film* ptrFilm) {
	return (ptrFilm->acteurs).nElements;
}

//Obtenir le tableau de pointeurs de films associe a un acteur

Film** obtenirListeSpecifiquePtrFilmes(Acteur* ptrActeur) {
	return (ptrActeur->joueDans).elements;
}

//Obtenir le tableau de pointeurs d'Acteurs / Film associe a un film

Acteur** obtenirListeSpecifiquePtrActeurs(Film* ptrFilm) {
	return (ptrFilm->acteurs).elements;
}

/* Copie d'un tableau  de pointeurs film / objets dans une ListeDeFilm / Liste Acteurs  */

void copierListeDeFilmes(const ListeFilms &listeDeFilmes, Film** (&nouveauTableauFilmes)) { // par reference pour modifier
	for (int i : range(listeDeFilmes.nElements))
		nouveauTableauFilmes[i] = obtenirPtrFilme(listeDeFilmes,i);
}

void copierListeDeActeurs(const ListeActeurs &listeDeActeurs, Acteur** (&nouveauTableauActeurs)) { // par reference pour modifier
	for (int i : range(listeDeActeurs.nElements))
		nouveauTableauActeurs[i] = obtenirPtrActeur(listeDeActeurs, i);
}

/* Trouver si un acteur est dans un film  */

// Permet de retourne le pointeur vers l'acteur mais aussi determiner avec le bool si l'acteur est present dans un film
Acteur* ObtenirPtrActeurDansFilme(Film* ptrFilme, string nomActeurCible) ;


/* Operation  memoire */

int determinerTailleAllocation(const int capacite, const int nElements);

void libererMemoireListeFilm(ListeFilms& ListeDeFilmes) { // reference pour que la struct subisse la modification
	if (ListeDeFilmes.capacite != 0)
		delete[] obtenirTableauFilme(ListeDeFilmes);
}

void libererMemoireListeActeurs(ListeActeurs& ListeDeActeurs) { // reference pour que la struct subisse la modification
	if (ListeDeActeurs.capacite != 0)
		delete[] obtenirTableauActeur(ListeDeActeurs);
}

