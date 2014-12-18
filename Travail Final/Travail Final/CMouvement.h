
class CMouvement {
public:

	virtual void ShowPlayer(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) = 0;

	virtual void ReactToEvent(SDL_Event* _pEvent, CVecteur2D* _pVecteurVitesse, bool* _boStable) = 0;

	virtual CSprite* ObtenirSprite(string _strNom) = 0;

	virtual void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) = 0;

	virtual void ShowDescription(SDL_Renderer* _pRenderer) = 0;

	virtual unsigned int ObtenirMunition() = 0;

	virtual bool IsActive(void) = 0;

	virtual CVecteur2D* ObtenirVecteur(void) = 0;
};