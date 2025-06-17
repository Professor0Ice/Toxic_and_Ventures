#include "GameLogic.h"

GameLogic::GameLogic()
{
	terminal_open();

    terminal_set("font: fonts/UbuntuMono-Regular.ttf, size=9, codepage=437");
    terminal_set("window.size=191x56");
    terminal_set("window.title='Toxic & Ventures'");
    terminal_set("window.icon='icons/icon.ico'");
    terminal_set("input.encoding = utf-8");
    terminal_set("window.encoding=437");
    terminal_set("window.resizeable=false");
    SetFontSize(false);
    //terminal_set("window.cursor.visible=false");
    terminal_set("window.redraw_on_focus=true");

    DrawFrameFromFile("ramka.txt", 0, 0);
    LoadScreen();

    currentEntity = new Enemy("hound");

    Player* Gplayer = new Player(1);
    Gplayer->setName(L"Нюанс");
    Gplayer->setMaxEmotion(6);
    Gplayer->setModD20(1);
    int emot[6] = { 10,2,2,2,2,2 };
    Gplayer->setEmotions(emot, emot);

    //Тут ещё будет логика меню и настроек

	while (true) {

        LoadDungeon();

        currentEntity = new Entity();
        currentEntity->start(Gplayer, WelcomeTag);

        for (auto& i : QueueEntity) {
            currentEntity = new Entity();
            currentEntity->start(Gplayer, i.second);

            currentEntity = new Enemy(i.first);
            if (!currentEntity->start(Gplayer)) {
                currentEntity = new Entity();
                currentEntity->start(Gplayer, DeathTag);

                break;
            }
        }
        std::cout << FinalTag << std::endl;
        currentEntity = new Entity();
        currentEntity->start(Gplayer, FinalTag);
	}
}

void GameLogic::LoadDungeon(std::string DungeonName)
{
    std::ifstream file("RepositoryFun.json");
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: RepositoryFun");
    }
    std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    json data;
    try {
        data = json::parse(utf8Content);
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("Ошибка парсинга JSON: " + std::string(e.what()));
    }

    for (const auto& info : data["dungeon"]) {
        if (info["name"].get<std::string>() == "beta") {
            int NumberOfFloors = info["numberOfFloors"].get<int>();

            WelcomeTag = info["welcome"].get<std::string>();
            DeathTag = info["death"].get<std::string>();
            FinalTag = info["final"].get<std::string>();

            std::vector<std::string> transition = info["transition"].get<std::vector<std::string>>();
            std::string tag = "floor";
            std::vector<std::string> pullEnemy;

            for (int i = 0; i < NumberOfFloors; i++) {
                pullEnemy = info[tag + std::to_string(i + 1)].get<std::vector<std::string>>();

                QueueEntity.push_back({ pullEnemy[getRandomInt(0, pullEnemy.size() - 1)] ,transition[i] });
            }
        }
    }
}
