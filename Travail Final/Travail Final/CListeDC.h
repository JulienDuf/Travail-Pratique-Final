/*
Liste doublement cha�n�e poss�dant un pointeur sur la derni�re cellule de la liste et un autre sur une cellule pouvant �tre manipul� par l'utilisateur.
Cr��e par Gabriel Beaudry (gabriel.bdry@gmail.com) le 15 avril 2014.
*/
#include "CCelluleListeDC.h"
template <class T>
class CListeDC {
private:

	// Donn�es membres...

	unsigned int m_uiCompte; // Entier non sign� repr�sentant le compte du nombre de cellule.
	CCelluleListeDC<T>* m_pDerniere; // Pointeur de type CCelluleListeDC sur la derni�re cellule de la liste.
	CCelluleListeDC<T>* m_pTrieur; // Pointeur de type CCelluleListeDC pour effectuer le trie.
	CCelluleListeDC<T>* m_pCurseur; // Pointeur de type CCelluleListeDC sur une cellule pouvant �tre manipul� par l'utilisateur.

public:

	// Constructeur...

	CListeDC() {
		m_uiCompte = 0;
		m_pDerniere = nullptr;
		m_pCurseur = nullptr;
		m_pTrieur = nullptr;
	}

	// Destructeur...

	~CListeDC() {
		while (m_uiCompte != 0) {
			Retirer(true);
		}
	}
	
	// M�thodes...
	
	/*
	Fonction permettant de modifier le m_pTrieur sur la cellule suivante de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerSuivantTrieur() {
		// Si m_pDerni�re et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pTrieur != nullptr) {
			m_pTrieur = m_pTrieur->ObtenirSuivante();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pTrieur sur la cellule pr�c�dente de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerPrecedentTrieur() {
		// Si m_pDerni�re et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pTrieur != nullptr) {
			m_pTrieur = m_pTrieur->ObtenirPrecedente();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur le d�but de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerDebut() {
		// Si m_pDerni�re n'est pas nulle...
		if (m_pDerniere != nullptr) {
			m_pCurseur = m_pDerniere->ObtenirSuivante();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur la fin de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerFin() {
		// Si m_pDerni�re n'est pas nulle...
		if (m_pDerniere != nullptr) {
			m_pCurseur = m_pDerniere;
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur la cellule suivante de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerSuivantCurseur() {
		// Si m_pDerni�re et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pCurseur != nullptr) {
			m_pCurseur = m_pCurseur->ObtenirSuivante();
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur la cellule pr�c�dente de la liste.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	*/
	bool AllerPrecedentCurseur() {
		// Si m_pDerni�re et m_pCurseur ne sont pas nulls...
		if (m_pDerniere != nullptr && m_pCurseur != nullptr) {
			m_pCurseur = m_pCurseur->ObtenirPrecedente();
			return true;
		}
		else
			return false;
	}

