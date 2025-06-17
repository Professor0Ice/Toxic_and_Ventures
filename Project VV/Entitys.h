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
	bool start(Player* playerN, std::string phraseTag = "");
protected:
	std::string phraseName = "welcome";
	virtual bool NextAction();
	Player* player;
};

class Enemy : public Entity {
public:
	Enemy(const std::string& filename); // загружаем данные отсюда
protected:
	std::string TagName;
	//PhaseName - начального для описания уже есть
	std::string EnemyFileName;

	int EnemyCoord[2] = { 1, 0 };

	std::string TagSearch;

	std::vector<std::pair<std::string, int >> EnemyAttackList; // tag и количество

	int PsychicType;
	int ChanceMadness;
	// Шанс, отклонения от образа атак по d20

	int stress; 
	int def;
	int dodge;

	int stressPlayer = 0;
	int DefendPlayer = 0;
	int DodgePlayer = 0;

	int emotionLut[6]; // - макс награда эмоций
	//std::vector<artefact>; // - какие артефакты могут выпасть

	//std::vector<enchant>; // - зачарование монстра

	std::vector<std::string> CritPhrase; // Атаки игрока с дабл уроном

	int DifficultyD20Roll = 2;

	float DifficultyEscape;

	bool NextAction() override;

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

	// Переменные для отслеживания состояния боя
	bool m_isDefeated = false;
	float m_defeatTimer = 0.0f;
	const float DEFEAT_DELAY = 2.0f; // Задержка после победы перед спавном нового врага

	void ResultStep(AttackRepository& Repos, std::string PlayerAttack, int Multiply);
	std::string EnemyStep(AttackRepository& Repos);
};

