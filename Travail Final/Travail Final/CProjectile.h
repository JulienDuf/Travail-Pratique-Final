class CProjectile {

public:

	virtual bool ReactionExplosion(int iX, int iY) = 0;

	virtual void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) = 0;

	virtual void ReactToEvent(SDL_Event* _pEvent) = 0;

	virtual void DefinirRotation(int _iVitesseAngulaire) = 0;

	virtual CSprite* ObtenirSprite(string _strNom) = 0;

	virtual unsigned int ObtenirMunition() = 0;

	virtual void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) = 0;

	virtual void ShowDescription(SDL_Renderer* _pRenderer) = 0;

	virtual void ReinitialisationProjectile(void) = 0;

	virtual CVecteur2D* ObtenirVecteurVitesse() = 0;

	virtual SDL_Rect* ObtenirRectDestination() = 0;
	
	virtual SDL_Surface* ObtenirSurface() = 0;

	virtual void DefinirAngle(double _dAngle) = 0;

	virtual void DefinirPosition(SDL_Rect _Rect) = 0;

	virtual void DefinirExplosion(bool _boExplosion) = 0;

	virtual bool EstLancer() = 0;

	virtual bool ExplosionEnCours() = 0;

	virtual unsigned int ObtenirRayonDommage() = 0;
};