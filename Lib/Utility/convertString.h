#pragma once
#include <string>
#include <format>
#include <Windows.h>
#include <vector>
//string → wstring
inline std::wstring ConvertString(const std::string& str);

//wstring → string
inline std::string ConvertString(const std::wstring& str);

//string → char
inline const char ConvertStringC(const std::string& str);

//char → string
inline std::string ConvertStringC(const char& c);


inline void Log(const std::string& message);