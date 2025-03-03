#include "Entitys.h"

int Nkey;

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
    DrawText(line, 95 - line.size() / 2, 50);

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
    // Интерфейс боя
    ClearTerminal();
    DrawFrameFromFile("fight interface.txt", 0, 0);

    UpdateStress();
    UpdateStressEm();
    LoadName();
    UpdateEmotion();
    

    DrawFrameFromFile(EnemyFileName, EnemyCoord[0], EnemyCoord[1]);

    while (true) {

        Nkey = terminal_read();

        BaseIfTerminal(Nkey);

        if (Nkey == TK_ENTER or Nkey == TK_SPACE) {
            break;
        }

		if (Nkey == TK_UP and int(stressPlayer / 10)<9) {
            stressPlayer += 10;
            UpdateStress();
            UpdateStressEm();
		}
        if (Nkey == TK_DOWN and int(stressPlayer / 10) > 0) {
			stressPlayer -= 10;
			UpdateStress();
			UpdateStressEm();
		}
    }
    // Конец интерфейса
    return new Entity;
}

void Enemy::UpdateStress()
{
	DrawFrameFromFile("stress" + std::to_string(int(stressPlayer / 10)) + ".txt", 128, 1);
}

void Enemy::LoadName()
{
    std::wstring str = LoadPhrase("name_h") + L" - [color=green]" + player->getName();
    DrawText(str, 143 + 48/2 - (str.size()-13)/2 - 2, 6);
}

void Enemy::UpdateEmotion()
{
    int emotions[6];
    int echo_emotions[6];
    std::wstring str;

    player->getEmotions(emotions, echo_emotions);
    std::cout << emotions[0];

    for (int i = 0; i < 6 ; i++) {
        std::cout << "emotion" + std::to_string(i);
        str = LoadPhrase("emotion" + std::to_string(i)) + L": " + std::to_wstring(emotions[i]) + L"  [color=gray]+" + std::to_wstring(echo_emotions[i]);
        DrawText(str, 144, 8 + i * 2);
    }
}

void Enemy::UpdateStressEm()
{
	std::wstring str = LoadPhrase("stress" + std::to_string(int(stressPlayer / 10)));
	DrawText(str, 133 - (str.size() / 2) - 1 + 38 / 2, 34);
}

Mimik::Mimik()
{
	EnemyFileName = "mimik.txt";
	EnemyCoord[0] = 6;
	EnemyCoord[1] = 2;
}
