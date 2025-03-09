#include "Render.h"

struct AttackEffect {
	int Stress;
	int Defense;
	int Dodge;
};

using EffectFormula = std::function<AttackEffect(struct AttackEffect Attack, std::array<int, 6>& minEmotions, std::array<int, 6>& Emotions)>;

struct AttackData {
	std::string tag;
	std::array<int, 6> minEmotions;
	AttackEffect EffectPlayer;
	AttackEffect EffectEnemy;
	EffectFormula resultMe;
	EffectFormula resultTarget;             // Функция для расчёта итогового эффекта с учётом лишних эмоций
};

class AttackRepositoryPlayer {
public:
	static const std::vector<AttackData>& GetAttacks();

	static std::vector<std::pair<std::string, std::array<int, 6>>> GetAttackTags();

	static const AttackData& GetAttackByTag(const std::string& tag);
};
