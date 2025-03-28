#include "Entitys.h"

std::array<int, 6> emotionPhrase = { 0,0,0,0,0,0 };
std::array<int, 6> DropEmotion = { 0,0,0,0,0,0 }; 
std::wstring dodgeColor[5] = {
	L"#aa1803",
	L"#bd613c",
	L"#dd6722",
	L"#6d8c00",
	L"#1a512e"
};
int NumButton = 0;
int LRbutton = 0;
int MaxLRbutton = 0;
int MinLRbutton = 0;

void delay(int milliseconds) {
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

Player::Player(int SaveFileN) {
    filename = "Save" + std::to_string(SaveFileN);

    name = L"-";
    emotions[0] = 0;
    emotions[1] = 0;
    emotions[2] = 0;
    emotions[3] = 0;
    emotions[4] = 0;
    emotions[5] = 0;
    echo_emotions[0] = 0;
    echo_emotions[1] = 0;
    echo_emotions[2] = 0;
    echo_emotions[3] = 0;
    echo_emotions[4] = 0;
    echo_emotions[5] = 0;
    hp = 3;
    MaxArtefacts = 3;
    MaxVanInteract = 2;
    Phrase = 0; // кол. фраз
}

void Player::save()
{
    getLifeTime();
    std::ofstream ofs(fs::current_path() / FolderName / filename, std::ios::binary);
    if (ofs) {
        // Сначала записываем размер строки
        size_t nameSize = name.size();
        ofs.write(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

        // Записываем строку как массив байтов
        ofs.write(reinterpret_cast<const char*>(name.c_str()), nameSize * sizeof(wchar_t));

        // Сериализуем и записываем другие данные
        serialize_data(ofs);
    }
}

void Player::load()
{
    std::ifstream ifs(fs::current_path() / FolderName / filename, std::ios::binary);
    if (ifs) {
        // Считываем размер строки
        size_t nameSize;
        ifs.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));

        // Считываем строку
        if (nameSize > 0 && nameSize < 1024) {
            name.clear();
            std::vector<wchar_t> tempName(nameSize);
            ifs.read(reinterpret_cast<char*>(tempName.data()), nameSize * sizeof(wchar_t));
            name = std::wstring(tempName.begin(), tempName.end());
        }

        // Загружаем остальные данные
        deserialize_data(ifs);
    }
}

void Player::PrintParam()
{
    std::wcout << emotions[0];
    std::cout << "\n";
    std::wcout << hp;
    std::cout << "\n";
}

std::wstring Player::getName() const
{
    return name;
}

void Player::setName(std::wstring w)
{
    name = w;
}

void Player::getEmotions(int* emotionsOut, int* echoEmotionsOut)
{
	for (int i = 0; i < 6; i++) {
		emotionsOut[i] = emotions[i];
		echoEmotionsOut[i] = echo_emotions[i];
	}
}

void Player::setEmotions(const int newEmotions[6], const int newEchoEmotions[6])
{
    for (int i = 0; i < 6; ++i) {
        emotions[i] = newEmotions[i];
        echo_emotions[i] = newEchoEmotions[i];
    }
}

int Player::getHP()
{
    return hp;
}

void Player::RemoveHP()
{
    hp -= 1;
}

int Player::getMaxArtefacts()
{
    return MaxArtefacts;
}

void Player::setMaxArtefacts(int Max)
{
    MaxArtefacts = Max;
}

std::vector<int> Player::getArtefacts()
{
    return artefacts;
}

void Player::setArtefacts(std::vector<int> ar)
{
    artefacts = ar;
}

int Player::getMaxVanInteract()
{
    return MaxVanInteract;
}

void Player::setMaxVanInteract(int Max)
{
    MaxVanInteract = Max;
}

std::vector<int> Player::getVanInteract()
{
    return VanInteract;
}

void Player::setVanInteract(std::vector<int> ar)
{
    VanInteract = ar;
}

int Player::getPhrase()
{
    return Phrase;
}

void Player::AdditionPhase(int phaseNew)
{
    Phrase += phaseNew;
}

int Player::getTime()
{
    return Time;
}

int Player::getMaxEmotion()
{
    return MaxEmotion;
}

void Player::setMaxEmotion(int max)
{
    MaxEmotion = max;
}

int Player::getModD20()
{
    return ModD20;
}

void Player::setModD20(int Mod)
{
    ModD20 = Mod;
}

