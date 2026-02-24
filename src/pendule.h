#ifndef PENDULE
#define PENDULE

#include <SFML/Graphics.hpp>

class Pendule : public sf::Drawable, public sf::Transformable
{
public:
    Pendule(sf::Vector2f positionClouInitiale);
    sf::Vector2f getPositionBoule() const;
    sf::Vector2f getPositionClou() const;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    float rayonBoule;
    float epaisseurFil;
    float angleFil; // en rad
    float longueurFil;
    sf::Vector2f positionClou;

    sf::RectangleShape fil;
    sf::CircleShape boule;
};
#endif