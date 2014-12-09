class CTool {

public:

	virtual void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) = 0;

	virtual void ReactToEvent(SDL_Event* _pEvent) = 0;
<<<<<<< HEAD

	virtual void DefinirActif(bool _boActif) = 0;
=======
>>>>>>> origin/Branche-jeu

	virtual CSprite* ObtenirSprite(string _strNom) = 0;

	virtual unsigned int ObtenirMunition() = 0;

	virtual void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) = 0;

	virtual void ShowDescription(SDL_Renderer* _pRenderer) = 0;

};