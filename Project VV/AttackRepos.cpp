#include "AttackRepos.h"

AttackEffect DefaultEffectFormulaTarget(struct AttackEffect Attack, std::array<int, 6>& minEmotions, std::array<int, 6>& Emotions) {

	Attack.Stress += 3 * (Emotions[0]- minEmotions[0]); //страх
	
	Attack.Stress += 4 * (Emotions[1] - minEmotions[1]);//злость

	Attack.Stress += 2 * (Emotions[2] - minEmotions[2]);//презрение
	Attack.Dodge -= 5 * (Emotions[2] - minEmotions[2]);

	Attack.Stress += 2 * (Emotions[5] - minEmotions[5]);//вдохновение

	return Attack;
}
AttackEffect DefaultEffectFormulaMe(struct AttackEffect Attack, std::array<int, 6>& minEmotions, std::array<int, 6>& Emotions) {

	Attack.Stress += 1* (Emotions[0] - minEmotions[0]); //страх
	Attack.Dodge += 2 * (Emotions[0] - minEmotions[0]);

	Attack.Defense -= 2 * (Emotions[1] - minEmotions[1]);//злость
	Attack.Dodge -= 2 * (Emotions[1] - minEmotions[1]);

	Attack.Stress -= (Emotions[3] - minEmotions[3]);//радость
	Attack.Defense += 2 * (Emotions[3] - minEmotions[3]);

	Attack.Stress -= (Emotions[4] - minEmotions[4]);//умиротворение
	Attack.Dodge += 2 * (Emotions[4] - minEmotions[4]);

	Attack.Stress -= (Emotions[5] - minEmotions[5]);// вдохновение

	return Attack;
}



std::vector<std::pair<std::string, std::array<int, 6>>> AttackRepository::GetAttackTagsPlayer()
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

	std::vector<std::pair<std::string, std::array<int, 6>>> attacks;

	for (const auto& attack : data["PlayerAttack"]) {
		attacks.push_back({ attack["tag"].get<std::string>(), attack["minEmotion"].get<std::array<int, 6>>() });
	}

	return attacks;
}

const AttackData AttackRepository::GetAttackByTagPlayer(const std::string& tag, std::array<int, 6>& Emotions)
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


static const AttackDataEnemy DefaultAttack = {
		"pants",
		{ 0, 0, 0 },
		{ 15, 0, 0 },
};

