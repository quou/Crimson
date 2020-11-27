# Core

## Logger

Logging functions will print to the editor's console, and to the external text console if it is available. Note that under some Windows environments, colours may not work.

```cpp
Console::Log("Test Message");
Console::LogWarning("Test Warning");
Console::LogError("Test Error");
Console::LogFatalError("Test Fatal Error");
```

## Input

Input buttons are defined in the `InputConfig.conf` file.
```cpp
struct Input::Button {
	bool pressed;
};

const Input::Button& Input::GetButton(const string &in name);

// Only recommend using this function where absolutely necessary. Use InputConfig.conf instead. 
void Input::RegisterButton(const string &in name, int keycode);

vec2 Input::GetMouseChange();
vec2 Input::GetMousePos();
vec2 Input::GetScrollDelta();
```
