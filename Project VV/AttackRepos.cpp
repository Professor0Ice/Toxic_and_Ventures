#include "AttackRepos.h"

AttackEffect DefaultEffectFormulaTarget(struct AttackEffect Attack, std::array<int, 6>& minEmotions, std::array<int, 6>& Emotions) {

	Attack.Stress += 3 * (Emotions[0]- minEmotions[0]); //�����
	
	Attack.Stress += 4 * (Emotions[1] - minEmotions[1]);//������

	Attack.Stress += 2 * (Emotions[2] - minEmotions[2]);//���������
	Attack.Dodge -= 5 * (Emotions[2] - minEmotions[2]);

	Attack.Stress += 2 * (Emotions[5] - minEmotions[5]);//�����������

	return Attack;
}
AttackEffect DefaultEffectFormulaMe(struct AttackEffect Attack, std::array<int, 6>& minEmotions, std::array<int, 6>& Emotions) {

	Attack.Stress += 1* (Emotions[0] - minEmotions[0]); //�����
	Attack.Dodge += 2 * (Emotions[0] - minEmotions[0]);

	Attack.Defense -= 2 * (Emotions[1] - minEmotions[1]);//������
	Attack.Dodge -= 2 * (Emotions[1] - minEmotions[1]);

	Attack.Stress -= (Emotions[3] - minEmotions[3]);//�������
	Attack.Defense += 2 * (Emotions[3] - minEmotions[3]);

	Attack.Stress -= (Emotions[4] - minEmotions[4]);//�������������
	Attack.Dodge += 2 * (Emotions[4] - minEmotions[4]);

	Attack.Stress -= (Emotions[5] - minEmotions[5]);// �����������

	return Attack;
}

std::vector<std::pair<std::string, std::array<int, 6>>> AttackRepository::GetAttackTagsPlayer()
{
	std::ifstream file("RepositoryFun.json");
	if (!file.is_open()) {
		throw std::runtime_error("�� ������� ������� ����: RepositoryFun");
	}

	std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	json data;
	try {
		data = json::parse(utf8Content);
	}
	catch (const json::parse_error& e) {
		throw std::runtime_error("������ �������� JSON: " + std::string(e.what()));
	}

	std::vector<std::pair<std::string, std::array<int, 6>>> attacks;

	for (const auto& attack : data["PlayerAttack"]) {
		attacks.push_back({ attack["tag"].get<std::string>(), attack["minEmotion"].get<std::array<int, 6>>() });
	}

	return attacks;
}

std::string AttackRepository::GetAttackByType(std::vector<std::pair<std::string, int>>& attacks, int type)
{	
	std::ifstream file("RepositoryFun.json");
	if (!file.is_open()) {
		throw std::runtime_error("�� ������� ������� ����: RepositoryFun");
	}

	std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	json data;
	try {
		data = json::parse(utf8Content);
	}
	catch (const json::parse_error& e) {
		throw std::runtime_error("������ �������� JSON: " + std::string(e.what()));
	}


	std::vector < std::string > TypeAttacks;

	for (const auto& info : attacks){
		for (const auto& attack : data["EnemyAttack"]) {
			if (attack["tag"].get<std::string>() == info.first) {
				if (attack["type"].get<int>() == type and info.second > 0) {
					TypeAttacks.push_back(info.first);
				}
			}
		}
	}

	if (TypeAttacks.size() == 0) {
		return "";
	}
	else if (TypeAttacks.size() == 1) {
		for (auto& info : attacks) {
			if (info.first == TypeAttacks[0]) {
				info.second--;
				return TypeAttacks[0];
			}
		}
	}
	else {
		std::string NameAttack = TypeAttacks[getRandomInt(0, TypeAttacks.size()-1)];

		for (auto& info : attacks) {
			if (info.first == NameAttack) {
				info.second--;
				return NameAttack;
			}
		}
	}
}

AttackDataEnemy AttackRepository::GetAttackByTagEnemy(const std::string& tag)
{
	std::ifstream file("RepositoryFun.json");
	if (!file.is_open()) {
		throw std::runtime_error("�� ������� ������� ����: RepositoryFun");
	}

	std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	json data;
	try {
		data = json::parse(utf8Content);
	}
	catch (const json::parse_error& e) {
		throw std::runtime_error("������ �������� JSON: " + std::string(e.what()));
	}

	AttackDataEnemy result;
	std::array<int, 3> array;

	for (const auto& attack : data["EnemyAttack"]) {
		if (attack["tag"].get<std::string>() == tag) {

			result.tag = attack["tag"].get<std::string>();
			
			array = attack["PlayerEffect"].get<std::array<int, 3>>();
			result.EffectPlayer = {array[0],array[1],array[2]};

			array = attack["EnemyEffect"].get<std::array<int, 3>>();
			result.EffectEnemy = { array[0],array[1],array[2] };

			return result;
		}
	}
	throw std::runtime_error("Attack not found");
}

const AttackData AttackRepository::GetAttackByTagPlayer(const std::string& tag, std::array<int, 6>& Emotions)
{
	std::ifstream file("RepositoryFun.json");
	if (!file.is_open()) {
		throw std::runtime_error("�� ������� ������� ����: RepositoryFun");
	}

	std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	json data;
	try {
		data = json::parse(utf8Content);
	}
	catch (const json::parse_error& e) {
		throw std::runtime_error("������ �������� JSON: " + std::string(e.what()));
	}

	std::vector<std::pair<std::string, std::array<int, 6>>> attacks;

	AttackData result;
	std::array<int, 3> array;

	for (const auto& attack : data["PlayerAttack"]) {
		if (attack["tag"].get<std::string>() == tag) {

			result.tag = attack["tag"].get<std::string>();
			result.minEmotions = attack["minEmotion"].get<std::array<int, 6>>();

			array = attack["PlayerEffect"].get<std::array<int, 3>>();
			result.EffectPlayer = DefaultEffectFormulaMe({array[0],array[1],array[2]},result.minEmotions, Emotions);

			array = attack["EnemyEffect"].get<std::array<int, 3>>();
			result.EffectEnemy = DefaultEffectFormulaTarget({ array[0],array[1],array[2] }, result.minEmotions, Emotions);

			return result;
		}
	}
	throw std::runtime_error("Attack not found");
}