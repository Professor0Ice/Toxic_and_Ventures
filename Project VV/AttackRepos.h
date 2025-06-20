#include "Render.h"

struct AttackEffect {
	int stress;
	int defense;
	int dodge;
	int attack;
};

using EffectFormula = std::function<AttackEffect(struct AttackEffect attack, std::array<int, 6>& minEmotions, std::array<int, 6>& Emotions)>;

struct AttackData {
	std::string tag;
	std::array<int, 6> min_emotions;
	AttackEffect effect_player;
	AttackEffect effect_enemy;
};

enum TypeAttack { damage, debuff, heal, buff, average };
// Типы фраз 

enum TypePsycho { active, passive, normis };
// Типы поведения монстров

struct AttackDataEnemy {
	std::string tag;
	int type;
	AttackEffect effect_player;
	AttackEffect effect_enemy;
};

class AttackRepository {
	
public:
	static std::vector<std::pair<std::string, std::array<int, 6>>> GetAttackTagsPlayer();

	std::string GetAttackByType(std::vector<std::pair<std::string, int >>& attacks, int type);

	AttackDataEnemy GetAttackByTagEnemy(const std::string& tag);

	static const AttackData GetAttackByTagPlayer(const std::string& tag, std::array<int, 6>& Emotions);
};