#include <iostream>
#include <vector>
#include <cmath>

class KnuPower {
private:
    int level;
    int baseClickGold;

public:
    KnuPower() : level(1), baseClickGold(10) {}

    int getLevel() const { return level; }

    int getClickGold() const {
        return baseClickGold * level;
    }

    int getUpgradeCost() const {
        return 100 * level;
    }

    bool levelUp(int& gold) {
        int cost = getUpgradeCost();
        if (gold >= cost) {
            gold -= cost;
            level++;
            std::cout << "크눙이 레벨이 " << level << "이 되었습니다!\n";
            return true;
        } else {
            std::cout << "골드가 부족합니다. (필요: " << cost << ")\n";
            return false;
        }
    }
};

class Friend {
private:
    int level;
    int numProperties;

public:
    Friend(int level, int numProperties)
        : level(level), numProperties(numProperties) {}

    int getLevel() const { return level; }
    int getPropertyCount() const { return numProperties; }

};

class KnuPowerSystem {
private:
    std::vector<Friend> friends;

public:
    void addFriend(const Friend& f) {
        friends.push_back(f);
    }

    float getBonusMultiplier() const {
        int totalFriendLevel = 0;
        int totalPropertyCount = 0;

        for (const auto& f : friends) {
            totalFriendLevel += f.getLevel();
            totalPropertyCount += f.getPropertyCount();
        }

        float bonus = 1.0f + (totalFriendLevel * 0.01f) + (totalPropertyCount * 0.02f);
        return bonus;
    }

    void showFriendStats() const {
        std::cout << "\n[친구 현황]\n";
        for (size_t i = 0; i < friends.size(); ++i) {
            std::cout << i << ". 레벨 " << friends[i].getLevel()
                      << ", 부동산 " << friends[i].getPropertyCount() << "개\n";
        }
        std::cout << "KNU 파워 배율: x" << getBonusMultiplier() << "\n";
    }
};

class Player {
public:
    int gold;
    KnuPower knung;
    KnuPowerSystem power;

    Player(int initialGold) : gold(initialGold) {}

    void clickSpacebar() {
        int base = knung.getClickGold();
        float bonus = power.getBonusMultiplier();
        int total = static_cast<int>(base * bonus);
        gold += total;
        std::cout << "[클릭] +" << total << " 골드 획득! (현재: " << gold << ")\n";
    }

    void levelUpKnung() {
        knung.levelUp(gold);
    }

    void showStatus() {
        std::cout << "\n[현재 골드] " << gold
                  << "\n[크눙이 레벨] " << knung.getLevel()
                  << "\n[클릭당 기본 골드] " << knung.getClickGold()
                  << "\n[레벨업 비용] " << knung.getUpgradeCost()
                  << "\n";
        power.showFriendStats();
    }
};

int main() {
    Player player(500);

    player.power.addFriend(Friend(5, 2));
    player.power.addFriend(Friend(3, 1));

    int choice;
    while (true) {
        std::cout << "\n1. 크눙이 클릭 (스페이스바)\n2. 크눙이 레벨업\n3. 상태 확인\n0. 종료\n>> ";
        std::cin >> choice;

        if (choice == 0) break;

        switch (choice) {
            case 1:
                player.clickSpacebar();
                break;
            case 2:
                player.levelUpKnung();
                break;
            case 3:
                player.showStatus();
                break;
            default:
                std::cout << "잘못된 입력입니다.\n";
        }
    }

    return 0;
}