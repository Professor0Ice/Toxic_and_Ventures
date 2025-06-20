#include "AttackRepos.h"

AttackEffect DefaultEffectFormulaTarget(struct AttackEffect attack, std::array<int, 6>& min_emotions, std::array<int, 6>& Emotions) {

	attack.stress += 3 * (Emotions[0]- min_emotions[0]); //страх
	
	attack.stress += 4 * (Emotions[1] - min_emotions[1]);//злость

	attack.stress += 2 * (Emotions[2] - min_emotions[2]);//презрение
	attack.dodge -= 5 * (Emotions[2] - min_emotions[2]);

	attack.stress += 2 * (Emotions[5] - min_emotions[5]);//вдохновение

	return attack;
}
AttackEffect DefaultEffectFormulaMe(struct AttackEffect attack, std::array<int, 6>& min_emotions, std::array<int, 6>& Emotions) {

	attack.stress += 1* (Emotions[0] - min_emotions[0]); //страх
	attack.dodge += 2 * (Emotions[0] - min_emotions[0]);

	attack.defense -= 2 * (Emotions[1] - min_emotions[1]);//злость
	attack.dodge -= 2 * (Emotions[1] - min_emotions[1]);

	attack.stress -= (Emotions[3] - min_emotions[3]);//радость
	attack.defense += 2 * (Emotions[3] - min_emotions[3]);

	attack.stress -= (Emotions[4] - min_emotions[4]);//умиротворение
	attack.dodge += 2 * (Emotions[4] - min_emotions[4]);

	attack.stress -= (Emotions[5] - min_emotions[5]);// вдохновение

	return attack;
}

std::vector<std::pair<std::string, std::array<int, 6>>> AttackRepository::GetAttackTagsPlayer()
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

	AttackDataEnemy result;
	std::array<int, 3> array;

	for (const auto& attack : data["EnemyAttack"]) {
		if (attack["tag"].get<std::string>() == tag) {

			result.tag = attack["tag"].get<std::string>();
			
			array = attack["PlayerEffect"].get<std::array<int, 3>>();
			result.effect_player = {array[0],array[1],array[2]};

			array = attack["EnemyEffect"].get<std::array<int, 3>>();
			result.effect_enemy = { array[0],array[1],array[2] };

			return result;
		}
	}
	throw std::runtime_error("Attack not found");
}

const AttackData AttackRepository::GetAttackByTagPlayer(const std::string& tag, std::array<int, 6>& Emotions)
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

	std::vector<std::pair<std::string, std::array<int, 6>>> attacks;

	AttackData result;
	std::array<int, 3> array;

	for (const auto& attack : data["PlayerAttack"]) {
		if (attack["tag"].get<std::string>() == tag) {

			result.tag = attack["tag"].get<std::string>();
			result.min_emotions = attack["minEmotion"].get<std::array<int, 6>>();

			array = attack["PlayerEffect"].get<std::array<int, 3>>();
			result.effect_player = DefaultEffectFormulaMe({array[0],array[1],array[2]},result.min_emotions, Emotions);

			array = attack["EnemyEffect"].get<std::array<int, 3>>();
			result.effect_enemy = DefaultEffectFormulaTarget({ array[0],array[1],array[2] }, result.min_emotions, Emotions);

			return result;
		}
	}
	throw std::runtime_error("Attack not found");
}