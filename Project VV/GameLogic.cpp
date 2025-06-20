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
	terminal_set("window.redraw_on_focus=true");
	DrawFrameFromFile("ramka.txt", 0, 0);
	LoadScreen();

	Player* main_player = new Player(1);
	main_player->setName(L"Нюанс");
	main_player->setMaxEmotion(6);
	main_player->setModD20(1);
	int initial_emotions[6] = { 10,2,2,2,2,2 };
	main_player->setEmotions(initial_emotions, initial_emotions);

	while (true) {

		LoadDungeon();

		currentEntity = new Entity();
		currentEntity->start(main_player, WelcomeTag);
		delete currentEntity;

		for (auto& i : QueueEntity) {
			currentEntity = new Entity();
			currentEntity->start(main_player, i.second);
			delete currentEntity;

			currentEntity = new Enemy(i.first);
			if (!currentEntity->start(main_player)) {
				delete currentEntity;
				currentEntity = new Entity();
				currentEntity->start(main_player, DeathTag);

				break;
			}
			delete currentEntity;
		}
		std::cout << FinalTag << std::endl;
		currentEntity = new Entity();
		currentEntity->start(main_player, FinalTag);
		delete currentEntity;
	}
}


void GameLogic::LoadDungeon(std::string DungeonName)
{
	std::ifstream file("RepositoryFun.json");
	if (!file.is_open()) {
		throw std::runtime_error("Не удалось открыть файл: RepositoryFun");
	}
	std::string utf8_content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	json data;
	try {
		data = json::parse(utf8_content);
	}
	catch (const json::parse_error& e) {
		throw std::runtime_error("Ошибка парсинга JSON: " + std::string(e.what()));
	}

	for (const auto& info : data["dungeon"]) {
		if (info["name"].get<std::string>() == "beta") {
			int number_of_floors = info["numberOfFloors"].get<int>();

			WelcomeTag = info["welcome"].get<std::string>();
			DeathTag = info["death"].get<std::string>();
			FinalTag = info["final"].get<std::string>();

			std::vector<std::string> transition = info["transition"].get<std::vector<std::string>>();
			std::string floor_tag = "floor";
			std::vector<std::string> pullEnemy;

			for (int i = 0; i < number_of_floors; i++) {
				pullEnemy = info[floor_tag + std::to_string(i + 1)].get<std::vector<std::string>>();

				QueueEntity.push_back({ pullEnemy[getRandomInt(0, pullEnemy.size() - 1)] ,transition[i] });
			}
		}
	}
}