	// Proc�dure permettant de positionner le curseur sur un objet voulu et le retirer si voulu...
	// En entr�e:
	// Param1: Objet voulu.
	// Param2; Bool�en qui dicte si l'objet doit �tre retirer.
	void AllerAObjetCurseur(T _Element, bool _boRetirer) {
		if (m_pDerniere != nullptr) {
			int i = 0;
			while (_Element != m_pCurseur->ObtenirElement() && i < m_uiCompte) {
				if (m_pCurseur->ObtenirElement() = _Element) {
					i = m_uiCompte;
					if (_boRetirer) {
						Retirer(true);
					}
				}
				else
					i++;
			}
		}
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur une position de cellule de la liste, en partant de 0.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un entier non sign� de la position d'insertion.
	*/
	bool AllerATrieur(unsigned int _uiPosition) {
		// Si m_pDerni�re n'est pas nulle et que la position voulue est plus petite que le compte.
		if (m_pDerniere != nullptr && _uiPosition < m_uiCompte) {
			// Si le compte est pair, on incr�mente la position momentann�ment pour faire le moins de d�placements possibles.
			if (m_uiCompte % 2 != 0) {
				_uiPosition + 1;
				// Si la position voulue est moins grande que le compte divis� par deux pour partir le plus pr�s de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					_uiPosition - 1;
					m_pTrieur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pTrieur = m_pTrieur->ObtenirSuivante();
					}
				}
				else
				{
					_uiPosition - 1;
					m_pTrieur = m_pDerniere;
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pTrieur = m_pTrieur->ObtenirPrecedente();
					}
				}
			}
			else
			{
				// Si la position voulue est moins grande que le compte divis� par deux pour partir le plus pr�s de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					m_pTrieur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pTrieur = m_pTrieur->ObtenirSuivante();
					}
				}
				else
				{
					m_pTrieur = m_pDerniere;
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pTrieur = m_pTrieur->ObtenirPrecedente();
					}
				}
			}
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur une position de cellule de la liste, en partant de 0.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un entier non sign� de la position d'insertion.
	*/
	bool AllerACurseur(unsigned int _uiPosition) {
		// Si m_pDerni�re n'est pas nulle et que la position voulue est plus petite que le compte.
		if (m_pDerniere != nullptr && _uiPosition < m_uiCompte) {
			// Si le compte est pair, on incr�mente la position momentann�ment pour faire le moins de d�placements possibles.
			if (m_uiCompte % 2 != 0) {
				_uiPosition + 1;
				// Si la position voulue est moins grande que le compte divis� par deux pour partir le plus pr�s de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					_uiPosition - 1;
					m_pCurseur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pCurseur = m_pCurseur->ObtenirSuivante();
					}
				}
				else
				{
					_uiPosition - 1;
					m_pCurseur = m_pDerniere;
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pCurseur = m_pCurseur->ObtenirPrecedente();
					}
				}
			}
			else
			{
				// Si la position voulue est moins grande que le compte divis� par deux pour partir le plus pr�s de la position.
				if (_uiPosition <= m_uiCompte / 2) {
					m_pCurseur = m_pDerniere->ObtenirSuivante();
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = 0; ui < _uiPosition; ui++) {
						m_pCurseur = m_pCurseur->ObtenirSuivante();
					}
				}
				else
				{
					m_pCurseur = m_pDerniere;
					// Boucle qui s'arr�te quand le pointeur m_pCurseur est � la position voulue.
					for (unsigned int ui = m_uiCompte - 1; ui > _uiPosition; ui--) {
						m_pCurseur = m_pCurseur->ObtenirPrecedente();
					}
				}
			}
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de modifier le m_pCurseur sur une nouvelle cellule cr��e en m�moire.
	Si la modification a peut avoir lieu, retourne vrai, sinon, retourne faux.
	Prend un �l�ment � ajouter.
	*/
	bool Inserer(T _Element) {
		// Si le curseur n'est pas null...
		if (m_pCurseur != nullptr) {
			m_pCurseur->ObtenirPrecedente()->DefinirSuivante(new CCelluleListeDC<T>(_Element, m_pCurseur, m_pCurseur->ObtenirPrecedente())); // Cr�ation d'un obbjet en m�moire et retour de son adresse dans m_pSuivante.
			m_pCurseur->DefinirPrecedente(m_pCurseur->ObtenirPrecedente()->ObtenirSuivante());
			m_pCurseur = m_pCurseur->ObtenirPrecedente();
			m_uiCompte++; // Incr�mentation du compte.
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de retirer le cellule � la position du curseur et de faire pointer celui-ci sur la cellule suivante, sauf s'il n'y en a pas.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un bool qui dit s'il faut supprimer l'�l�ment dans la cellule.
	*/
	bool Retirer(bool _boSupprimer) {
		// Si le curseur n'est pas null...
		if (m_pCurseur != nullptr) {
			m_pCurseur->ObtenirPrecedente()->DefinirSuivante(m_pCurseur->ObtenirSuivante());
			m_pCurseur->ObtenirSuivante()->DefinirPrecedente(m_pCurseur->ObtenirPrecedente());
			CCelluleListeDC<T>* pCelltmpListeDC = m_pCurseur; // Pointeur de type CCelluleListeDC sur m_pCurseur.
			// Si le curseur n'est pas sur m_pDerni�re... il sera �gal � sa suivante
			if (m_pCurseur != m_pDerniere)
				m_pCurseur = m_pCurseur->ObtenirSuivante();
			// Sinon il est �gal � sa pr�c�dente pour ne pas revenir au d�but.
			else
			{
				m_pCurseur = m_pCurseur->ObtenirPrecedente();
				m_pDerniere = m_pCurseur;
			}
			// Si l'utilisateur veut effacer l'�l�ment dans la cellule.
			if (_boSupprimer)
				delete pCelltmpListeDC->ObtenirElement();
			delete pCelltmpListeDC;
			m_uiCompte--; // D�cr�mentation du compte.
			// Si le compte est � z�ro, tout mettre � nullptr.
			if (m_uiCompte == 0) {
				m_pCurseur = nullptr;
				m_pDerniere = nullptr;
			}
			return true;
		}
		else
			return false;
	}

	/*
	Fonction permettant de retirer le cellule � la position du curseur et de faire pointer celui-ci sur la cellule suivante, sauf s'il n'y en a pas.
	Si la modification a peut avoir lieu, retourn vrai, sinon, retourne faux.
	Prend un bool qui dit s'il faut supprimer l'�l�ment dans la cellule.
	*/
	bool RetirerTrieur(bool _boSupprimer) {
		// Si le curseur n'est pas null...
		if (m_pTrieur != nullptr) {
			m_pTrieur->ObtenirPrecedente()->DefinirSuivante(m_pTrieur->ObtenirSuivante());
			m_pTrieur->ObtenirSuivante()->DefinirPrecedente(m_pTrieur->ObtenirPrecedente());
			CCelluleListeDC<T>* pCelltmpListeDC = m_pTrieur; // Pointeur de type CCelluleListeDC sur m_pCurseur.
			
			if (m_pTrieur == m_pCurseur)
				m_pCurseur = m_pCurseur->ObtenirPrecedente();
			
			// Si le curseur n'est pas sur m_pDerni�re... il sera �gal � sa suivante
			if (m_pTrieur != m_pDerniere)
				m_pTrieur = m_pTrieur->ObtenirSuivante();
			// Sinon il est �gal � sa pr�c�dente pour ne pas revenir au d�but.
			else
			{
				m_pTrieur = m_pTrieur->ObtenirPrecedente();
				m_pDerniere = m_pTrieur;
			}
			// Si l'utilisateur veut effacer l'�l�ment dans la cellule.
			if (_boSupprimer)
				delete pCelltmpListeDC->ObtenirElement();
			delete pCelltmpListeDC;
			m_uiCompte--; // D�cr�mentation du compte.
			// Si le compte est � z�ro, tout mettre � nullptr.
			if (m_uiCompte == 0) {
				m_pTrieur = nullptr;
				m_pCurseur = nullptr;
				m_pDerniere = nullptr;
			}
			return true;
		}
		else
			return false;
	}

	/*
	Proc�dure permettant d'ajouter une cellule au d�but de la liste
	Prend un �l�ment � ajouter.
	*/
	void AjouterDebut(T _Element) {
		// Si m_pDerni�re est null, on ajoute une premi�re cellule.
		if (m_pDerniere == nullptr) {
			m_pDerniere = new CCelluleListeDC<T>(_Element, nullptr, nullptr); // Cr�ation d'un objet en m�moire et retour de l'adresse m�moire dans m_pDerni�re.
			m_pDerniere->DefinirSuivante(m_pDerniere);
			m_pDerniere->DefinirPrecedente(m_pDerniere);
		}
		else
		{
			m_pDerniere->DefinirSuivante(new CCelluleListeDC<T>(_Element, m_pDerniere->ObtenirSuivante(), m_pDerniere)); // Cr�ation d'un objet en m�moire et retour de l'addresse m�moire dans la suivante de la derni�re.
			m_pDerniere->ObtenirSuivante()->ObtenirSuivante()->DefinirPrecedente(m_pDerniere->ObtenirSuivante());
		}
		m_uiCompte++; // Incr�mentation du compte.
	}

	/*
	Proc�dure permettant d'ajouter une cellule � la fin de la liste
	Prend un �l�ment � ajouter.
	*/
	void AjouterFin(T _Element) {
		// Si m_pDerni�re est null , on ajoute comme ajouter d�but.
		if (m_pDerniere == nullptr) {
			AjouterDebut(_Element);
		}
		else
		{
			m_pDerniere->DefinirSuivante(new CCelluleListeDC<T>(_Element, m_pDerniere->ObtenirSuivante(), m_pDerniere));
			m_pDerniere = m_pDerniere->ObtenirSuivante();
			m_pDerniere->ObtenirSuivante()->DefinirPrecedente(m_pDerniere);
			m_uiCompte++; // Incr�mentation du compte.
		}
	}

	// Accesseurs...

	// Fonction qui retourne l'�l�ment dans la cellule o� pointe le curseur.

	T ObtenirElementTrieur() {
		return m_pTrieur->ObtenirElement();
	}

	T ObtenirElementCurseur() {
		return m_pCurseur->ObtenirElement();
	}

	void DefinirElementTrieur(T _Element) {
		m_pTrieur->DefinirElement(_Element);
	}

	void DefinirElementCurseur(T _Element) {
		m_pCurseur->DefinirElement(_Element);
	}

	// Fonction qui retourne le compte.
	unsigned int ObtenirCompte() {
		return m_uiCompte;
	}
};