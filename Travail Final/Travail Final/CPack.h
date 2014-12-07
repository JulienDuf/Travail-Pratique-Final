

class CPack {

public:

	virtual void ShowPack(SDL_Renderer* _pSDLRenderer) = 0;

	virtual bool Use(CPlayer* _pPlayer) = 0;

	virtual void ModifierPosition(SDL_Rect _RectDestination) = 0;

	virtual void ModifierStabilePack(bool _boStable) = 0;

	virtual void ModifierAnlge(double _dAngle) = 0;

	virtual SDL_Surface* GetSurface(void) = 0;

	virtual SDL_Rect GetRectDestination(void) = 0;

	virtual bool IsStable(void) = 0;

};