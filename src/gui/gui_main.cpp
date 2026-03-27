#include <SFML/Graphics.hpp>
#include <iostream>
int main (){

    sf::RenderWindow window(sf::VideoMode({1920,1080}), "Retro Poker", sf::Style::Default, sf::State::Windowed);
    window.setFramerateLimit(60);

    sf::Font font("assets/fonts/AncientMedium.ttf");
    sf::Text title(font, "RetroPoker", 102);
    
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Regular);    

    sf::FloatRect bounds = title.getLocalBounds();
    
    title.setOrigin({bounds.position.x + bounds.size.x / 2.f,bounds.position.y + bounds.size.y / 2.f});
    title.setPosition({960.f, 540.f});


    sf::Text press_play_text(font, "Press Space to Start", 20);
    press_play_text.setFillColor(sf::Color::White);
    press_play_text.setStyle(sf::Text::Regular);
    bounds = press_play_text.getLocalBounds();
    press_play_text.setOrigin({bounds.position.x + bounds.size.x / 2.f,bounds.position.y + bounds.size.y / 2.f});
    press_play_text.setPosition({960.f, 900.f});







    while(window.isOpen()){
        while(const auto event = window.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                window.close();
            }
        }

    

        

        window.clear(sf::Color(0,0,0));
        window.draw(title);
        window.draw(press_play_text);
        window.display();
    }
    return 0;
}
