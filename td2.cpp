#pragma region "Includes"//{
#define _CRT_SECURE_NO_WARNINGS // On permet d'utiliser les fonctions de copies de chaînes qui sont considérées non sécuritaires.

#include "structures.hpp"      // Structures de données pour la collection de films en mémoire.
#include "mes_fonctions.hpp" 

#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>

#include "cppitertools/range.hpp"
#include "gsl/span"

#include "bibliotheque_cours.hpp"
#include "verification_allocation.hpp" // Nos fonctions pour le rapport de fuites de mémoire.
#include "debogage_memoire.hpp"        // Ajout des numéros de ligne des "new" dans le rapport de fuites.  Doit être après les include du système, qui peuvent utiliser des "placement new" (non supporté par notre ajout de numéros de lignes).

using namespace std;
using namespace iter;
using namespace gsl;

#pragma endregion//}

typedef uint8_t UInt8;
typedef uint16_t UInt16;

#pragma region "Fonctions de base pour lire le fichier binaire"//{
template <typename T>
T lireType(istream& fichier)
{
	T valeur{};
	fichier.read(reinterpret_cast<char*>(&valeur), sizeof(valeur));
	return valeur;
}
#define erreurFataleAssert(message) assert(false&&(message)),terminate()
static const uint8_t enteteTailleVariableDeBase = 0xA0;
size_t lireUintTailleVariable(istream& fichier)
{
	uint8_t entete = lireType<uint8_t>(fichier);
	switch (entete) {
	case enteteTailleVariableDeBase + 0: return lireType<uint8_t>(fichier);
	case enteteTailleVariableDeBase + 1: return lireType<uint16_t>(fichier);
	case enteteTailleVariableDeBase + 2: return lireType<uint32_t>(fichier);
	default:
		erreurFataleAssert("Tentative de lire un entier de taille variable alors que le fichier contient autre chose à cet emplacement.");
	}
}

string lireString(istream& fichier)
{
	string texte;
	texte.resize(lireUintTailleVariable(fichier));
	fichier.read((char*)&texte[0], streamsize(sizeof(texte[0])) * texte.length());
	return texte;
}

#pragma endregion//}


//TODO: Une fonction pour ajouter un Film à une ListeFilms, le film existant déjà; on veut uniquement ajouter le pointeur vers le film existant.  Cette fonction doit doubler la taille du tableau alloué, avec au minimum un élément, dans le cas où la capacité est insuffisante pour ajouter l'élément.  Il faut alors allouer un nouveau tableau plus grand, copier ce qu'il y avait dans l'ancien, et éliminer l'ancien trop petit.  Cette fonction ne doit copier aucun Film ni Acteur, elle doit copier uniquement des pointeurs.
void ajouterFilm(ListeFilms& listeDeFilms, Film* ptrNouveauFilm) {

	const int capacite = listeDeFilms.capacite;
	const int nElements = listeDeFilms.nElements;
	const int indexNouvelElement = listeDeFilms.nElements;// index du nouveau film dans la liste de films

	/*1- Determiner dans quel cas d'allocation on est*/
	int tailleAllocation = determinerTailleAllocation(capacite,nElements) ;

	/*2- Allocation et Liberation de Memoire*/
	
	// Allocation d'un nouveau tableau pointeurs de Films

	bool estAllocationNecessaire = tailleAllocation ;
	if (estAllocationNecessaire) {
		Film* *nouveauTableauFilmes = new Film* [tailleAllocation];  
		listeDeFilms.capacite = tailleAllocation; 
		// Cas insuffisant 
		bool estCapaciteInsuffisante = (tailleAllocation > 1);
		if (estCapaciteInsuffisante) {
			// copie des elements a partir de l'ancien tableau
			copierListeDeFilmes(listeDeFilms, nouveauTableauFilmes);
			// liberer la memoire pris par l'ancien tableau de pointeurs
			libererMemoireListeFilm(listeDeFilms);
		}
		// Mise a jour de l'attribut Elements
		listeDeFilms.elements = nouveauTableauFilmes;
	}
	listeDeFilms.elements[indexNouvelElement] = ptrNouveauFilm; // assigner un pointeur qui a l'adresse du nouveau film
	++listeDeFilms.nElements; // incrementer le nombre d'elements

}

