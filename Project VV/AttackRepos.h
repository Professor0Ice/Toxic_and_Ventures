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
};

class AttackRepository {

public:
	static std::vector<std::pair<std::string, std::array<int, 6>>> GetAttackTagsPlayer();

	static const AttackData GetAttackByTagPlayer(const std::string& tag, std::array<int, 6>& Emotions);
};

enum TypeAttack { damage, debaff, heal, baff, average};
// Типы фраз 

struct AttackDataEnemy {
	std::string tag;
	AttackEffect EffectPlayer;
	AttackEffect EffectEnemy;
};
