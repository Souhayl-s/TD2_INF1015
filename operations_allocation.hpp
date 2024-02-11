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


int obtenirTailleAllocation(const int capacite, const int nElements) {

	bool estCapaciteNulle{ capacite <= 0 }, estCapaciteInsuffisante{ capacite == nElements };

	if (estCapaciteNulle)
		return  1;
	else if (estCapaciteInsuffisante)
		return (2 * capacite);

	return 0; // retourner 0 si aucune allocation de memoire est necessaire
}

void libererMemoireListeFilm(ListeFilms& ListeDeFilmes) { // reference pour que la struct subisse la modification
	if (ListeDeFilmes.capacite != 0)
		delete[] ListeDeFilmes.elements ;
}

void libererMemoireListeActeurs(ListeActeurs& ListeDeActeurs) { // reference pour que la struct subisse la modification
	if (ListeDeActeurs.capacite != 0)
		delete[] ListeDeActeurs.elements;
}


// Permet de copier la liste des filmes / acteurs dans un nouveau tableau de pointeurs de filmes / acteurs

void copierListeDeFilmes(ListeFilms listeDeFilmes, Film** (&nouveauTableauFilmes)) { // par reference pour modifier
	for (int i : range(listeDeFilmes.nElements))
		nouveauTableauFilmes[i] = listeDeFilmes.elements[i];
}

void copierListeDeActeurs(ListeActeurs listeDeActeurs, Acteur** (&nouveauTableauActeurs)) { // par reference pour modifier
	for (int i : range(listeDeActeurs.nElements))
		nouveauTableauActeurs[i] = listeDeActeurs.elements[i];
}