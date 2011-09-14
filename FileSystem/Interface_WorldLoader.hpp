#pragma once

#ifndef IMPORT_WORLDLOADER
#    define WORLDLOADER_API __declspec(dllexport)
#else
#    define WORLDLOADER_API __declspec(dllimport)
#endif

class WorldLoader;