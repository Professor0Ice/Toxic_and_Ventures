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

//����� ��� ����������� � �������
std::wstring Utf8ToWstring(const std::string& str) {
    std::wstring result;
    wchar_t wch = 0;
    int bytes = 0;

    for (unsigned char ch : str) {
        if (ch <= 0x7F) { // ASCII �������
            result += wchar_t(ch);
        }
        else if ((ch & 0xC0) == 0xC0) { // ������ ������������� ������������������
            bytes = (ch & 0xE0) == 0xC0 ? 1 : 2;
            wch = ch & (0xFF >> (bytes + 2));
        }
        else if (bytes) { // ����������� ������������� ������������������
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
        throw std::runtime_error("�� ������� ������� ����: " + filePathLang);
    }

    std::string utf8Content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    json data;
    try {
        data = json::parse(utf8Content);
    }
    catch (const json::parse_error& e) {
        throw std::runtime_error("������ �������� JSON: " + std::string(e.what()));
    }

    for (const auto& phrase : data["phrases"]) {
        if (phrase["name"] == phraseName) {
            std::string text = phrase["text"];
            std::wstring wstr = Utf8ToWstring(text);
            return wstr;
        }
    }
    std::cout << phraseName << std::endl;

 
    throw std::runtime_error("����� � ������ '" + phraseName + "' �� �������.");
}

int getRandomInt(int min, int max) {
    if (min > max) std::swap(min, max);

    std::random_device rd;
    std::seed_seq seed{ rd(), static_cast<unsigned>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count()) };
    std::mt19937 g(seed);

    std::uniform_int_distribution<int> dist(min, max);
    return dist(g);
}

void ClearTerminal() {
    screenFiles.clear();
    screenFilesCoord.clear();
    screenText.clear();
    screenTextCoord.clear();
    replaceText.clear();
    terminal_clear();
}

void DrawFrameFromFile(const std::string& filename, int x, int y, bool refresh, std::wstring replace, bool memory) {
    fs::path path = fs::current_path() / "Interfaces" / filename;
	if(memory){
		screenFiles.push_back(filename);
		screenFilesCoord.push_back(x);
		screenFilesCoord.push_back(y);
		replaceText.push_back(replace);
	}

    std::ifstream file(path.string());
    if (!file.is_open()) {
        std::cerr << "�� ������� ������� ����: " << path.string() << std::endl;
        return;
    }

    terminal_layer(0);
    std::string line;
    while (std::getline(file, line)) {
        // ����������� ����������� ������ �� UTF-8 � std::wstring
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

void SetFontSize(bool fullscreen) {
    const int cellCountX = 191;
    const int cellCountY = 56;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    if (screenWidth <= 0 || screenHeight <= 0) {
        std::cerr << "������: ������������ ������ ������!" << std::endl;
        return;
    }

    float scaleFactor = fullscreen ? 1.0f : 0.75f;  // 100% ��� 75% �� ������
    int windowWidth = static_cast<int>(screenWidth * scaleFactor);
    int windowHeight = static_cast<int>(screenHeight * scaleFactor);

    int cellWidth = max(5, windowWidth / cellCountX);
    int cellHeight = max(10, windowHeight / cellCountY);
    int fontSize = min(cellHeight, static_cast<int>(cellWidth * 1.7));

    std::string fontCmd = "font: fonts/UbuntuMono-Regular.ttf, size=" + std::to_string(fontSize);
    std::string cellSizeCmd = "window.cellsize=" + std::to_string(cellWidth) + "x" + std::to_string(cellHeight);
    std::string winSizeCmd = "window.size=" + std::to_string(cellCountX) + "x" + std::to_string(cellCountY);

    terminal_set(fontCmd.c_str());
    terminal_set(cellSizeCmd.c_str());
    terminal_set(winSizeCmd.c_str());  // ��������� ������ ����!

    std::cout << "[DEBUG] " << fontCmd << "\n";
    std::cout << "[DEBUG] " << cellSizeCmd << "\n";
    std::cout << "[DEBUG] " << winSizeCmd << "\n";
}

void DrawText(std::wstring text, int x, int y, bool Memory, bool refresh) {
	terminal_layer(1);
    terminal_print(x, y, text.c_str());
    if(refresh){terminal_refresh();}
	if(Memory){
		screenText.push_back(text);
		screenTextCoord.push_back(x);
		screenTextCoord.push_back(y);
	}
}

void LoadScreen() {
    std::vector<std::string> screenF = screenFiles;
    std::vector<int> screenFC = screenFilesCoord;
    std::vector<int> screenTC = screenTextCoord;
    std::vector<std::wstring> screenT = screenText;
    std::vector<std::wstring> replaceT = replaceText;

    ClearTerminal();

    for (int i = 0; i < screenF.size(); i++) {
        DrawFrameFromFile(screenF[i], screenFC[i * 2], screenFC[i * 2 + 1], false, replaceT[i]);
    }
    for (int i = 0; i < screenT.size(); i++) {
        DrawText(screenT[i], screenTC[i * 2], screenTC[i * 2 + 1], true,false);
    }
    terminal_refresh();
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

        terminal_set(L"window.size=191x56"); // ���������� ������
        terminal_refresh();
        LoadScreen(); // �������������� ���

        lastWidth = currentWidth;
        lastHeight = currentHeight;
    }
}

bool IsFullScreen() {
    int width = terminal_state(TK_WIDTH);      // ������ ����
    int height = terminal_state(TK_HEIGHT);    // ������ ����

    // ��������, ������� �����������, ���� ������� ������ 80x25
    return (width == GetSystemMetrics(SM_CXSCREEN) && height == GetSystemMetrics(SM_CYSCREEN));
}

void BaseIfTerminal(int& key) {

    if (key == TK_CLOSE or key == TK_ESCAPE) {
        terminal_read();
        terminal_close();
        exit(0);
    }
    else if (key == TK_F11) {
        terminal_read();
        fullscreen = !fullscreen;
        terminal_set(fullscreen ? "window.fullscreen=true" : "window.fullscreen=false");
        SetFontSize(fullscreen);
        LoadScreen();
    }

    auto now = std::chrono::steady_clock::now();
    static auto lastRefresh = now; 
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastRefresh).count();
    if (elapsed > 1000) {  
        LoadScreen(); 
        lastRefresh = now;
    }
}
