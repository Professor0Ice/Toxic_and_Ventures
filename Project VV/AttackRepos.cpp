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




const std::vector<AttackData>& AttackRepositoryPlayer::GetAttacks()
{
	static const std::vector<AttackData> attacks = {
			{
				"primer", // Ёто тег ключевой фразы - при добавлении "_full" - будет тег самой фразы
				{ 2, 0, 0, 0, 0, 0 }, // Ёто мин. количество эмоций
				{ 10, -5, 0 }, // Ёто эффект на игрока - стресс, защита уклонение
				{ 20, -4, -2 }, // Ёто эффект на противника - стресс, защита уклонение
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget // сюда им€ формулы через которую будут проходить параметры
			},
			{
				"pants",
				{ 1, 0, 0, 0, 0, 0 }, // 1 страх
				{ 5, 0, 10 },
				{ 15, 0, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"punch",
				{ 0, 1, 0, 0, 0, 0 }, // 1 злость
				{ 0, -10, -5 },
				{ 25, 0, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"delete",
				{ 0, 1, 0, 0, 0, 0 }, // 1 злость
				{ 0, -10, -10},
				{ 25, -5, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"myphic",
				{ 0, 0, 1, 0, 0, 0 }, // 1 презрение
				{ 0, 0, 0},
				{ 10, -20, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"smile",
				{ 0, 0, 0, 0, 1, 0 }, // 1 умротворенность
				{ -15, 0, 10},
				{ 0, 0, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"carryon",
				{ 0, 0, 0, 1, 0, 0 }, // 1 радость
				{ -15, 10, 0},
				{ 0, 0, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"NPC",
				{ 0, 0, 0, 0, 0, 1 }, // 1 вдохновение
				{ -15, 0, 0},
				{ 10, 0, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"norespect",
				{ 1, 0, 1, 0, 0, 0}, // 1 страх 1 през
				{ 0, 0, 5},
				{ 15, -15, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"mountains",
				{ 0, 1, 0, 0, 0, 1}, // 1 злось 1 вдох
				{ -5, -10, 0},
				{ 30, 0, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"miserable",
				{ 0, 0, 1, 0, 0, 1}, // 1 през 1 вдох
				{ -5, 5, 0},
				{ 20, -10, 0 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"nature",
				{ 1, 0, 1, 0, 0, 0}, // 1 през 1 злость
				{ 5, -5, 0},
				{ 10, -10, -10 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"truth",
				{ 0, 0, 1, 0, 2, 0}, // 2 умир 1 през
				{ 0, 10, 15},
				{ 0, -5, -15 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},
			{
				"science",
				{ 2, 0, 0, 0, 0, 1}, // 2 страх 1 вдох
				{ -10, 10, 5},
				{ 5, -15, 0},
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			},

	};
	return attacks;
}

std::vector<std::pair<std::string, std::array<int, 6>>> AttackRepositoryPlayer::GetAttackTags()
{
	std::vector<std::pair<std::string, std::array<int, 6>>> tags;
	for (const auto& attack : GetAttacks()) {
		tags.push_back({ attack.tag, attack.minEmotions });
	}
	return tags;
}

const AttackData& AttackRepositoryPlayer::GetAttackByTag(const std::string& tag)
{
	for (const auto& attack : GetAttacks()) {
		if (attack.tag == tag)
			return attack;
	}
	throw std::runtime_error("Attack not found");
}