bool Player::DevideEmotion(int Emotion)
{
    if (echo_emotions[Emotion] > 0) {
        echo_emotions[Emotion]--;
        return true;
    }
    if (emotions[Emotion] > 0) {
        emotions[Emotion]--;
        echo = true;
        return true;
    }
    return false;
}

bool Player::ItsEcho(int Emotion)
{   
    if (echo) {
        echo = !echo;
        return true;
    }
    return false;
}

void Player::AddEchoEmotion(int Emotion, int num)
{
    echo_emotions[Emotion] += num;
}

void Player::xor_encrypt_decrypt(char* data, size_t size, char key)
{
    for (size_t i = 0; i < size; ++i) {
        data[i] ^= key;
    }
}

void Player::serialize_data(std::ofstream& ofs)
{
    // Сериализуем массив эмоций
    ofs.write(reinterpret_cast<char*>(&emotions), sizeof(emotions));
    ofs.write(reinterpret_cast<char*>(&echo_emotions), sizeof(echo_emotions));

    // Сериализуем другие данные
    ofs.write(reinterpret_cast<char*>(&hp), sizeof(hp));
    ofs.write(reinterpret_cast<char*>(&MaxArtefacts), sizeof(MaxArtefacts));

    size_t artefacts_size = artefacts.size();
    ofs.write(reinterpret_cast<char*>(&artefacts_size), sizeof(artefacts_size));
    if (artefacts_size > 0) {
        ofs.write(reinterpret_cast<char*>(artefacts.data()), artefacts_size * sizeof(int));
    }

    // Повторяем это для всех остальных данных...
}

void Player::deserialize_data(std::ifstream& ifs)
{
    // Десериализуем массив эмоций
    ifs.read(reinterpret_cast<char*>(&emotions), sizeof(emotions));
    ifs.read(reinterpret_cast<char*>(&echo_emotions), sizeof(echo_emotions));

    // Десериализуем другие данные
    ifs.read(reinterpret_cast<char*>(&hp), sizeof(hp));
    ifs.read(reinterpret_cast<char*>(&MaxArtefacts), sizeof(MaxArtefacts));

    size_t artefacts_size;
    ifs.read(reinterpret_cast<char*>(&artefacts_size), sizeof(artefacts_size));
    if (artefacts_size > 0) {
        artefacts.resize(artefacts_size);
        ifs.read(reinterpret_cast<char*>(artefacts.data()), artefacts_size * sizeof(int));
    }

    // Повторяем для остальных данных...
}

void Player::getLifeTime()
{
    auto currentTime = std::chrono::steady_clock::now();
    auto currentSessionLifetime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - creationTime).count();
    creationTime = currentTime;
    Time += static_cast<int>(currentSessionLifetime);
}

Entity* Entity::start(Player* playerN)
{
    int Nkey;

    player = playerN;

    // Интерфейс диалога
    ClearTerminal();
    DrawFrameFromFile("dialogue.txt", 0, 0);

    const std::vector<std::wstring> phrase = SplitString(LoadPhrase(phraseName));
    std::wstring line;
    int k = 0;

    for (auto i : phrase) {
        if (line.size() + i.size() >= 138) {
            DrawText(line, 25, 20 + k);
            k++;
            line = i + L" ";
        }
        else {
            line += i + L" ";
        }
    }

    if (line.size() != 0) {
        DrawText(line, 26, 20 + k);
    }

    line = LoadPhrase("next");
    DrawText(line, 95 - static_cast<int>(line.size() / 2), 50);

    while (true) {

        Nkey = terminal_read();

        BaseIfTerminal(Nkey);

        if (Nkey == TK_ENTER or Nkey == TK_SPACE) {
            break;
        }
    }
    // Конец интерфейса

    return NextAction();
}

Entity* Entity::NextAction()
{
    std::cout << "Entity is destroyed";
    return new Entity;
}

