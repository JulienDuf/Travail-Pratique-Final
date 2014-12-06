/*
Classe qui gere les armes de melee
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CMelee : public CTool {
private:
	int iDamage; //Degats de l'arme de melee
	CLabel* m_plblDescription; //Description de l'arme de melee
	SDL_Surface* m_pSurfaceSprite; //Surface SDL qui deviendra le sprite
	CSprite* m_pSprite; //sprite du personnage qui utilise l'arme de melee
	SDL_Rect m_RectDestinationArme;
public:

	//constructeur
	CMelee(string _strEmplacement){

		iDamage = 0.2;
		//m_pSprite = new CSprite(0, 0,0 ,300);
	}

	void ShowTool(SDL_Renderer* _pRenderer, SDL_Rect _RectPlayerDestination) {
		m_pSprite->Render(_pRenderer, m_RectDestinationArme);
	}

	void ReactToEvent(SDL_Event* _pEvent) {}

	void DefinirActif(bool _boActif) {}

	CSprite* ObtenirSprite(string _strNom) { return nullptr; }

	unsigned int ObtenirMunition() { return 0; }

	void DefinirboShowDescription(bool _boShow) {}
};