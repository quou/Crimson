#pragma once

#include <string>
#include <map>
#include <glm/glm.hpp>

struct GLFWwindow;

#define CR_KEY_UNKNOWN            -1

#define CR_KEY_SPACE              32
#define CR_KEY_APOSTROPHE         39  /* ' */
#define CR_KEY_COMMA              44  /* , */
#define CR_KEY_MINUS              45  /* - */
#define CR_KEY_PERIOD             46  /* . */
#define CR_KEY_SLASH              47  /* / */
#define CR_KEY_0                  48
#define CR_KEY_1                  49
#define CR_KEY_2                  50
#define CR_KEY_3                  51
#define CR_KEY_4                  52
#define CR_KEY_5                  53
#define CR_KEY_6                  54
#define CR_KEY_7                  55
#define CR_KEY_8                  56
#define CR_KEY_9                  57
#define CR_KEY_SEMICOLON          59  /* ; */
#define CR_KEY_EQUAL              61  /* = */
#define CR_KEY_A                  65
#define CR_KEY_B                  66
#define CR_KEY_C                  67
#define CR_KEY_D                  68
#define CR_KEY_E                  69
#define CR_KEY_F                  70
#define CR_KEY_G                  71
#define CR_KEY_H                  72
#define CR_KEY_I                  73
#define CR_KEY_J                  74
#define CR_KEY_K                  75
#define CR_KEY_L                  76
#define CR_KEY_M                  77
#define CR_KEY_N                  78
#define CR_KEY_O                  79
#define CR_KEY_P                  80
#define CR_KEY_Q                  81
#define CR_KEY_R                  82
#define CR_KEY_S                  83
#define CR_KEY_T                  84
#define CR_KEY_U                  85
#define CR_KEY_V                  86
#define CR_KEY_W                  87
#define CR_KEY_X                  88
#define CR_KEY_Y                  89
#define CR_KEY_Z                  90
#define CR_KEY_LEFT_BRACKET       91  /* [ */
#define CR_KEY_BACKSLASH          92  /* \ */
#define CR_KEY_RIGHT_BRACKET      93  /* ] */
#define CR_KEY_GRAVE_ACCENT       96  /* ` */
#define CR_KEY_WORLD_1            161 /* non-US #1 */
#define CR_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define CR_KEY_ESCAPE             256
#define CR_KEY_ENTER              257
#define CR_KEY_TAB                258
#define CR_KEY_BACKSPACE          259
#define CR_KEY_INSERT             260
#define CR_KEY_DELETE             261
#define CR_KEY_RIGHT              262
#define CR_KEY_LEFT               263
#define CR_KEY_DOWN               264
#define CR_KEY_UP                 265
#define CR_KEY_PAGE_UP            266
#define CR_KEY_PAGE_DOWN          267
#define CR_KEY_HOME               268
#define CR_KEY_END                269
#define CR_KEY_CAPS_LOCK          280
#define CR_KEY_SCROLL_LOCK        281
#define CR_KEY_NUM_LOCK           282
#define CR_KEY_PRINT_SCREEN       283
#define CR_KEY_PAUSE              284
#define CR_KEY_F1                 290
#define CR_KEY_F2                 291
#define CR_KEY_F3                 292
#define CR_KEY_F4                 293
#define CR_KEY_F5                 294
#define CR_KEY_F6                 295
#define CR_KEY_F7                 296
#define CR_KEY_F8                 297
#define CR_KEY_F9                 298
#define CR_KEY_F10                299
#define CR_KEY_F11                300
#define CR_KEY_F12                301
#define CR_KEY_F13                302
#define CR_KEY_F14                303
#define CR_KEY_F15                304
#define CR_KEY_F16                305
#define CR_KEY_F17                306
#define CR_KEY_F18                307
#define CR_KEY_F19                308
#define CR_KEY_F20                309
#define CR_KEY_F21                310
#define CR_KEY_F22                311
#define CR_KEY_F23                312
#define CR_KEY_F24                313
#define CR_KEY_F25                314
#define CR_KEY_KP_0               320
#define CR_KEY_KP_1               321
#define CR_KEY_KP_2               322
#define CR_KEY_KP_3               323
#define CR_KEY_KP_4               324
#define CR_KEY_KP_5               325
#define CR_KEY_KP_6               326
#define CR_KEY_KP_7               327
#define CR_KEY_KP_8               328
#define CR_KEY_KP_9               329
#define CR_KEY_KP_DECIMAL         330
#define CR_KEY_KP_DIVIDE          331
#define CR_KEY_KP_MULTIPLY        332
#define CR_KEY_KP_SUBTRACT        333
#define CR_KEY_KP_ADD             334
#define CR_KEY_KP_ENTER           335
#define CR_KEY_KP_EQUAL           336
#define CR_KEY_LEFT_SHIFT         340
#define CR_KEY_LEFT_CONTROL       341
#define CR_KEY_LEFT_ALT           342
#define CR_KEY_LEFT_SUPER         343
#define CR_KEY_RIGHT_SHIFT        344
#define CR_KEY_RIGHT_CONTROL      345
#define CR_KEY_RIGHT_ALT          346
#define CR_KEY_RIGHT_SUPER        347
#define CR_KEY_MENU               348