Entity* Enemy::NextAction()
{
    int Nkey;
    int sum;
    bool focus = false;
    bool UpDown = true;
    bool Bruh;

    std::string TagAttack;

    float Multiply = 1;
    AttackRepository Repos;

    // Интерфейс боя
	ClearTerminal();
	DrawFrameFromFile("fight interface.txt", 0, 0);

	UpdateStress();
	UpdateStressEm();
	LoadName();
	UpdateEmotion();
	LoadArtefacts();
	LoadBonus();
	LoadEnemyInfo();
	UpdateButton();
    

    DrawFrameFromFile(EnemyFileName, EnemyCoord[0], EnemyCoord[1]);

    while (true) {

        Nkey = terminal_read();

        BaseIfTerminal(Nkey);

		if (Nkey == TK_DOWN) {
            if (NumButton < 4 and UpDown) {
                focus = false;
                NumButton++;
                UpdateButton();
                ClearAction();
            }
		}
        if (Nkey == TK_UP) {
            if (NumButton > 1 and UpDown) {
                focus = false;
				NumButton--;
				UpdateButton();
                ClearAction();
			}
		}

		if (Nkey == TK_LEFT) {
			if (focus) {
				switch (NumButton) {
				case 1:
                    if (LRbutton > MinLRbutton -1) {
                        LRbutton--;
                        LoadEmotionV();
                    }
					break;
				default:
					break;
				}
			}
		}
		if (Nkey == TK_RIGHT) {
			if (focus) {
				switch (NumButton) {
				case 1:
					if (LRbutton < MaxLRbutton - 1) {
						LRbutton++;
						LoadEmotionV();
					}
					break;
				default:
					break;
				}
			}
		}

		if (Nkey == TK_ENTER or Nkey==TK_SPACE) {
			if(!focus){
				switch (NumButton) {
				case 1:
                    focus = true;
					MinLRbutton = 1;
					MaxLRbutton = 7;
					LRbutton = 4;
					LoadEmotionV();
					break;
				case 4:
					UpDown = false;
					Multiply = RollD20(20);

					if (Multiply >= DifficultyEscape) {
						return new Entity;
					}
					else {
						NumButton = 0;
						LRbutton = 0;
						UpdateStress();
						UpdateStressEm();
						UpdateEmotion();
						LoadEnemyInfo();
						UpdateButton();
						ClearAction();
						UpDown = true;
					}
				default:
					break;
				}
			}
            else {
				switch (NumButton) {
				case 1:
					sum = 0;
					for (auto i : emotionPhrase) {
						sum += i;
					}

                    if (LRbutton < 6) {
						if (sum < player->getMaxEmotion()) {
                            if (player->DevideEmotion(LRbutton)) {
								emotionPhrase[LRbutton] += 1;
								if (player->ItsEcho(LRbutton)) {
                                    DropEmotion[LRbutton]++;
								}
                                UpdateEmotion();
								LoadEmotionV();
                            }
						}
					}

                    else  if(LRbutton == 6 and sum > 0){
                        UpDown = false;
                        ClearAction();
                        
                        TagAttack = SelectPhrase(Repos);

                        ClearAction();

                        Multiply = 2;

                        Bruh = false;
                        for (auto i : CritPhrase) {
                            if (i == TagAttack) {
                                Bruh = true;
                            }
                        }
                        if (!Bruh) {
                            Multiply = RollD20(DifficultyD20Roll);
                        }

                        ResultStep(Repos, TagAttack, Multiply);

                        if (stress >= 100) {
                            return new Entity;
                        }
                        if (stressPlayer >= 100) {
                            return new Entity;
                        }
                        
						NumButton = 0;
						LRbutton = 0;
						UpdateStress();
						UpdateStressEm();
						UpdateEmotion();
						LoadEnemyInfo();
						UpdateButton();
                        ClearAction();
                        UpDown = true;
                    }
                    break;
				default:
					break;
				}
            }
		}

        if (Nkey == TK_BACKSPACE or Nkey == TK_DELETE) {
            if (focus and NumButton == 1 and emotionPhrase[LRbutton] != 0 and LRbutton!=7) {
				emotionPhrase[LRbutton] -= 1;
                player->AddEchoEmotion(LRbutton);
                UpdateEmotion();
				LoadEmotionV();
            }
        }
    }
}

void Enemy::UpdateStress()
{
    std::wstring Color = dodgeColor[0];

	if (DodgePlayer > -1) {
        if (DodgePlayer < 100) { Color = dodgeColor[int(DodgePlayer / 20)]; }
		else { Color = dodgeColor[4]; }
	}
    if ((stressPlayer > -1 and stressPlayer < 99)) {
        DrawFrameFromFile("stress" + std::to_string(int(stressPlayer / 10)) + ".txt", 128, 1, false, Color);
    }
	if (DefendPlayer > -1) {
		if (DefendPlayer < 101) { DrawFrameFromFile("Dstress" + std::to_string(int(DefendPlayer / 10)) + ".txt", 128, 1, false); }
		else { DrawFrameFromFile("Dstress10.txt", 128, 1, false); }
	}
    terminal_refresh();
}

