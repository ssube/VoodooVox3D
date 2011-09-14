#pragma once

#ifndef IMPORT_RENDERENGINE
#   define RENDERENGINE_API __declspec(dllexport)
#else
#   define RENDERENGINE_API __declspec(dllimport)
#endif

#define OBJECT_TYPE_RENDERENGINE 0x10
#define OBJECT_TYPE_RENDEROBJECT 0x11
#define OBJECT_TYPE_INPUTMANAGER 0x12
#define OBJECT_TYPE_CAMERA       0x13

class Camera;
class InputManager;
class RenderEngine;
class RenderObject;
