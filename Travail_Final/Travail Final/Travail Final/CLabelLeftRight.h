/*
Classe Gérant les Labels "Left Right"
Créée le 30 octobre par Samuel Rambaud (Sam.Rambaud@outlook.com)
*/

class CLabelLeftRight{
private:

	CLabel* m_pLabel; //Label du controle
	CButton* m_pButtonLeft; //Bouton de droite du controle
	CButton* m_pButtonRight; //Bouton de gauche du controle

public:

	/*
	Contructeur du controle visuel
	parametres:
	_pLabel: Label du controle visuel
	_pButtonRight : Bouton de droite
	_pButtonLeft: Bouton de gauche
	*/
	CLabelLeftRight(CLabel* _pLabel, CButton* _pButtonLeft, CButton* _pButtonRight){
		m_pLabel = _pLabel;
		m_pButtonLeft = _pButtonLeft;
		m_pButtonRight = _pButtonRight;
	}

	/*
	Affiche les controles visuels
	parametres:
	_Renderer: Renderer ou s'affichera le controle
	*/
	void ShowControl(SDL_Renderer* _Renderer){
		m_pButtonLeft->ShowControl(_Renderer);
		m_pButtonRight->ShowControl(_Renderer);
		m_pLabel->ShowControl(_Renderer);
	}

	// Procédure réagissant à un événement...
	// Entrée:
	// Param1: Le gestionaire d'événement de SDL.
	void ReactToEvent(SDL_Event* _pSDLEvent) {

	}

	//Accesseurs

	CButton* GetButtonLeft(void){
		return m_pButtonLeft;
	}

	CButton* GetButtonRight(void){
		return m_pButtonRight;
	}
};