//TODO: Une fonction pour enlever un Film d'une ListeFilms (enlever le pointeur) sans effacer le film; la fonction prenant en paramètre un pointeur vers le film à enlever.  L'ordre des films dans la liste n'a pas à être conservé.
void retirerFilm(ListeFilms& listeDeFilms, Film* ptrFilmeExclu) {
	const int nElements = listeDeFilms.nElements;
	for (int i : range(nElements)) {
		bool estFilmCorrespondant = (obtenirPtrFilme(listeDeFilms, i) == ptrFilmeExclu);
		if (estFilmCorrespondant) {
			listeDeFilms.elements[i] = nullptr; // ne point vers rien 
			--listeDeFilms.nElements; // decrementer le nombre d'elements
			break;
		}			
	}
}


//TODO: Une fonction pour trouver un Acteur par son nom dans une ListeFilms, qui retourne un pointeur vers l'acteur, ou nullptr si l'acteur n'est pas trouvé.  Devrait utiliser span.

Acteur* ObtenirPtrActeurDansListeFilme( const ListeFilms& listeDeFilmes, string nomActeurCible) {
	span <Film*> SpanListeDeFilmes = { obtenirTableauFilme(listeDeFilmes) , size_t(listeDeFilmes.capacite) };
	for (Film* ptrFilm : SpanListeDeFilmes.first(listeDeFilmes.nElements) ) {
		Acteur* PtrActeur = ObtenirPtrActeurDansFilme(ptrFilm, nomActeurCible);
		bool estDansFilme = (PtrActeur != nullptr); // la fonction retourne null ptr si l'acteur ne joue pas dans le film
		if (estDansFilme)
			return PtrActeur;
	}
	return nullptr;
	}

//TODO: Compléter les fonctions pour lire le fichier et créer/allouer une ListeFilms.  La ListeFilms devra être passée entre les fonctions, pour vérifier l'existence d'un Acteur avant de l'allouer à nouveau (cherché par nom en utilisant la fonction ci-dessus).
Acteur* lireActeur(istream& fichier , ListeFilms &listeDeFilmes)
{	
	Acteur acteur = {};
	acteur.nom = lireString(fichier);
	acteur.anneeNaissance = int(lireUintTailleVariable(fichier));
	acteur.sexe = char(lireUintTailleVariable(fichier));

	Acteur* ptrActeur = ObtenirPtrActeurDansListeFilme(listeDeFilmes, acteur.nom); 
	bool estDansListeFilmes = (ptrActeur != nullptr);
	if (estDansListeFilmes)
		return ptrActeur; 
	// cas ou l'acteur n'est pas present dans la liste

	ptrActeur = new Acteur(acteur); // initialiser le pointeur avec la bonne valeur en allouant dynamiquement
	(ptrActeur->joueDans).nElements = (ptrActeur->joueDans).capacite = CAPACITE_NULLE; // initialiser a 0 le nb elements et la capacite de la liste de filme de l'acteur
	cout << ptrActeur->nom << endl ;
	return ptrActeur; //TODO: Retourner un pointeur soit vers un acteur existant ou un nouvel acteur ayant les bonnes informations, selon si l'acteur existait déjà.  Pour fins de débogage, affichez les noms des acteurs crées; vous ne devriez pas voir le même nom d'acteur affiché deux fois pour la création.

}

Film* lireFilm(istream& fichier , ListeFilms& listeDeFilmes)
{
	Film film = {};
	film.titre = lireString(fichier);
	film.realisateur = lireString(fichier);
	film.anneeSortie = int(lireUintTailleVariable(fichier));
	film.recette = int(lireUintTailleVariable(fichier));
	film.acteurs.nElements = int(lireUintTailleVariable(fichier));  //NOTE: Vous avez le droit d'allouer d'un coup le tableau pour les acteurs, sans faire de réallocation comme pour ListeFilms.  Vous pouvez aussi copier-coller les fonctions d'allocation de ListeFilms ci-dessus dans des nouvelles fonctions et faire un remplacement de Film par Acteur, pour réutiliser cette réallocation.
	
	// allouer memoire pour un objet film dans la memoire en l'intialisant a la bonne valeur
	Film* ptrFilm = new Film(film);
	(ptrFilm->acteurs).capacite = obtenirNelementsActeurDeFilm(ptrFilm); // On connait la capacite a l'avance pour chaque film
	// allouer la memoire pour le tableau Acteur*
	Acteur** tableauPtrActeurs = new Acteur * [obtenirCapaciteActeurDeFilm(ptrFilm)];
	// maj attribut elements de l'objet film
	(ptrFilm->acteurs).elements = tableauPtrActeurs;
	
	for (int i = 0; i < film.acteurs.nElements; i++) {
		Acteur *ptrActeur = lireActeur(fichier , listeDeFilmes); 
		//TODO: Placer l'acteur au bon endroit dans les acteurs du film.
		obtenirListeSpecifiquePtrActeurs(ptrFilm)[i] = ptrActeur; 
		//TODO: Ajouter le film à la liste des films dans lesquels l'acteur joue.
		ajouterFilm(ptrActeur->joueDans, ptrFilm); 
	}
	return ptrFilm; //TODO: Retourner le pointeur vers le nouveau film.
}

