

class CPack {

public:

	virtual void ShowPack(SDL_Renderer* _pSDLRenderer) = 0;

	virtual void Use(void) = 0;

	virtual void GetSurface(void) = 0;

	virtual void GetRectDestination(void) = 0;

};