#include "Entitys.h"

class GameLogic {
public:
	GameLogic();
private:
	std::string WelcomeTag;
	std::string FinalTag;
	std::string DeathTag;

	std::vector<std::pair<std::string, std::string >> QueueEntity;
	Entity* currentEntity;

	void LoadDungeon(std::string DungeonName = "beta");
};