ListeFilms creerListe(string nomFichier)
{
	ifstream fichier(nomFichier, ios::binary);
	fichier.exceptions(ios::failbit);

	int nElements = int(lireUintTailleVariable(fichier));
	//TODO: Créer une liste de films vide.
	ListeFilms listeDeFilmes = {};
	listeDeFilmes.nElements = 0; listeDeFilmes.capacite = nElements;
	listeDeFilmes.elements = new Film * [nElements]; 
	for (int i = 0; i < nElements; i++) {
		Film* ptrNouveauFilm = lireFilm(fichier, listeDeFilmes); //TODO: Ajouter le film à la liste.
		ajouterFilm(listeDeFilmes, ptrNouveauFilm);
	}
	return listeDeFilmes; //TODO: Retourner la liste de films.
}

//TODO: Une fonction pour détruire un film (relâcher toute la mémoire associée à ce film, et les acteurs qui ne jouent plus dans aucun films de la collection).  Noter qu'il faut enleve le film détruit des films dans lesquels jouent les acteurs.  Pour fins de débogage, affichez les noms des acteurs lors de leur destruction.


void detruireFilm(ListeFilms& listeDeFilmes ,Film* ptrFilmExclue) {
	int capaciteActeursDeFilme = obtenirCapaciteActeurDeFilm(ptrFilmExclue); 
	span <Acteur*> spanActeurDeFilme = { obtenirListeSpecifiquePtrActeurs(ptrFilmExclue),size_t(capaciteActeursDeFilme) };
	// Detruire l'acteur du filme 
	for (Acteur* ptrActeur : spanActeurDeFilme) {
		int nombreDeFilmesRestants = obtenirNelementsFilmDeActeur(ptrActeur) ;
		if (nombreDeFilmesRestants == 1) {// c'est le seul filme qui reste
			delete[] obtenirListeSpecifiquePtrFilmes(ptrActeur); // desallouer le tableau de pointeurs de Filmes de l'acteur	
			delete	 ptrActeur; // desallouer l'objet acteur
			ptrActeur = nullptr; // retirer l'acteur
		}
		else {
			--(ptrActeur->joueDans).nElements; // decrementer le nombre films de l'acteur en question
			ListeFilms listeFilmesDeActeur = (ptrActeur->joueDans); // liste de filmes specifique a l'acteur
			retirerFilm(listeFilmesDeActeur, ptrFilmExclue);
		}
	}
	delete[] obtenirListeSpecifiquePtrActeurs(ptrFilmExclue); // Desallouer le tableau de Acteur* (liste qu'on vient de parcourir)
	retirerFilm(listeDeFilmes, ptrFilmExclue); // mettre a nullptr le filme de la collection principale
	delete ptrFilmExclue; // Desallouer l'objet film considere
}

//TODO: Une fonction pour détruire une ListeFilms et tous les films qu'elle contient.


void detruireCollectionPrincipale(ListeFilms& listeDeFilmes) {
	span <Film*> collectionPrincipaleFilmes = { listeDeFilmes.elements , size_t(listeDeFilmes.capacite) };
	for (Film* ptrFilm : collectionPrincipaleFilmes ) {
		bool estFilmeExistant = (ptrFilm != nullptr);
		if (estFilmeExistant) {
			detruireFilm(listeDeFilmes, ptrFilm); // on detruit le film est tout ce qui lui est associe
			}
	}
	libererMemoireListeFilm(listeDeFilmes); // desallouer pour la collection principale
}


void afficherActeur(const Acteur& acteur)
{
	cout << "  " << acteur.nom << ", " << acteur.anneeNaissance << " " << acteur.sexe << endl;
}

//TODO: Une fonction pour afficher un film avec tous ces acteurs (en utilisant la fonction afficherActeur ci-dessus).

void afficherFilm(Film* ptrFilm) {
	bool estFilmeExistant = (ptrFilm != nullptr);
	if (estFilmeExistant) {
		Acteur** tableauActeursDuFilme = obtenirListeSpecifiquePtrActeurs(ptrFilm);
		int nActeursDuFilme = obtenirNelementsActeurDeFilm(ptrFilm);
		cout << ptrFilm->titre << endl << endl;
		for (int i : range(nActeursDuFilme)) {
			Acteur* ptrActeur = tableauActeursDuFilme[i];
			afficherActeur(*ptrActeur);
		}
	}
	cout << endl;
}

