class CTools {

public:

	virtual void ShowTools(SDL_Renderer* _pRenderer) = 0;

	virtual void ReactToEvent(SDL_Event* _pEvent) = 0;
	
	virtual void ModifierPositionTool(SDL_Rect _RectDestination) = 0;
};