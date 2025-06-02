#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

class Property {
private:
    std::string name;
    int price;
    float interestRate;

public:
    Property(std::string name, int price, float interestRate)
        : name(name), price(price), interestRate(interestRate) {}

    int getPrice() const { return price; }
    float getInterestRate() const { return interestRate; }
    std::string getName() const { return name; }

    int calculateInterest() const {
        return static_cast<int>(price * interestRate);
    }
};

class RealEstateSystem {
private:
    std::vector<Property> ownedProperties;
    std::mutex mtx;

public:
    void buyProperty(const Property& prop, int& gold) {
        std::lock_guard<std::mutex> lock(mtx);
        if (gold >= prop.getPrice()) {
            ownedProperties.push_back(prop);
            gold -= prop.getPrice();
            std::cout << "[구매 완료] " << prop.getName() << "를 구입했습니다.\n";
        } else {
            std::cout << "[구매 실패] 골드가 부족합니다.\n";
        }
    }

    void sellProperty(int index, int& gold) {
        std::lock_guard<std::mutex> lock(mtx);
        if (index >= 0 && index < ownedProperties.size()) {
            Property prop = ownedProperties[index];
            gold += prop.getPrice();
            ownedProperties.erase(ownedProperties.begin() + index);
            std::cout << "[판매 완료] " << prop.getName() << "를 판매했습니다.\n";
        } else {
            std::cout << "[판매 실패] 잘못된 인덱스입니다.\n";
        }
    }

    int collectInterest() {
        std::lock_guard<std::mutex> lock(mtx);
        int total = 0;
        for (const auto& prop : ownedProperties) {
            total += prop.calculateInterest();
        }
        return total;
    }

    void showOwned() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "\n[보유 부동산 목록]\n";
        if (ownedProperties.empty()) {
            std::cout << "없음\n";
        } else {
            for (size_t i = 0; i < ownedProperties.size(); ++i) {
                std::cout << i << ". " << ownedProperties[i].getName()
                          << " (가격: " << ownedProperties[i].getPrice()
                          << ", 이자율: " << ownedProperties[i].getInterestRate() * 100 << "%)\n";
            }
        }
    }
};

class Player {
public:
    int gold;
    std::atomic<bool> running;
    RealEstateSystem realEstate;

    Player(int initialGold) : gold(initialGold), running(true) {}

    void startInterestLoop() {
        std::thread([this]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                int interest = realEstate.collectInterest();
                gold += interest;
            }
        }).detach();
    }
};

void showMenu() {
    std::cout << "\n===== 메뉴 =====\n";
    std::cout << "1. 부동산 구매\n";
    std::cout << "2. 부동산 판매\n";
    std::cout << "3. 보유 부동산 확인\n";
    std::cout << "4. 현재 골드 확인\n";
    std::cout << "0. 게임 종료\n";
    std::cout << "==============\n";
    std::cout << "선택 >> ";
}

int main() {
    Player player(1000);

    std::vector<Property> market = {
        Property("상가", 500, 0.05f),
        Property("주택", 300, 0.03f),
        Property("건물", 1000, 0.07f)
    };

    player.startInterestLoop();

    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            player.running = false;
            std::cout << "게임을 종료합니다...\n";
            break;
        }

        switch (choice) {
        case 1: {
            std::cout << "\n[구매 가능한 부동산 목록]\n";
            for (size_t i = 0; i < market.size(); ++i) {
                std::cout << i << ". " << market[i].getName()
                          << " (가격: " << market[i].getPrice()
                          << ", 이자율: " << market[i].getInterestRate() * 100 << "%)\n";
            }
            std::cout << "구매할 번호 입력 >> ";
            int idx;
            std::cin >> idx;
            if (idx >= 0 && idx < market.size()) {
                player.realEstate.buyProperty(market[idx], player.gold);
            } else {
                std::cout << "[오류] 잘못된 번호입니다.\n";
            }
            break;
        }
        case 2:
            player.realEstate.showOwned();
            std::cout << "판매할 인덱스 입력 >> ";
            int idx;
            std::cin >> idx;
            player.realEstate.sellProperty(idx, player.gold);
            break;

        case 3:
            player.realEstate.showOwned();
            break;

        case 4:
            std::cout << "[현재 골드] " << player.gold << " G\n";
            break;

        default:
            std::cout << "[오류] 잘못된 메뉴 선택입니다.\n";
            break;
        }
    }

    return 0;
}