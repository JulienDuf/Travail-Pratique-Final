//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant une fenêtre.
// Créé le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//

class CControl {
public:

	virtual void ShowControl(SDL_Renderer* _pSDLRenderer) = 0;

	virtual CControl* ReactToEvent(SDL_Event* _pSDLEvent) = 0;

};