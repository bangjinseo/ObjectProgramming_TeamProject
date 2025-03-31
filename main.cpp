#include <SFML/Graphics.hpp>
#include <optional> // SFML 3에서 pollEvent는 std::optional 반환

int main() {
    sf::RenderWindow window({800, 600}, "Hello SFML!");

    while (window.isOpen()) {
        // pollEvent는 std::optional<sf::Event> 반환!
        while (auto event = window.pollEvent()) {
            if (event->type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}
