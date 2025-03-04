#include "Render.h"


std::vector<std::string> screenFiles = {};
std::vector<int> screenFilesCoord = {};
std::vector<std::wstring> screenText = {};
std::vector<int> screenTextCoord = {};
std::vector<std::wstring> replaceText = {};

extern std::string filePathLang = "ru.json";

std::wregex pattern(L"J");

static bool fullscreen = false;

std::wstring operator*(const std::wstring& str, int n) {
	if (n <= 0) {
		return L""; 
	}

	std::wstring result;
	result.reserve(str.size() * n); 

	for (int i = 0; i < n; ++i) {
		result += str; 
	}

	return result;
}

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
    std::cout << phraseName << std::endl;
    throw std::runtime_error("Фраза с именем '" + phraseName + "' не найдена.");
}

void ClearTerminal() {
    screenFiles.clear();
    screenFilesCoord.clear();
    screenText.clear();
    screenTextCoord.clear();
    replaceText.clear();
    terminal_clear();
}

void DrawFrameFromFile(const std::string& filename, int x, int y, bool refresh, std::wstring replace) {
    fs::path path = fs::current_path() / "Interfaces" / filename;
    screenFiles.push_back(filename);
    screenFilesCoord.push_back(x);
    screenFilesCoord.push_back(y);
    replaceText.push_back(replace);

    std::ifstream file(path.string());
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << path.string() << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Преобразуем прочитанную строку из UTF-8 в std::wstring
        std::wstring wline = Utf8ToWstring(line);

        if (replace.size() > 0) {
            wline = std::regex_replace(wline, pattern, replace);
        }

        terminal_print(x, y++, wline.c_str());
        if (y >= terminal_state(TK_HEIGHT))
            break;
    }
    if (refresh) { terminal_refresh(); }
}

void SetFontSize() {
    const int width = terminal_state(TK_WIDTH);
    const int height = terminal_state(TK_HEIGHT);

    int fontSize = sqrt(width * width + height * height) / 14;
    if (fontSize < 2) fontSize = 2;  // Минимальный размер шрифта   WSC 
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

void LoadScreen() {
    std::vector<std::string> screenF = screenFiles;
    std::vector<int> screenFC = screenFilesCoord;
    std::vector<int> screenTC = screenTextCoord;
    std::vector<std::wstring> screenT = screenText;
    std::vector<std::wstring> replaceT = replaceText;

    ClearTerminal();

    for (int i = 0; i < screenF.size(); i++) {
        DrawFrameFromFile(screenF[i], screenFC[i * 2], screenFC[i * 2 + 1], true, replaceT[i]);
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

void CheckResize() {
    static int lastWidth = terminal_state(TK_WIDTH);
    static int lastHeight = terminal_state(TK_HEIGHT);

    int currentWidth = terminal_state(TK_WIDTH);
    int currentHeight = terminal_state(TK_HEIGHT);

    if (currentWidth != lastWidth || currentHeight != lastHeight) {
        std::cout << "Resize detected!" << std::endl;

        terminal_set(L"window.size=191x56"); // Возвращаем размер
        terminal_refresh();
        LoadScreen(); // Перерисовываем арт

        lastWidth = currentWidth;
        lastHeight = currentHeight;
    }
}

bool IsFullScreen() {
    int width = terminal_state(TK_WIDTH);      // Ширина окна
    int height = terminal_state(TK_HEIGHT);    // Высота окна

    // Например, считаем фуллскрином, если размеры больше 80x25
    return (width == GetSystemMetrics(SM_CXSCREEN) && height == GetSystemMetrics(SM_CYSCREEN));
}

void BaseIfTerminal(int& key) {

    if (key == TK_CLOSE or key == TK_ESCAPE) {
        terminal_close();
        exit(0);
    }
    else if (key == TK_F11) {
        fullscreen = !fullscreen;
        terminal_set(fullscreen ? "window.fullscreen=true" : "window.fullscreen=false");
        if (!fullscreen) {
            terminal_set("font: fonts/UbuntuMono-Regular.ttf, size=9");
			terminal_set("window.size=191x56");
            LoadScreen();
        }
        else {
            SetFontSize();
            LoadScreen();
        }
    }
    else if (key == TK_RESIZED) {
        SetFontSize();
        LoadScreen();
    }  
}
