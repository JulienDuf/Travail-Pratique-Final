
// 420-202-RE : Travail final
// Classe qui représente une minuterie.
// 4 novembre 2014 par Nicolas Dean (Gody117@hotmail.com)
//
class CTimer {

private:
	unsigned int m_uiTimer;
	unsigned int m_uiTicks;

public:

	// Constructeur...
	CTimer(unsigned int _uiTimer) {

		m_uiTimer = _uiTimer;
		m_uiTicks = 0;

	}


	// Méthodes...

	// procédure qui fait commencer la minuterie.
	void Start(void) {

		m_uiTicks = SDL_GetTicks();

	}

	// Fonction qui indique quand la minuterie est terminée.
	// retourne une booléenne qui indique true si la minuterie est terminée.
	bool IsDone(void) {
		if (m_uiTicks != 0) {
			return (SDL_GetTicks() - m_uiTicks >= m_uiTimer);
		}
		else
			return true;
	}


	// Accesseurs...

	void DefinirTimer(unsigned int _uiTimer){

		m_uiTimer = _uiTimer;

	}



};