void Enemy::LoadName()
{
    std::wstring str = LoadPhrase("name_h") + L" - [color=green]" + player->getName();
    DrawText(str, 139 + static_cast<int>(52/2) - static_cast<int>((str.size()-13)/2) - 4, 5);
    LoadHP();
}

void Enemy::LoadHP()
{
	std::wstring str = LoadPhrase("hp") + L": " + std::to_wstring(player->getHP());
	DrawText(str, 139 + static_cast<int>(52 / 2 - (str.size()) / 2) - 4, 6);
}

void Enemy::LoadArtefacts()
{
    std::wstring str;

    str = LoadPhrase("artefacts");
    DrawText(str, 170 + static_cast<int>(18 / 2 - (str.size() +1 ) / 2), 9);

    for (auto i : player->getArtefacts()) {
        //потом реализую - пока нет класса артефактов
        str = L"Хи-хи-ха";
        DrawText(str, 170 + static_cast<int>(18 / 2 - (str.size()) / 2), 11);
    }
    str = L"Хи-хи-ха";
    DrawText(str, 170 + static_cast<int>(18 / 2 - (str.size()) / 2) , 11);
}

void Enemy::LoadBonus()
{
    DrawText(LoadPhrase("bonus") + L":", 142, 20);
}

void Enemy::LoadEnemyInfo()
{
    std::wstring str;
    std::wstring space = L" ";

    str = space*36;
    for (int i = 0; i < 16; i++) {
        DrawText(str, 3, 37 + i, true, false);
    }

    str = LoadPhrase(TagName);
    DrawText(str, 3 + static_cast<int>(38/2 - (str.size() - 15 )/2) -2, 38,true,false);

    str = LoadPhrase("En_stress") + L": " + LoadPhrase("En_stress" + std::to_string(int(stress/20+1)));
    DrawText(str, 3 + static_cast<int>(38 / 2 - (str.size()) / 2) - 2, 40, true, false);

	str = LoadPhrase("En_def") + L": " + LoadPhrase("En_def" + std::to_string(int(def / 20 +1)));
	DrawText(str, 3 + static_cast<int>(38 / 2 - (str.size()) / 2) - 2, 41, true, false);

	str = LoadPhrase("En_dod") + L": " + LoadPhrase("En_dod" + std::to_string(int(dodge / 20 +1)));
	DrawText(str, 3 + static_cast<int>(38 / 2 - (str.size()) / 2) - 2, 42, true, false);

	str = LoadPhrase("En_bonus") + L":";
	DrawText(str, 3 + static_cast<int>(38 / 2 - (str.size()) / 2) - 2, 44, true, false);

    terminal_refresh();
}

void Enemy::LoadButton()
{
    std::wstring str;
    for (int i = 0; i < 3; i++) {
        str = LoadPhrase("button_battle" + std::to_string(i+1));

        DrawFrameFromFile("button_battle.txt",157,36+i*5, false);
        DrawText(str, 163 + static_cast<int>(24 / 2 - (str.size()) / 2), 38+i*5);
    }
    DrawFrameFromFile("button_battle_ext.txt", 157, 51);
}

void Enemy::UpdateButton()
{
    std::wstring str;

    if (NumButton == 0) {
        LoadButton();
        terminal_refresh();
    }
    else {
        LoadButton();
		if(NumButton<4){
            DrawFrameFromFile("button_battle_act.txt", 157, 36 + (NumButton - 1) * 5, false);
			str = LoadPhrase("button_battle" + std::to_string(NumButton));
			DrawText(str, 163 + static_cast<int>(24 / 2 - (str.size()) / 2), 38 + (NumButton-1) * 5);
		}
        else {
            DrawFrameFromFile("button_battle_ext_act.txt", 157, 36 + (NumButton - 1) * 5, false);
        }
        terminal_refresh();
    }
}

