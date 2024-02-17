#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS 

#include "structures.hpp"      
#include "mes_fonctions.hpp" 

#include <string>
#include <iostream>
#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}
/* Trouver la taille de la memoire a allouer l'espace restant dans un tableau */

int determinerTailleAllocation(const int capacite, const int nElements) {

	bool estCapaciteNulle{ capacite <= 0 }, estCapaciteInsuffisante{ capacite == nElements };

	if (estCapaciteNulle)
		return  CAPACITE_1;
	else if (estCapaciteInsuffisante)
		return (DOUBLER_CAPACITE * capacite);

	return CAPACITE_NULLE; // retourner 0 si aucune allocation de memoire est necessaire
}


/* Trouver si un acteur est dans un film  */

// Permet de retourne le pointeur vers l'acteur mais aussi determiner avec le bool si l'acteur est present dans un film

Acteur* ObtenirPtrActeurDansFilme(Film* ptrFilme , string nomActeurCible) {
	span <Acteur*> SpanListeDeActeurs = {obtenirListeSpecifiquePtrActeurs(ptrFilme) , size_t(obtenirCapaciteActeurDeFilm(ptrFilme))};
	size_t nombreActeurs = (ptrFilme->acteurs).nElements;
	for (Acteur* ptrActeur : SpanListeDeActeurs.first(nombreActeurs)) { // Acteurs specifques a un film
		string nomActeurPotentiel = ptrActeur -> nom;
		if (nomActeurPotentiel == nomActeurCible)
			return ptrActeur;
	}
	return nullptr;
}
const string IDENTATION = "\t\t\t\t\t\t\t\t"; 
void AffichageCollectionPrincipale(ListeFilms collectionPrincipale) {
	for (int i : range(collectionPrincipale.capacite)) {
		Film* ptrFilm = obtenirPtrFilme(collectionPrincipale,i); 
		cout << ptrFilm->titre << endl << endl;
		ListeActeurs ListeActeurDuFilm = ptrFilm->acteurs;
		for (int j : range(ListeActeurDuFilm.capacite)) {
			Acteur* ptrActeurDuFilm = obtenirPtrActeur(ListeActeurDuFilm, j);
			cout << "      " << ptrActeurDuFilm->nom << endl;
			ListeFilms ListeFilmeDeActeur = ptrActeurDuFilm->joueDans;
			cout << IDENTATION;
			for (int k : range(ListeFilmeDeActeur.capacite)) {
				Film* ptrFilmDeActeur = obtenirPtrFilme(ListeFilmeDeActeur,k);
				cout << ptrFilmDeActeur->titre << "   *   ";
			}
			cout << endl << endl;
		}
		cout << "\n/////////////////////////////////////////////////////////////////////////////////////////////////////\n" << endl;;
	}
}