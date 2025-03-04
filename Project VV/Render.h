#include "BearLibTerminal.h"
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <chrono>
#include <filesystem>
#include <cmath>
#include <Windows.h>
#include <regex>

namespace fs = std::filesystem;
using json = nlohmann::json;

std::wstring operator*(const std::wstring& str, int n);

std::wstring LoadPhrase(const std::string& phraseName);

void ClearTerminal();

void DrawFrameFromFile(const std::string& filename, int x, int y, bool refresh = true, std::wstring replace = L"");

void SetFontSize();

void DrawText(std::wstring text, int x, int y);

void LoadScreen();

std::vector<std::wstring> SplitString(const std::wstring& input);

void BaseIfTerminal(int& key);
