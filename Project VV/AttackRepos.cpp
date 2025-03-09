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




const std::vector<AttackData>& AttackRepositoryPlayer::GetAttacks()
{
	static const std::vector<AttackData> attacks = {
			{
				"primer", // ��� ��� �������� ����� - ��� ���������� "_full" - ����� ��� ����� �����
				{ 2, 0, 0, 0, 0, 0 }, // ��� ���. ���������� ������
				{ 10, -5, 0 }, // ��� ������ �� ������ - ������, ������ ���������
				{ 20, -4, -2 }, // ��� ������ �� ���������� - ������, ������ ���������
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget // ���� ��� ������� ����� ������� ����� ��������� ���������
			},
			{
				"xs",
				{ 1, 1, 2, 0, 0, 0 },
				{ 20, -4, -2 },
				{ 20, -4, -2 },
				DefaultEffectFormulaMe,
				DefaultEffectFormulaTarget
			}
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
