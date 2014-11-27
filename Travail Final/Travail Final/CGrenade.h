/*
420-202-RE : Travail final
Classe qui gere les grenades et leur rebondissement
Cr�e par Samuel Rambaud le 25 novembre 2014
*/

class CGrenade: public CTools{
private:
	int m_iAngle; //Angle du lancement de la grenade
	int m_iForce; //Force du lancement de la grenade
	CBarrePuissance* m_pBarrePuissance; //Barre de puissance de la grenade

	CLabel* m_plblDescription; //Description de la grenade

	SDL_Texture* m_pTexture; //Texture de la grenade
	SDL_Rect m_pRectDestination; //Position de la grenade


public:
	//constructeur
	CGrenade(string _strSourceImage, SDL_Renderer* _Renderer){
		//Initialisation de la texture
		string strSourceImage = _strSourceImage;
		strSourceImage.append("\\Armes et Packs\\Grenade.png");
		m_pTexture = IMG_LoadTexture(_Renderer, strSourceImage.c_str());

		//initialisation des vecteurs de la grenade
		m_iAngle = 0;
		m_iForce = 0;
		m_pBarrePuissance = new CBarrePuissance(_strSourceImage, _Renderer);

		//initialisation de la position de la grenade
		m_pRectDestination.h = 24;
		m_pRectDestination.w = 33;
		m_pRectDestination.y = 0;
		m_pRectDestination.x = 0;
	}
	/*
	Utilisation de l'arme de melee
	parametre:
	_User = pointeur sur le lanceur de grenade
	*/
	void Use(CPlayer* _User){

	}
};