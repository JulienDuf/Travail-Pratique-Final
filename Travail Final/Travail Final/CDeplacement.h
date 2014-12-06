class CDeplacement {
public:

	virtual void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) = 0;

	virtual void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) = 0;

	virtual CSprite* ObtenirSprite(string _strNom) = 0;

	virtual void DefinirboShowDescription(bool _boShow) = 0;
};