void afficherListeFilms(const ListeFilms& listeFilms)
{
	//TODO: Utiliser des caractères Unicode pour définir la ligne de séparation (différente des autres lignes de séparations dans ce progamme).
	static const string ligneDeSeparation = {"\n//////////////////////////////////////////////////\n\n"};
	cout << ligneDeSeparation;
	//TODO: Changer le for pour utiliser un span.
	span <Film*> tableauDeFilmes = { listeFilms.elements , size_t(listeFilms.capacite) };
	for (Film* ptrFilm : tableauDeFilmes) {
		//TODO: Afficher le film.
		afficherFilm(ptrFilm);
		cout << ligneDeSeparation;
	}
}

void afficherFilmographieActeur(const ListeFilms& listeFilms, const string& nomActeur)
{
	//TODO: Utiliser votre fonction pour trouver l'acteur (au lieu de le mettre à nullptr).
	const Acteur* acteur = ObtenirPtrActeurDansListeFilme(listeFilms , nomActeur);
	if (acteur == nullptr)
		cout << "Aucun acteur de ce nom" << endl;
	else
		afficherListeFilms(acteur->joueDans);
}

int main()
{
	bibliotheque_cours::activerCouleursAnsi();  // Permet sous Windows les "ANSI escape code" pour changer de couleurs https://en.wikipedia.org/wiki/ANSI_escape_code ; les consoles Linux/Mac les supportent normalement par défaut.


	static const string ligneDeSeparation = "\n\033[35m════════════════════════════════════════\033[0m\n";

	//TODO: Chaque TODO dans cette fonction devrait se faire en 1 ou 2 lignes, en appelant les fonctions écrites.

	//TODO: La ligne suivante devrait lire le fichier binaire en allouant la mémoire nécessaire et devrait afficher les noms de 20 acteurs sans doublons (par l'affichage pour fins de débogage dans votre fonction lireActeur). 
	ListeFilms listeFilms = creerListe("films.bin");

	cout << ligneDeSeparation << "Le premier film de la liste est:" << endl;
	//TODO: Afficher le premier film de la liste.  Devrait être Alien.
	Film* ptrFilmAlien = obtenirPtrFilme(listeFilms, 0);
	afficherFilm(ptrFilmAlien);

	cout << ligneDeSeparation << "Les films sont:" << endl;
	//TODO: Afficher la liste des films.  Il devrait y en avoir 7.
	afficherListeFilms(listeFilms);

	//TODO: Modifier l'année de naissance de Benedict Cumberbatch pour être 1976 (elle était 0 dans les données lues du fichier).  Vous ne pouvez pas supposer l'ordre des films et des acteurs dans les listes, il faut y aller par son nom.
	Acteur* ptrActriceBenedict = ObtenirPtrActeurDansListeFilme(listeFilms, "Benedict Cumberbatch");
	ptrActriceBenedict->anneeNaissance = 1976; 

	cout << ligneDeSeparation << "Liste des films où Benedict Cumberbatch joue sont:" << endl;
	//TODO: Afficher la liste des films où Benedict Cumberbatch joue.  Il devrait y avoir Le Hobbit et Le jeu de l'imitation.
	int nFilmesDeActrice = obtenirNelementsFilmDeActeur(ptrActriceBenedict); 
	for (int i : range(nFilmesDeActrice)) {
		afficherFilm(obtenirListeSpecifiquePtrFilmes(ptrActriceBenedict)[i]);
	}

	//TODO: Détruire et enlever le premier film de la liste (Alien).  Ceci devrait "automatiquement" (par ce que font vos fonctions) détruire les acteurs Tom Skerritt et John Hurt, mais pas Sigourney Weaver puisqu'elle joue aussi dans Avatar.
	detruireFilm(listeFilms,ptrFilmAlien);

	cout << ligneDeSeparation << "Les films sont maintenant:" << endl;
	//TODO: Afficher la liste des films.
	afficherListeFilms(listeFilms);

	//TODO: Faire les appels qui manquent pour avoir 0% de lignes non exécutées dans le programme (aucune ligne rouge dans la couverture de code; c'est normal que les lignes de "new" et "delete" soient jaunes).  Vous avez aussi le droit d'effacer les lignes du programmes qui ne sont pas exécutée, si finalement vous pensez qu'elle ne sont pas utiles.

	//TODO: Détruire tout avant de terminer le programme.  La bibliothèque de verification_allocation devrait afficher "Aucune fuite detectee." a la sortie du programme; il affichera "Fuite detectee:" avec la liste des blocs, s'il manque des delete.
	detruireCollectionPrincipale(listeFilms);
}
