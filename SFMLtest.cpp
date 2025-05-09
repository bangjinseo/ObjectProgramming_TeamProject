#include <SFML/Graphics.hpp>

int main() {
    // SFML 윈도우 객체 생성
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");

    // 프로그램이 종료될 때까지 실행
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(); // 화면을 지우고
        window.display(); // 화면에 그리기
    }

    return 0;
}