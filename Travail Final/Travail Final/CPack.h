

class CPack {

public:

	virtual void ShowPack(SDL_Renderer* _pSDLRenderer) = 0;
	virtual SDL_Rect GetRectDestination() = 0;
	virtual SDL_Surface* GetSurface() = 0;

};