void Enemy::LoadEmotionV()
{
	std::vector<std::wstring> phrases;
    std::wstring plus = L"+";

	for (int i = 0; i < 6; i++) {
		phrases.push_back(LoadPhrase("emotion" + std::to_string(i)));
	}
	phrases.push_back(LoadPhrase("phrase_enter"));

	int totalWordsLength = 0;
	for (const auto& word : phrases) {
		totalWordsLength += static_cast<int>(word.size());
	}

	int availableSpace = 98 - totalWordsLength;
	int numGaps = static_cast<int>(phrases.size()) - 1;
	int gapSize = (numGaps > 0 && availableSpace > 0) ? (availableSpace / numGaps) : 0;

	int posX = 49;

    phrases[LRbutton] = L"[color=#D9C906]" + phrases[LRbutton];
    for (int i = 0; i < 6; i++) {
		DrawText(phrases[i], posX, 50);
        DrawText(plus*emotionPhrase[i]+L" ", posX, 51);

        posX += static_cast<int>(phrases[i].size()) + gapSize -15*(LRbutton == i);
	}
    DrawText(phrases[6], posX, 50);

    UpdateEmotionVisual();
}

void Enemy::UpdateEmotionVisual()
{
    int count = player->getMaxEmotion();
    std::string str;
	std::wstring c[7] = 
    { 
        L"#FFFFFF",
		L"#AA1712",
		L"#3739B0",
		L"#5C781B",
		L"#FFC936",
		L"#60DAE1",
		L"#AE6BDE",
    };
    std::vector<int> ci;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < emotionPhrase[i]; j++) {
            ci.push_back(i+1);
        }
    }
    while (ci.size() < player->getMaxEmotion()){
        ci.push_back(0);
    }
    
    for (int i = 0; i < count; i++) {
        str = "emo_" + std::to_string(count - 2) + "_"  + std::to_string(i + 1) + ".txt";
        DrawFrameFromFile(str, 84, 39, false, c[ci[i]]);
    }
    terminal_refresh();
}

void Enemy::ClearAction()
{
    std::wstring space = L" ";
	for (int i = 0; i < 17; i++) {
		DrawText(space * 106, 45, 37 + i);
	}
    DrawFrameFromFile("fight action.txt",45,36);
}

