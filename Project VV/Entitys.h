#include "Render.h"

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
private:
	std::string filename;
	std::wstring name;
	int emotions[6];
	int echo_emotions[6];
	int hp;
	int MaxArtefacts;
	std::vector<int> artefacts;
	int MaxVanInteract;
	std::vector<int> VanInteract;
	int Phrase; // ���. ����
	std::chrono::time_point<std::chrono::steady_clock> creationTime;
	int Time;

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
	std::wstring name;
	//PhaseName - ��� �������� ��� ����
	std::string EnemyFileName;
	int EnemyCoord[2];

	//std::vector<EnemyAttack> Attack; // - � ������� ��� ������ ��������� ����
	std::vector<int> NumAttack; // - ��� ������ ���������� ����

	int stress; // - ��������� ���������� ������� � ���������
	int stressPlayer = 0;

	int emotionLut[6]; // - ���� ������� ������
	//std::vector<artefact>; // - ����� ��������� ����� �������

	//std::vector<enchant>; // - ����������� �������

	//std::vector<PlayerAttack>; // ����� ������ � ���� ������

	Entity* NextAction() override;
	void UpdateStress();
	void UpdateStressEm();
	void LoadName();
	void UpdateEmotion();
};

class Mimik : public Enemy {
public:
	Mimik();
};
