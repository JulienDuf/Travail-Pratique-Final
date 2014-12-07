/*
Classe qui gere les grenades et leur rebondissement
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CGrenade: public CTool {
private:
	int m_iAngle; //Angle du lancement de la grenade
	int m_iForce; //Force du lancement de la grenade
	unsigned int m_uiMunition;
	CBarrePuissance* m_pBarrePuissance; //Barre de puissance de la grenade

	CLabel* m_pLblDescription; //Description de la grenade
	bool m_boShowDescription;

	SDL_Texture* m_pTexture; //Texture de la grenade
	SDL_Rect m_pRectDestination; //Position de la grenade


public:
	//constructeur
	CGrenade(string _strSourceImage, SDL_Renderer* _Renderer){
		//Initialisation de la texture
		string strSourceImage = _strSourceImage;
		strSourceImage.append("\\Armes et Packs\\Grenade.png");
		m_pTexture = IMG_LoadTexture(_Renderer, strSourceImage.c_str());

		m_pLblDescription = new CLabel(m_pTexture, { 0, 0, 100, 100 });

		//initialisation des vecteurs de la grenade
		m_iAngle = 0;
		m_iForce = 0;
		//m_pBarrePuissance = new CBarrePuissance(_strSourceImage);

		//initialisation de la position de la grenade
		m_pRectDestination.h = 24;
		m_pRectDestination.w = 33;
		m_pRectDestination.y = 0;
		m_pRectDestination.x = 0;
	}
	
	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {}

	void ShowDescription(SDL_Renderer* _pRenderer) {
		if (m_boShowDescription) {
			m_pLblDescription->ShowControl(_pRenderer);
		}
	}
	void ReactToEvent(SDL_Event* _pEvent) {}

	void DefinirActif(bool _boActif) {}

	CSprite* ObtenirSprite(string _strNom) { return nullptr; }

	unsigned int ObtenirMunition() { return 0; }

	void UpdateDescription(bool _boShow, SDL_Rect _RectPositionDescription) {
		m_boShowDescription = _boShow;
		m_pLblDescription->SetRectDestinationX(_RectPositionDescription.x);
		m_pLblDescription->SetRectDestinationY(_RectPositionDescription.y);
	}

};