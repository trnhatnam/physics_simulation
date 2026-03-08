#include <SFML/Graphics.hpp>
#include "pendule.h"
#include "namedPipeGuard.h"
#include <cmath>
#include <iostream>
#include <windows.h>


float f(float u, float v, float gamma, float g, float L)
{
    return -(gamma*u + g/L*sin(v));
}

void update(float &u, float &v, float &h, float &gamma, float &g, float &L)
{
    // RK4

    // variables intermédiaires
    float k1 = f(u,v, gamma, g, L);
    float l1 = u;
    
    float k2 = f(u+h*k1/2, v+h*l1/2, gamma, g, L);
    float l2 = u + h*k1/2;

    float k3 = f(u+h*k2/2, v+h*l2/2, gamma, g, L);
    float l3 = u + h*k2/2;

    float k4 = f(u+h*k3, v+h*l3, gamma, g, L);
    float l4 = u + h*k3;

    // update
    u = u + (h/6)*(k1 + 2*k2 + 2*k3 + k4);
    v = v + (h/6)*(l1 + 2*l2 + 2*l3 + l4);

}

int main()
{
    float fps = 30.0;

    sf::RenderWindow window(sf::VideoMode({600, 600}), "My simulation");
    window.setFramerateLimit(fps);

	// création de la pendule
    Pendule p1({300,100}, 
                45*M_PI/180, // angle en radians
                300); // longueur du fil en pixel

    // variables initiales
    float u = 0; // vitesse angulaire
    float v = p1.getAngle();
    
    // paramètres
    float gamma = 0.1;
    float h = 1.0/fps;
    float g = 9.81 * 500; // mètres -> pixel ?
    float l = p1.getLongueur() ; // dépend de l'échelle cm -> pixel


    // création du pipe
    HANDLE pipe = CreateNamedPipe(
        TEXT("\\\\.\\pipe\\pipePython"),
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, 1024, 1024, 0, NULL);
    if (pipe == INVALID_HANDLE_VALUE) 
        return 1;

    // envoi de données vers python
    NamedPipeGuard pipeGuard(pipe); // RAII = Resource Acquisition Is Initialization
    std::cout << "En attente de connexion..." << std::endl;
    ConnectNamedPipe(pipeGuard.get(), NULL);
    DWORD bytesWritten;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            // "close requested" event: we close the window
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(p1);
        window.display();

        update(u,v, h, gamma, g, l);
        p1.setAngle(v);
        
        // Écriture du message
        WriteFile(pipeGuard.get(), &v, sizeof(v), &bytesWritten, NULL);
        FlushFileBuffers(pipeGuard.get());
    }

    return 0;
}