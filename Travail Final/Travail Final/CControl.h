//
// 420-202-RE Gr. 01 : Travail final.
// Classe repr�sentant une fen�tre.
// Cr�� le 28 octobre 2014 par Xavier St-Georges (xavierst-georges@hotmail.com)
//

#define MouseButtonDown 0
#define MouseButtonUp 1

class CControl {
public:

	virtual void ShowControl(SDL_Renderer* _pSDLRenderer) = 0;

	virtual void ReactToEvent(SDL_Event* _pSDLEvent) = 0;

	virtual void AjouterTexture(int argc, ...) = 0;

	virtual int ObtenirElement(string _Element) = 0;

	virtual void ChangeTexture(bool _boDroite) = 0;
};