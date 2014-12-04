

class CPack {

public:

	virtual void ShowPack(SDL_Renderer* _pSDLRenderer) = 0;

	virtual bool Use(CPlayer* _pPlayer) = 0;

	virtual SDL_Surface* GetSurface(void) = 0;

	virtual SDL_Rect GetRectDestination(void) = 0;

};