#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_

#include <SDL.h>
#include <vector>
#include <map>
#include <string>

class InputConfig;
class Window;

//you should only ever instantiate one of these, by the way
class InputManager
{
public:
	InputManager(Window* window);
	~InputManager();

	void loadConfig();
	void writeConfig();
	static std::string getConfigPath();

	void init();
	void deinit();

	void setNumPlayers(int num);
	int getNumPlayers();

	int getNumJoysticks();
	int getButtonCountByDevice(int id);

	bool parseEvent(const SDL_Event& ev);

	InputConfig* getInputConfigByPlayer(int player);

private:
	static const int DEADZONE = 23000;

	Window* mWindow;

	//non-InputManager classes shouldn't use this, as you can easily miss the keyboard
	InputConfig* getInputConfigByDevice(int device);

	void loadDefaultConfig();

	int mNumJoysticks;
	int mNumPlayers;
	SDL_Joystick** mJoysticks;
	InputConfig** mInputConfigs;
	InputConfig* mKeyboardInputConfig;
	std::map<int, int>* mPrevAxisValues;
};

#endif
