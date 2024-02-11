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

// Obtenir film / acteur

Film** obtenirTableauFilme(const ListeFilms& listeDeFilmes) {
	return listeDeFilmes.elements;
}

Acteur** obtenirTableauActeur(const ListeActeurs& listeDeActeurs) {
	return listeDeActeurs.elements;
}


Film* obtenirPtrFilme(const ListeFilms& listeDeFilmes, int index) {
	return listeDeFilmes.elements[index];
}

Film obtenirObjetFilm(const ListeFilms& listeDeFilmes, int index) {
	return *(obtenirPtrFilme(listeDeFilmes, index));
}

Acteur* obtenirPtrActeur(const ListeActeurs& listeDeActeurs, int index) {
	return listeDeActeurs.elements[index];
}

Acteur obtenirObjetActeur(const ListeActeurs& listeDeActeurs, int index) {
	return *(obtenirPtrActeur(listeDeActeurs, index));
}


// 
int determinerTailleAllocation(const int capacite, const int nElements) {

	bool estCapaciteNulle{ capacite <= 0 }, estCapaciteInsuffisante{ capacite == nElements };

	if (estCapaciteNulle)
		return  1;
	else if (estCapaciteInsuffisante)
		return (2 * capacite);

	return 0; // retourner 0 si aucune allocation de memoire est necessaire
}

// Liberation memoire

void libererMemoireListeFilm(ListeFilms& ListeDeFilmes) { // reference pour que la struct subisse la modification
	if (ListeDeFilmes.capacite != 0)
		delete[] obtenirTableauFilme(ListeDeFilmes);
}

void libererMemoireListeActeurs(ListeActeurs& ListeDeActeurs) { // reference pour que la struct subisse la modification
	if (ListeDeActeurs.capacite != 0)
		delete[] obtenirTableauActeur(ListeDeActeurs);
}


// Permet de copier la liste des filmes / acteurs dans un nouveau tableau de pointeurs de filmes / acteurs

void copierListeDeFilmes(const ListeFilms &listeDeFilmes, Film** (&nouveauTableauFilmes)) { // par reference pour modifier
	for (int i : range(listeDeFilmes.nElements))
		nouveauTableauFilmes[i] = obtenirPtrFilme(listeDeFilmes,i);
}

void copierListeDeActeurs(const ListeActeurs &listeDeActeurs, Acteur** (&nouveauTableauActeurs)) { // par reference pour modifier
	for (int i : range(listeDeActeurs.nElements))
		nouveauTableauActeurs[i] = obtenirPtrActeur(listeDeActeurs, i);
}

