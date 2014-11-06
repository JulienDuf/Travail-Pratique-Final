//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant un �v�nement.
// Cr�� le 28 octobre 2014 par Julien Dufresne (dufresne_julien@hotmai.ca)
//

class CEvenement {

private:

	CArbreAVL<CControl*>* m_pArbreControlPresent; // L'arbre contenant les contr�les pr�sents dans l'�v�nement.

public:

	// Constructeurs de CEvenement...
	// En entr�e:
	// Param1: Le nombre de contr�les � mettre dans l'abre.
	// Param2: Les contr�les � mettre dans l'arbre.
	CEvenement(int argc, ...) {

		// Cr�ation de l'arbre...
		m_pArbreControlPresent = new CArbreAVL<CControl*>();

		// S'il y a des cont�les � mettre dans l'arbre, on les met...
		if (argc > 0) {
			va_list parametres;

			va_start(parametres, argc);
			for (int i = 0; i < argc; i++) {
				m_pArbreControlPresent->Ajouter(va_arg(parametres, CControl*));
			}
			va_end(parametres);
		}
	}

	// Porc�dure ajoutant des cont�les dans l'�v�nement.
	// En entr�e:
	// Param1: Le nombre de contr�les � mettre dans l'abre.
	// Param2: Les contr�les � mettre dans l'arbre.
	void Inscrire(int argc, ...) {

		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreControlPresent->Ajouter(va_arg(parametres, CControl*));
		}
		va_end(parametres);

	}

	// Porc�dure retirant des cont�les dans l'�v�nement.
	// En entr�e:
	// Param1: Le nombre de contr�les � enlever de l'abre.
	// Param2: Les contr�les � enlever de l'arbre.
	void Desinscrire(int argc, ...) {

		va_list parametres;

		va_start(parametres, argc);
		for (int i = 0; i < argc; i++) {
			m_pArbreControlPresent->Retirer(va_arg(parametres, CControl*));
		}
		va_end(parametres);

	}

	// Porc�dure r�agissant si l'�v�nement est d�clanch�.
	// En entr�e:
	// Param1: L'event du programme.
	void Reaction(SDL_Event* _pSDLEvent) {

		// Parcours l'arbre au complet.
		m_pArbreControlPresent->ParcoursReaction(_pSDLEvent);

	}
};