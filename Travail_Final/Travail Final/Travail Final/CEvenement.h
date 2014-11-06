//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant un événement.
// Créé le 28 octobre 2014 par Julien Dufresne (dufresne_julien@hotmai.ca)
//

class CEvenement {

private:

	CArbreAVL<CControl*>* m_pArbreControlPresent; // L'arbre contenant les contrôles présents dans l'événement.

public:

	// Constructeurs de CEvenement...
	// En entrée:
	// Param1: Le nombre de contrôles à mettre dans l'abre.
	// Param2: Les contrôles à mettre dans l'arbre.
	CEvenement(int argc, ...) {

		// Création de l'arbre...
		m_pArbreControlPresent = new CArbreAVL<CControl*>();

		// S'il y a des contôles à mettre dans l'arbre, on les met...
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pArbreControlPresent->Ajouter(va_arg(parametres, CControl*));
			}
			va_end(parametres);
		}
	}

	// Porcédure ajoutant des contôles dans l'événement.
	// En entrée:
	// Param1: Le nombre de contrôles à mettre dans l'abre.
	// Param2: Les contrôles à mettre dans l'arbre.
	void Inscrire(int argc, ...) {

		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreControlPresent->Ajouter(va_arg(parametres, CControl*));
		}
		va_end(parametres);

	}

	// Porcédure retirant des contôles dans l'événement.
	// En entrée:
	// Param1: Le nombre de contrôles à enlever de l'abre.
	// Param2: Les contrôles à enlever de l'arbre.
	void Desinscrire(int argc, ...) {

		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreControlPresent->Retirer(va_arg(parametres, CControl*));
		}
		va_end(parametres);

	}

	// Porcédure réagissant si l'événement est déclanché.
	// En entrée:
	// Param1: L'event du programme.
	void Reaction(SDL_Event* _pSDLEvent) {

		// Parcours l'arbre au complet.
		m_pArbreControlPresent->ParcoursReaction(_pSDLEvent);

	}
};