std::vector<int> generateShuffledArray(int n) {
	std::vector<int> arr(n);
	for (int i = 0; i < n; ++i) {
		arr[i] = i + 1;
	}

	std::random_device rd;
	std::seed_seq seed{ rd(), static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
	std::mt19937 g(seed);

	std::shuffle(arr.begin()  , arr.end(), g);

	return arr;
}

void Enemy::DamagePlayer(AttackEffect damage)
{
    DefendPlayer += damage.Defense;
    DodgePlayer += damage.Dodge;
    std::cout << "\n [Игрок] ";
    std::cout << "\n Защита игрока увеличина на " << damage.Defense;
    std::cout << "\n уклонение игрока игрока увеличино на " << damage.Dodge;

    if(DodgePlayer < getRandomInt(1,100) + 20){
        if (damage.Stress > 0 and DefendPlayer > 0) {
            DefendPlayer -= damage.Stress;
            std::cout << "\n щит игрока уменьшен на " << damage.Stress;
            if (DefendPlayer < 0) {
                damage.Stress = DefendPlayer * (-1);
                DefendPlayer = 0;
                stressPlayer += damage.Stress;
                std::cout << "\n игроку нанесли стресса " << damage.Stress;
            }
        }
        else {
            stressPlayer += damage.Stress;
            std::cout << "\n игроку нанесли стресса " << damage.Stress;
        }
    }
    std::cout << "\n";
}

void Enemy::DamageEnemy(AttackEffect damage)
{
	def += damage.Defense;
	dodge += damage.Dodge;
    std::cout << "\n [Монстр] ";
	std::cout << "\n Защита монстра увеличина на " << damage.Defense;
	std::cout << "\n уклонение монстра увеличино на " << damage.Dodge;

	if (dodge < getRandomInt(1, 100) + 50) {
		if (damage.Stress > 0 and def > 0) {
			def -= damage.Stress;
            std::cout << "\n щит монстра уменьшен на " << damage.Stress;
			if (def < 0) {
				damage.Stress = def * (-1);
				def = 0;
                stress += damage.Stress;
                std::cout << "\n монстру нанесли стресса " << damage.Stress;
			}
		}
		else {
			stress += damage.Stress;
            std::cout << "\n монстру нанесли стресса " << damage.Stress;
		}
	}
    std::cout << "\n";
}

float Enemy::RollD20(int difficulty)
{
    DrawFrameFromFile("d20.txt", 86, 38);
    std::wstring str;
    std::vector<int> values = generateShuffledArray(20);
    int Nkey;
    int n;
    int i = 0;
    int MaxI = 60;

    str = LoadPhrase("mod_d20") + L" +" + std::to_wstring(player->getModD20());
    DrawText(str, 98- static_cast<int>(str.size()/2),51);

	str = L"[color=#D9C906]" + LoadPhrase("stop_d20");
	DrawText(str, 98 - static_cast<int>((str.size()-15) / 2),53);

    while(true){
        Nkey = terminal_peek();

        BaseIfTerminal(Nkey);

        n = values[i % 20];
		if (n / 10 < 1) {
			DrawText(L"[offset=0,5]" + std::to_wstring(n), 97, 43);

			DrawText(L" ", 96, 43);
			DrawText(L" ", 98, 43);
		}
		else {
			DrawText(L"[offset=3,5]" + std::to_wstring(int(n / 10)), 96, 43);
			DrawText(L"[offset=-3,5]" + std::to_wstring(int(n % 10)), 98, 43);

			DrawText(L" ", 97, 43);
		}

		if (Nkey == TK_ENTER or Nkey == TK_SPACE) {
            delay(500);
            if (n == 20) {
                return 2.0f;
            }
            if (n > 14) {
                return 1.3f;
            }
			if (n > 9) {
				return 1.1f;
			}
			if (n > 4) {
				return 1.0f;
			}
			if (n > 1) {
				return 0.7f;
			}
            else {
                return -0.2f;
            }
		}
        else if (i >= MaxI) {
            delay(500);
			if (n == 20) {
				return 2.0f;
			}
			if (n > 14) {
				return 1.3f;
			}
			if (n > 9) {
				return 1.1f;
			}
			if (n > 4) {
				return 1.0f;
			}
			if (n > 1) {
				return 0.7f;
			}
			else {
				return -0.2f;
			}
        }

		delay(280);
		DrawFrameFromFile("d20An.txt", 86, 38, true, L"", false);
		delay(150);
		DrawFrameFromFile("d20.txt", 86, 38, true, L"", false);
		i++;
    }
}

std::string Enemy::SelectPhrase(AttackRepository& Repos)
{
	std::vector<std::pair<std::string, std::array<int, 6>>> attack = Repos.GetAttackTagsPlayer();
    std::vector<std::pair<std::string,int>> pullFull;
    std::vector<std::string> pullMaxPhrase;
    std::vector<std::string> pull;
    bool correct;
    int MaxPh = 0;
    int Nkey;

    std::string tagDestoy;

	for (auto i : attack) {
        correct = true;
        for (int j = 0; j < 6; j++) {
            if (emotionPhrase[j] < i.second[j]) {
                correct = false;
            }
        }
        if (correct) {
            pullFull.push_back({ i.first, std::accumulate(std::begin(i.second),std::end(i.second), 0) });
            MaxPh = max(MaxPh, std::accumulate(std::begin(i.second), std::end(i.second), 0));
        }
	}

    for (auto i : pullFull) {
        if (i.second == MaxPh) {
            pullMaxPhrase.push_back(i.first);
        }
    }
    tagDestoy = pullMaxPhrase[getRandomInt(0, static_cast<int>(pullMaxPhrase.size()) - 1)];
    pull.push_back(tagDestoy);
    for (int i = 0; i < pullFull.size(); i++) {
        if (tagDestoy == pullFull[i].first) {
            pullFull.erase(pullFull.begin() + i);
            break;
        }
    }
    for (int i = 0; i < 2; i++) {
		if(pullFull.size()>0){
			tagDestoy = pullFull[getRandomInt(0, static_cast<int>(pullFull.size()) - 1)].first;
			pull.push_back(tagDestoy);
			for (int j = 0; j < pullFull.size(); j++) {
				if (tagDestoy == pullFull[j].first) {
					pullFull.erase(pullFull.begin() + j);
					break;
				}
			}
		}
    }

    while (pull.size() < 3) {
        pull.push_back(pull[0]);
    }

    DrawFrameFromFile("select_phrase.txt",85,38);
	MinLRbutton = 7;
	MaxLRbutton = 9;
	LRbutton = 7;
    SelectPhraseText(pull, LRbutton-7);

    while (true) {
        Nkey = terminal_read();
        BaseIfTerminal(Nkey);

        if (Nkey == TK_ENTER or Nkey == TK_SPACE) {
            return pull[LRbutton - 7];
        }

		if (Nkey == TK_LEFT) {
            if (LRbutton > MinLRbutton) {
				LRbutton--;
                SelectPhraseText(pull, LRbutton - 7);
			}
		}
		if (Nkey == TK_RIGHT) {
			if (LRbutton < MaxLRbutton) {
				LRbutton++;
                SelectPhraseText(pull, LRbutton - 7);
			}
		}
    }
}

void Enemy::SelectPhraseText(std::vector<std::string>& phrase, int select)
{
    std::wstring str;
    for (int i = 0; i < 3; i++) {
        str = LoadPhrase(phrase[i]);
        if (i == select) {
            str = L"[color=#D9C906]" + str;
            DrawText(str, 66 + i * 30 - static_cast<int>((str.size()-15) / 2), 49 + 2 * (i == 1));
        }
        else {
            DrawText(str, 66 + i * 30 - static_cast<int>(str.size() / 2), 49 + 2 * (i == 1));
        }
    }
}

void Enemy::ResultStep(AttackRepository& Repos, std::string PlayerAttack, int Multiply)
{
    std::vector<std::wstring> phrase;
    std::wstring line;
    int k;
    int endk;

    AttackData AttackPlayer;
    AttackDataEnemy AttackEnemy;

    AttackEffect AttackEffect;

    AttackEnemy = Repos.GetAttackByTagEnemy(EnemyStep(Repos));

    phrase = SplitString(L"[color=green](" + player->getName() + L")[/color] " + LoadPhrase(PlayerAttack + "_full"));
    line = L"";
    k = 0;

    ClearAction();
    for (auto i : phrase) {
        if (line.size() + i.size() >= 104) {
            DrawText(line, 46 + 3 * (k != 0), 38 + k);
            k++;
            line = i + L" ";
        }
        else {
            line += i + L" ";
        }
    }
    if (line.size() != 0) {
        DrawText(line, 46 + 3 * (k != 0), 38 + k);
        k++;
    }

    phrase = SplitString(L"[color=red](" + LoadPhrase(TagName) + L")[/color] " + LoadPhrase(AttackEnemy.tag));
    line = L"";
    endk = k+2;
    k = 0;

    for (auto i : phrase) {
        if (line.size() + i.size() >= 104) {
            DrawText(line, 46 + 3 * (k != 0), 38 + k + endk);
            k++;
            line = i + L" ";
        }
        else {
            line += i + L" ";
        }
    }
    if (line.size() != 0) {
        DrawText(line, 46 + 3 * (k != 0), 38 + k + endk);
    }

    k = terminal_read();
    while (true) {
        k = terminal_read();

        BaseIfTerminal(k);

        if (k == TK_ENTER or k == TK_SPACE) {
            break;
        }
    }

    AttackPlayer = AttackRepository().GetAttackByTagPlayer(PlayerAttack, emotionPhrase);

    AttackEffect = AttackPlayer.EffectPlayer;

    AttackEffect.Defense = static_cast<int>(AttackEffect.Defense * Multiply);
    AttackEffect.Dodge = static_cast<int>(AttackEffect.Dodge * Multiply);
    AttackEffect.Stress = static_cast<int>(AttackEffect.Stress * Multiply);

    DamagePlayer(AttackEffect);

    AttackEffect = AttackPlayer.EffectEnemy;

    AttackEffect.Defense = static_cast<int>(AttackEffect.Defense * Multiply);
    AttackEffect.Dodge = static_cast<int>(AttackEffect.Dodge * Multiply);
    AttackEffect.Stress = static_cast<int>(AttackEffect.Stress * Multiply);

    DamageEnemy(AttackEffect);

    for (int i = 0; i < 6; i++) {
        player->AddEchoEmotion(i, DropEmotion[i]);
    }
    for (int i = 0; i < 6; i++) {
        DropEmotion[i] = 0;
        emotionPhrase[i] = 0;
    }

    

    AttackEffect.Defense = AttackEnemy.EffectPlayer.Defense;
    AttackEffect.Dodge = AttackEnemy.EffectPlayer.Dodge;
    AttackEffect.Stress = AttackEnemy.EffectPlayer.Stress;

    DamagePlayer(AttackEffect);

    AttackEffect.Defense = AttackEnemy.EffectEnemy.Defense;
    AttackEffect.Dodge = AttackEnemy.EffectEnemy.Dodge;
    AttackEffect.Stress = AttackEnemy.EffectEnemy.Stress;

    DamageEnemy(AttackEffect);

    ClearAction();
}

std::string Enemy::EnemyStep(AttackRepository& Repos)
{
    std::string result;
    std::vector<std::string> StockLifePhrase;
    int r;

    if (ChanceMadness >= getRandomInt(1, 20)) {
        for (auto& i : EnemyAttackList) {
            if (i.second > 0) {
                StockLifePhrase.push_back(i.first);
            }
        }

        if (StockLifePhrase.size() == 0) return "silence";

        r = getRandomInt(0, StockLifePhrase.size() - 1);

        for (auto& i : EnemyAttackList) {
            if (i.first == StockLifePhrase[r]) {
                i.second--;
            }
        }

        return StockLifePhrase[r];
    }
    else {
        switch (PsychicType)
        {
        case active:
            if (stress >= 75 and stressPlayer <= 70) {
                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff); 

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, average); 

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, damage);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, debuff);

                if (result != "") return result;

                return "silence";
            }
            else {
                if (DefendPlayer >= 30 or DodgePlayer >= 50) {
                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;
                }
                else {
                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;
                }

                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                return "silence";
            }
        case passive:
            if (stress >= 60) {
                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, damage);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, debuff);

                if (result != "") return result;

                return "silence";
            }
            else if (stress >= 40) {
                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, debuff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, damage);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                return "silence";
            }
            else {
                if (DefendPlayer >= 30 or DodgePlayer >= 50) {
                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;
                }
                else {
                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;
                }

                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                return "silence";
            }
        case normis:
            if (stress >= 70) {
                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, damage);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, debuff);

                if (result != "") return result;

                return "silence";
            }
            else if (stress >= 50 and def + dodge > 40) {
                if (DefendPlayer >= 30 or DodgePlayer >= 50) {
                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;
                }
                else {
                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;
                }

                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                return "silence";
            }
            else {
                result = Repos.GetAttackByType(EnemyAttackList, average);

                if (result != "") return result;

                result = Repos.GetAttackByType(EnemyAttackList, buff);

                if (result != "") return result;

                if (DefendPlayer >= 30 or DodgePlayer >= 50) {
                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;
                }
                else {
                    result = Repos.GetAttackByType(EnemyAttackList, damage);

                    if (result != "") return result;

                    result = Repos.GetAttackByType(EnemyAttackList, debuff);

                    if (result != "") return result;
                }

                result = Repos.GetAttackByType(EnemyAttackList, heal);

                if (result != "") return result;

                return "silence";
            }
        default:
            break;
        }
    }
}

