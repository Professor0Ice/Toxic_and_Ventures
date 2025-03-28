#include "AttackRepos.h"

const fs::path FolderName = "Save";
const char key = 62;

class Player{
public:
	Player(int SaveFileN);
	void save();
	void load();
	void PrintParam();
	std::wstring getName() const;
	void setName(std::wstring w);
	void getEmotions(int* emotionsOut, int* echoEmotionsOut);
	void setEmotions(const int newEmotions[6], const int newEchoEmotions[6]);
	int getHP();
	void RemoveHP();
	int getMaxArtefacts();
	void setMaxArtefacts(int Max);
	std::vector<int> getArtefacts();
	void setArtefacts(std::vector<int> ar);
	int getMaxVanInteract();
	void setMaxVanInteract(int Max);
	std::vector<int> getVanInteract();
	void setVanInteract(std::vector<int> ar);
	int getPhrase();
	void AdditionPhase(int phaseNew);
	int getTime();
	int getMaxEmotion();
	void setMaxEmotion(int max);
	int getModD20();
	void setModD20(int Mod);

	bool DevideEmotion(int Emotion);
	bool ItsEcho(int Emotion);
	void AddEchoEmotion(int Emotion, int num = 1);
private:
	std::string filename;
	std::wstring name;
	int emotions[6];
	int echo_emotions[6];
	int MaxEmotion;
	int hp;
	int MaxArtefacts;
	std::vector<int> artefacts;
	int MaxVanInteract;
	std::vector<int> VanInteract;
	int Phrase; // кол. фраз
	std::chrono::time_point<std::chrono::steady_clock> creationTime;
	int Time;
	int ModD20;

	bool echo = false; //Чтобы узнать была потраченна ли эхо эмоция

	void xor_encrypt_decrypt(char* data, size_t size, char key);
	void serialize_data(std::ofstream& ofs);
	void deserialize_data(std::ifstream& ifs);
	void getLifeTime();
};

class Entity {
public:
	Entity* start(Player* playerN);
protected:
	std::string phraseName = "welcome";
	virtual Entity* NextAction();
	Player* player;
};

class Enemy : public Entity {
protected:
	std::string TagName;
	//PhaseName - начального для описания уже есть
	std::string EnemyFileName;
	int EnemyCoord[2];

	std::string TagSearch;

	std::vector<std::pair<std::string, int >> EnemyAttackList; // tag и количество
	int PsychicType = active;
	int ChanceMadness = 2;
	// Шанс, отклонения от образа атак по d20

	int stress = 60; 
	int def = 30;
	int dodge = 72;

	int stressPlayer = 60;
	int DefendPlayer = 30;
	int DodgePlayer = 72;

	int emotionLut[6]; // - макс награда эмоций
	//std::vector<artefact>; // - какие артефакты могут выпасть

	//std::vector<enchant>; // - зачарование монстра

	std::vector<std::string> CritPhrase; // Атаки игрока с дабл уроном

	int DifficultyD20Roll = 2;

	float DifficultyEscape = 1.0f;

	Entity* NextAction() override;

private:
	void UpdateStress();
	void UpdateStressEm();
	void LoadName();
	void UpdateEmotion();
	void LoadHP();
	void LoadArtefacts();
	void LoadBonus();
	void LoadEnemyInfo();
	void LoadButton();
	void UpdateButton();
	void LoadEmotionV();
	void UpdateEmotionVisual();
	void ClearAction();

	void DamagePlayer(AttackEffect damage);
	void DamageEnemy(AttackEffect damage);
	float RollD20(int difficulty);
	std::string SelectPhrase(AttackRepository& Repos);
	void SelectPhraseText(std::vector<std::string>& phrase, int select);

	void ResultStep(AttackRepository& Repos, std::string PlayerAttack, int Multiply);
	std::string EnemyStep(AttackRepository& Repos);
};

class Mimik : public Enemy {
public:
	Mimik();
};
