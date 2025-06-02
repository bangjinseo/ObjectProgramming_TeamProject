#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

int main() {
   
    sf::RenderWindow window(sf::VideoMode(405, 720), "Homeless Character Animation");

    
    sf::Font font;
    if (!font.loadFromFile("NotoSansKR-Black.ttf")) {
        std::cerr << "��Ʈ �ε� ����!" << std::endl;
        return -1;
    }

  
    int money = 0;
    sf::Text moneyText;
    moneyText.setFont(font);
    moneyText.setCharacterSize(24); 
    moneyText.setFillColor(sf::Color::Yellow);
    moneyText.setPosition(15.f, 15.f);
    moneyText.setString("��: 0��");

    
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("frame3.png")) {
        std::cerr << "frame3.png �ε� ����!" << std::endl;
        return -1;
    }
    sf::Sprite background(backgroundTexture);

    
    float scaleX = window.getSize().x / static_cast<float>(backgroundTexture.getSize().x);
    float scaleY = window.getSize().y / static_cast<float>(backgroundTexture.getSize().y);
    background.setScale(scaleX, scaleY);

    
    std::vector<sf::Texture> textures(2);
    if (!textures[0].loadFromFile("frame1.png")) {
        std::cerr << "frame1.png �ε� ����!" << std::endl;
        return -1;
    }
    if (!textures[1].loadFromFile("frame2.png")) {
        std::cerr << "frame2.png �ε� ����!" << std::endl;
        return -1;
    }

    sf::Sprite sprite;
    sprite.setTexture(textures[0]);
    sprite.setScale(0.1f, 0.1f); 
    sprite.setPosition(60.f, 300.f); 

    
    sf::Clock clock;
    float switchTime = 0.5f;
    int currentFrame = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                money++;
                moneyText.setString("��: " + std::to_string(money) + "��");

            }
        }

        
        if (clock.getElapsedTime().asSeconds() >= switchTime) {
            currentFrame = (currentFrame + 1) % 2;
            sprite.setTexture(textures[currentFrame]);
            clock.restart();
        }

        
        window.clear();
        window.draw(background);
        window.draw(sprite);
        window.draw(moneyText);
        window.display();
    }

    return 0;
}