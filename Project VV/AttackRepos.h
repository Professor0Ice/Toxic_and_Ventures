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

enum TypeAttack { damage, debuff, heal, buff, average };
// Типы фраз 

struct AttackDataEnemy {
	std::string tag;
	int type;
	AttackEffect EffectPlayer;
	AttackEffect EffectEnemy;
};

class AttackRepository {
	
public:
	static std::vector<std::pair<std::string, std::array<int, 6>>> GetAttackTagsPlayer();

	std::string GetAttackByType(std::vector<std::pair<std::string, int >>& attacks, int type);

	AttackDataEnemy GetAttackByTagEnemy(const std::string& tag);

	static const AttackData GetAttackByTagPlayer(const std::string& tag, std::array<int, 6>& Emotions);
};