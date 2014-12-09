class CProjectile {

public:

	virtual void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) = 0;

	virtual void ReactToEvent(SDL_Event* _pEvent) = 0;

	virtual void DefinirActif(bool _boActif) = 0;

	virtual CSprite* ObtenirSprite(string _strNom) = 0;

	virtual unsigned int ObtenirMunition() = 0;

	virtual void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) = 0;

	virtual void ShowDescription(SDL_Renderer* _pRenderer) = 0;

	virtual CVecteur2D* ObtenirVecteurVitesse() = 0;

	virtual SDL_Rect* ObtenirRectDestination() = 0;
};