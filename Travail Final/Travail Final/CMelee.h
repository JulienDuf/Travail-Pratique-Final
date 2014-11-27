/*
Classe qui gere les armes de melee
Crée par Samuel Rambaud le 25 novembre 2014
*/

class CMelee: public CTools{
private:
	int iDamage; //Degats de l'arme de melee
	CLabel* m_plblDescription; //Description de l'arme de melee
	SDL_Surface* m_pSurfaceSprite; //Surface SDL qui deviendra le sprite
	CSprite* m_pSprite; //sprite du personnage qui utilise l'arme de melee
public:

	//constructeur
	CMelee(){
		iDamage = 20;
		m_pSprite = new CSprite(, , 300);
	}

	/*
	Utilisation de l'arme de melee
		parametre:
		_Cible = pointeur sur la victime de l'attaque de melee
	*/
	void Use(CPlayer* _Cible){
		_Cible->SetHealth((_Cible->GetHealth) - iDamage);
	}


	/*
	Affichage du sprite de l'utilisation de l'arme de melee
	parametre:
		_Renderer = Renderer SDL ou s'afffichera le sprite
	*/
	void Render(SDL_Renderer* _Renderer){
		m_pSprite->Render(_Renderer);
	}
};