#pragma once

#include <string>

std::string LoadFromFile(const std::string fileName);

#ifdef _DEBUG
#define LOG(x,y) LogError(x,y)
#define LOGGL(x) LogErrorGL(x)
#else
#define LOG(x,y)
#define LOGGL(x)
#endif

int UtilInt();

void LogError(std::string prefix, std::string message);

void LogErrorGL(std::string prefix);