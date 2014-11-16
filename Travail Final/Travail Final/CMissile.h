// 420-202-RE : Travail final
// Classe qui représente un missile ayant un angle et une force.
// 16 novembre 2014 par Julien Dufresne (dufresne_julien@hotmail.ca)
//

class CMissile : public CTools {

private:

	int m_iAngle; // L'angle de départ du missile.
	int m_iForce; // Force de départ du missile.

	SDL_Texture* m_pSDLTextureMissile; // La texture du missile.
	SDL_Rect m_RectDestinationMissile; // La destination du missile dans la fenêtre.
	CBarrePuissance* m_pBarrePuissance; // La barre déterminant l'angle et la puissance du missile.

public:

	CMissile(string _strEmplacement, SDL_Renderer* _pRenderer) {

		string strEmplacement = _strEmplacement;

		m_iAngle = 0;
		m_iForce = 0;

		strEmplacement.append("Armes et Packs\\missile.png");
		m_pSDLTextureMissile = IMG_LoadTexture(_pRenderer, strEmplacement.c_str());

		m_RectDestinationMissile = { 0, 0, 0, 0 };
		SDL_QueryTexture(m_pSDLTextureMissile, NULL, NULL, &m_RectDestinationMissile.w, &m_RectDestinationMissile.h);
		m_pBarrePuissance = new CBarrePuissance(_strEmplacement, _pRenderer);

	}



};