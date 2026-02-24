#include "pendule.h"
#include <cmath>

Pendule::Pendule(sf::Vector2f positionClouInitiale):
    rayonBoule(40.0f), 
    epaisseurFil(5.0f),
    angleFil(M_PI_4),
    longueurFil(300.f),
    positionClou(positionClouInitiale),
    fil({epaisseurFil, longueurFil}),
    boule(rayonBoule)
{
    // le clou est placé au milieu du début du fil
    fil.setOrigin({epaisseurFil / 2, 0.f});
    fil.setPosition(positionClou);
    fil.setRotation(-sf::radians(angleFil));

    // la boule est placé au bout du fil
    boule.setOrigin({rayonBoule, rayonBoule});
    boule.setPosition({positionClou.x + static_cast<float>(longueurFil*sin(angleFil)), positionClou.y + static_cast<float>(longueurFil*cos(angleFil))});
};

sf::Vector2f Pendule::getPositionBoule() const{
    return {positionClou.x + static_cast<float>(longueurFil*sin(angleFil)), positionClou.y + static_cast<float>(longueurFil*cos(angleFil))};
};

sf::Vector2f Pendule::getPositionClou() const{
    return positionClou;
};


void Pendule::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(fil, states);
    target.draw(boule, states);
}







