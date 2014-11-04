//
// 420-202-RE Gr. 01 : Travail final.
// Classe représentant une fenêtre.
// Créé le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//

#define MouseButtonDown 0
#define MouseButtonUp 1

class CControl {
public:

	virtual void ShowControl(SDL_Renderer* _pSDLRenderer) = 0;

	virtual unsigned int ReactToEvent(SDL_Event* _pSDLEvent) = 0;

};