#include <SFML/Graphics.hpp>

int main (){
    sf::RenderWindow window(sf::VideoMode({1920,1080}), "Retro Poker", sf::Style::Default, sf::State::Fullscreen);
    window.setFramerateLimit(60);

    
    while(window.isOpen()){
        while(const auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }

        window.clear(sf::Color(20,29,39));
        window.display();
    }
    return 0;
}
