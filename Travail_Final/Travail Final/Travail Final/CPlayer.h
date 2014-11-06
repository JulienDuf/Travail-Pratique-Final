
// 420-202-RE : Travail final
// Classe qui repr�sente un joueur du jeu.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CPlayer {

private:

	CSprite* m_pSpriteCourse;			// Pointeur de sprite qui pointe sur le sprite qui repr�sente le joueur qui est en �tat de course.
	CSprite* m_pSpriteSaut;				// pointeur de sprite qui pointe sur le sprite qui repr�sente le joueur qui est en �tat de saut.

	string m_strName;					// Chaine de caract�res qui contient le nom du joueur.

	CListeDC<CTools*>* m_pToolList;		// pointeur de liste d'outils qui pointe sur la liste d'outils de combat que l'utilisateur peut utiliser.


public:
	
	// Constructeur...
	// Param�tre: _pSpriteCourse, pointe sur le sprite qui repr�sente le joueur qui est en �tat de course.
	// Param�tre: _pSpriteSaut, pointe sur le sprite qui repr�sente le joueur qui est en �tat de saut.
	// Param�tre: _pToolList, pointe sur la liste d'outils que l'on veut donner au joueur.
	// Param�tre: _strName, contient le nom que l'on veut donner au joueur.
	// Retour: Rien (constructeur).
	CPlayer(CSprite* _pSpriteCourse, CSprite* _pSpriteSaut, CListeDC<CTools*>* _pToolList, string _strName) {

		m_pSpriteCourse = _pSpriteCourse;
		m_pSpriteSaut = _pSpriteSaut;

		m_strName = _strName;

		m_pToolList = _pToolList;


	}

	// Proc�dure qui affiche le joueur.
	// Param�tre: _pSDLRenderer, Rendeur de la fen�tre dans laquelle on veut afficher le joueur.
	// Retour: Rien.
	void ShowPlayer(SDL_Renderer* _pSDLRenderer) {



	}


};