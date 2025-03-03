#include "Render.h"


std::vector<std::string> screenFiles = {};
std::vector<int> screenFilesCoord = {};
std::vector<std::wstring> screenText = {};
std::vector<int> screenTextCoord = {};

extern std::string filePathLang = "ru.json";

//штука для переработки в встринг
std::wstring Utf8ToWstring(const std::string& str) {
    std::wstring result;
    wchar_t wch = 0;
    int bytes = 0;

    for (unsigned char ch : str) {
        if (ch <= 0x7F) { // ASCII символы
            result += wchar_t(ch);
        }
        else if ((ch & 0xC0) == 0xC0) { // Начало многобайтовой последовательности
            bytes = (ch & 0xE0) == 0xC0 ? 1 : 2;
            wch = ch & (0xFF >> (bytes + 2));
        }
        else if (bytes) { // Продолжение многобайтовой последовательности
            wch = (wch << 6) | (ch & 0x3F);
            if (--bytes == 0) {
                result += wch;
            }
        }
    }
    return result;
}

std::wstring LoadPhrase(const std::string& phraseName) {
    std::ifstream file(fs::current_path() / "text" / filePathLang, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filePathLang);
    }

    std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    json data;
    try {
        data = json::parse(utf8Content);
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("Ошибка парсинга JSON: " + std::string(e.what()));
    }

    for (const auto& phrase : data["phrases"]) {
        if (phrase["name"] == phraseName) {
            std::string text = phrase["text"];
            std::wstring wstr = Utf8ToWstring(text);
            return wstr;
        }
    }
    throw std::runtime_error("Фраза с именем '" + phraseName + "' не найдена.");
}

void ClearTerminal() {
    screenFiles.clear();
    screenFilesCoord.clear();
    screenText.clear();
    screenTextCoord.clear();
    terminal_clear();
}

void DrawFrameFromFile(const std::string& filename, int x, int y) {
    fs::path path = fs::current_path() / "Interfaces" / filename;
    screenFiles.push_back(filename);
    screenFilesCoord.push_back(x);
    screenFilesCoord.push_back(y);

    std::ifstream file(path.string());
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << path.string() << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Преобразуем прочитанную строку из UTF-8 в std::wstring
        std::wstring wline = Utf8ToWstring(line);
        terminal_print(x, y++, wline.c_str());
        if (y >= terminal_state(TK_HEIGHT))
            break;
    }
    terminal_refresh();
}

void SetFontSize() {
    const int width = terminal_state(TK_WIDTH);
    const int height = terminal_state(TK_HEIGHT);

    int fontSize = sqrt(width * width + height * height) / 14;
    if (fontSize < 1) fontSize = 1;  // Минимальный размер шрифта
    const char* w = "font: fonts/UbuntuMono-Regular.ttf, size=";
    char result[128];

    snprintf(result, sizeof(result), "%s%d", w, fontSize);

    terminal_set(result);
    terminal_set("window.size=191x56");

    std::cout << result;
    std::cout << "\n";
    std::cout << fontSize;
    std::cout << "\n";
}

void DrawText(std::wstring text, int x, int y) {
    terminal_print(x, y, text.c_str());
    terminal_refresh();
    screenText.push_back(text); 
    screenTextCoord.push_back(x);
    screenTextCoord.push_back(y);
}

void LoadScreen(std::vector<std::string> screenF, std::vector<int> screenFC, std::vector<int> screenTC, std::vector<std::wstring> screenT) {
    ClearTerminal();

    for (int i = 0; i < screenF.size(); i++) {
        DrawFrameFromFile(screenF[i], screenFC[i * 2], screenFC[i * 2 + 1]);
    }
    for (int i = 0; i < screenT.size(); i++) {
        DrawText(screenT[i], screenTC[i * 2], screenTC[i * 2 + 1]);
    }
}

std::vector<std::wstring> SplitString(const std::wstring& input) {
    std::wistringstream stream(input); 
    std::vector<std::wstring> words; 
    std::wstring word;

    while (stream >> word) {
        words.push_back(word);
    }

    return words;
}


void If_Dialogue(const std::string& phraseName) {
    ClearTerminal();
    DrawFrameFromFile("dialogue.txt",0,0);
    
    const std::vector<std::wstring> phrase = SplitString(LoadPhrase(phraseName));
    std::wstring line;
    int k = 0;

    for (auto i : phrase) {
        if (line.size() + i.size() >= 138) {
            DrawText(line, 25, 20+k);
            k++;
            line = i + L" ";
        }
        else {
            line += i + L" ";
        }
    }

    if (line.size() != 0){
        DrawText(line, 26, 20 + k);
    }

    line = LoadPhrase("next");
    DrawText(line,95-line.size()/2, 50);

    int key;

    while (true) {

        key = terminal_read();

        if (key == TK_F11) {
            static bool fullscreen = false;
            fullscreen = !fullscreen;
            terminal_set(fullscreen ? "window.fullscreen=true" : "window.fullscreen=false");
            SetFontSize();
            LoadScreen(screenFiles, screenFilesCoord, screenTextCoord, screenText);
        }

        if (key == TK_ENTER or key == TK_SPACE) {
            break;
        }
    }
}

void OpenTerminal() {
    terminal_open();
    terminal_set("font: fonts/UbuntuMono-Regular.ttf, size=10, codepage=437");
    terminal_set("window.maximized=true");
    terminal_set("window.title='Toxic & Ventures'");
    terminal_set("window.icon='icons/icon.ico'");
    terminal_set("input.encoding = utf-8");
    terminal_set("window.encoding=437");
    //terminal_set("window.cursor.visible=false");

    DrawFrameFromFile("ramka.txt",0,0);
    SetFontSize();
    LoadScreen(screenFiles, screenFilesCoord, screenTextCoord, screenText);

    int key;

    while (true) {

        key = terminal_read();

        if (key == TK_CLOSE || key == TK_ESCAPE) {
            break;
        }

        if (key == TK_F11) {
            static bool fullscreen = false;
            fullscreen = !fullscreen;
            terminal_set(fullscreen ? "window.fullscreen=true" : "window.fullscreen=false");
            SetFontSize();
            LoadScreen(screenFiles, screenFilesCoord, screenTextCoord, screenText);
        }
    }

    terminal_close();
}