void Enemy::UpdateEmotion(){
    int emotions[6];
    int echo_emotions[6];
    std::wstring str;

    player->getEmotions(emotions, echo_emotions);

    str = LoadPhrase("emotionSum") + L": " + std::to_wstring(emotions[0] + emotions[1] + emotions[2] + emotions[3] + emotions[4] + emotions[5] +
        echo_emotions[0] + echo_emotions[1] + echo_emotions[2] + echo_emotions[3] + echo_emotions[4] + echo_emotions[5]);
	DrawText(str, 142, 9);

    for (int i = 0; i < 6 ; i++) {
        str = LoadPhrase("emotion" + std::to_string(i)) + L": " + std::to_wstring(emotions[i]) + L"  [color=gray]+" + std::to_wstring(echo_emotions[i]);
        DrawText(str, 142, 11 + i + int(i>=3) );
    }
}

void Enemy::UpdateStressEm()
{
    if (stressPlayer > -1 and stressPlayer < 99) {
		std::wstring str = LoadPhrase("stress" + std::to_string(int(stressPlayer / 10)));
		DrawText(str, 133 - static_cast<int>(str.size() / 2) - 1 + static_cast<int>(38 / 2), 34);
	}
}

Mimik::Mimik()
{
    TagName = "mimik";
	EnemyFileName = "mimik.txt";
	EnemyCoord[0] = 1;
	EnemyCoord[1] = 0;
    stress = 60;
    dodge = 30;
    def = 0;
}