#define CR_KEY_LAST               CR_KEY_MENU

#define CR_MOD_SHIFT           0x0001

#define CR_MOD_CONTROL         0x0002

#define CR_MOD_ALT             0x0004

#define CR_MOD_SUPER           0x0008

#define CR_MOD_CAPS_LOCK       0x0010

#define CR_MOD_NUM_LOCK        0x0020

#define CR_MOUSE_BUTTON_1         0
#define CR_MOUSE_BUTTON_2         1
#define CR_MOUSE_BUTTON_3         2
#define CR_MOUSE_BUTTON_4         3
#define CR_MOUSE_BUTTON_5         4
#define CR_MOUSE_BUTTON_6         5
#define CR_MOUSE_BUTTON_7         6
#define CR_MOUSE_BUTTON_8         7
#define CR_MOUSE_BUTTON_LAST      CR_MOUSE_BUTTON_8
#define CR_MOUSE_BUTTON_LEFT      CR_MOUSE_BUTTON_1
#define CR_MOUSE_BUTTON_RIGHT     CR_MOUSE_BUTTON_2
#define CR_MOUSE_BUTTON_MIDDLE    CR_MOUSE_BUTTON_3


#define CR_JOYSTICK_1             0
#define CR_JOYSTICK_2             1
#define CR_JOYSTICK_3             2
#define CR_JOYSTICK_4             3
#define CR_JOYSTICK_5             4
#define CR_JOYSTICK_6             5
#define CR_JOYSTICK_7             6
#define CR_JOYSTICK_8             7
#define CR_JOYSTICK_9             8
#define CR_JOYSTICK_10            9
#define CR_JOYSTICK_11            10
#define CR_JOYSTICK_12            11
#define CR_JOYSTICK_13            12
#define CR_JOYSTICK_14            13
#define CR_JOYSTICK_15            14
#define CR_JOYSTICK_16            15
#define CR_JOYSTICK_LAST          CR_JOYSTICK_16


#define CR_GAMEPAD_BUTTON_A               0
#define CR_GAMEPAD_BUTTON_B               1
#define CR_GAMEPAD_BUTTON_X               2
#define CR_GAMEPAD_BUTTON_Y               3
#define CR_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define CR_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define CR_GAMEPAD_BUTTON_BACK            6
#define CR_GAMEPAD_BUTTON_START           7
#define CR_GAMEPAD_BUTTON_GUIDE           8
#define CR_GAMEPAD_BUTTON_LEFT_THUMB      9
#define CR_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define CR_GAMEPAD_BUTTON_DPAD_UP         11
#define CR_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define CR_GAMEPAD_BUTTON_DPAD_DOWN       13
#define CR_GAMEPAD_BUTTON_DPAD_LEFT       14
#define CR_GAMEPAD_BUTTON_LAST            CR_GAMEPAD_BUTTON_DPAD_LEFT

#define CR_GAMEPAD_BUTTON_CROSS       CR_GAMEPAD_BUTTON_A
#define CR_GAMEPAD_BUTTON_CIRCLE      CR_GAMEPAD_BUTTON_B
#define CR_GAMEPAD_BUTTON_SQUARE      CR_GAMEPAD_BUTTON_X
#define CR_GAMEPAD_BUTTON_TRIANGLE    CR_GAMEPAD_BUTTON_Y


#define CR_GAMEPAD_AXIS_LEFT_X        0
#define CR_GAMEPAD_AXIS_LEFT_Y        1
#define CR_GAMEPAD_AXIS_RIGHT_X       2
#define CR_GAMEPAD_AXIS_RIGHT_Y       3
#define CR_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define CR_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define CR_GAMEPAD_AXIS_LAST          CR_GAMEPAD_AXIS_RIGHT_TRIGGER


namespace Crimson {

	struct Key {
		int keycode;
		int action;
		bool pressed;
	};

	class Input {
	private:
		std::map<std::string, Key> m_keys;

		glm::vec2 oldMousePos{0.0f, 0.0f};
		glm::vec2 mouseChange{0.0f, 0.0f};
		glm::vec2 mouseDelta{0.0f, 0.0f};
		glm::vec2 mousePos;
		bool firstInput{true};
	public:
		static Input& instance() {
      	static Input INSTANCE;
      	return INSTANCE;
   	}

		static void Init();

		static void IKeyCallback(int key, int scancode, int action, int mods);
		static void IMouseButtonCallback(int button, int action, int mods);
		static void IMouseMovementCallback(double x, double y);
		static void IScrollCallback(double x, double y);

		static void IEndFrame();

		static void RegisterKey(const std::string& name, int keycode);

		static const Key& GetKey(const std::string& name);

		static void LoadConfig(const char* lua);
		static void SaveConfig(const char* filePath);

		static void SetCursorPosition(GLFWwindow* window, glm::vec2 newPos);

		static glm::vec2 GetMouseChange();
		static glm::vec2 GetScrollDelta();

		inline static glm::vec2 GetMousePos() {return instance().mousePos;}
	};
}
