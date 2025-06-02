#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

using namespace std;

atomic<bool> running(true);  // 자동 수익 종료 조건

class Friend {
public:
    int level;

    Friend(int lvl = 1) : level(lvl) {}

    int getGoldPerSecond() const {
        return level * 10; // 레벨 1당 초당 10골드
    }

    void levelUp() {
        level++;
        cout << "[레벨업!] 친구가 레벨 " << level << "이 되었어요!" << endl;
    }
};

class Player {
public:
    int gold = 0;

    void earnGold(int amount) {
        gold += amount;
        cout << "[수익] 현재 골드: " << gold << " G" << endl;
    }
};

// 초당 자동 수익 계산
void autoIncome(Player& player, vector<Friend>& friends) {
    while (running) {
        int totalIncome = 0;
        for (auto& f : friends) {
            totalIncome += f.getGoldPerSecond();
        }
        player.earnGold(totalIncome);

        this_thread::sleep_for(chrono::seconds(1)); // 1초 대기
    }
}

int main() {
    Player player;
    vector<Friend> friends = { Friend(1), Friend(2), Friend(1) }; // 친구 3명: Lv1, Lv2, Lv1

    // 자동 수익 스레드 시작
    thread incomeThread(autoIncome, ref(player), ref(friends));

    // 테스트: 친구 레벨업 순차적으로 시도
    for (int i = 0; i < 3; ++i) {
        this_thread::sleep_for(chrono::seconds(4));
        friends[i % 3].levelUp(); // 친구들 순서대로 레벨업
    }

    // 15초 후 자동 수익 종료
    this_thread::sleep_for(chrono::seconds(15));
    running = false;
    incomeThread.join();

    cout << "\n[게임 종료] 최종 골드: " << player.gold << " G" << endl;
    return 0;
}