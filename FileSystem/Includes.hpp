
#include <string>
#include <sqlite3.h>

#ifndef FILEMANAGER_IMPORT
#	define FILEMANAGER_API __declspec(dllexport)
#else
#	define FILEMANAGER_API __declspec(dllimport)
#endif

#define COMMON_IMPORT
#include "CommonTypes.hpp"
