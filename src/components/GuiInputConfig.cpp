#include "GuiInputConfig.h"
#include "../Window.h"
#include "../Renderer.h"
#include "../Font.h"
#include "GuiGameList.h"
#include "../Log.h"

static int inputCount = 10;
static std::string inputName[10] = { "Up", "Down", "Left", "Right", "A", "B", "Menu", "Select", "PageUp", "PageDown" };
static std::string inputDispName[10] = { "Up", "Down", "Left", "Right", "Accept", "Back", "Menu", "Jump to Letter", "Page Up", "Page Down" };

GuiInputConfig::GuiInputConfig(Window* window, InputConfig* target) : Gui(window), mTargetConfig(target)
{
	mCurInputId = 0;
	LOG(LogInfo) << "Configuring device " << target->getDeviceId();
}

void GuiInputConfig::update(int deltaTime)
{

}

void GuiInputConfig::input(InputConfig* config, Input input)
{
	if(config != mTargetConfig || input.value == 0)
		return;

	if(mCurInputId >= inputCount)
	{
		//done
		if(input.type == TYPE_BUTTON || input.type == TYPE_KEY)
		{
			if(mTargetConfig->getPlayerNum() < mWindow->getInputManager()->getNumPlayers() - 1)
			{
				mWindow->pushGui(new GuiInputConfig(mWindow, mWindow->getInputManager()->getInputConfigByPlayer(mTargetConfig->getPlayerNum() + 1)));
			}else{
				mWindow->getInputManager()->writeConfig();
				GuiGameList::create(mWindow);
			}
			delete this;
		}
	}else{
		if(config->getMappedTo(input).size() > 0)
		{
			mErrorMsg = "Already mapped!";
			return;
		}

		input.configured = true;
		LOG(LogInfo) << "  [" << input.string() << "] -> " << inputName[mCurInputId];

		config->mapInput(inputName[mCurInputId], input);
		mCurInputId++;
		mErrorMsg = "";

		//if the controller doesn't have enough buttons for Page Up/Page Down, skip to done
		if(mWindow->getInputManager()->getButtonCountByDevice(config->getDeviceId()) <= 4 && mCurInputId >= 8)
		{
			mCurInputId = inputCount;
		}
	}
}

void GuiInputConfig::render()
{
	Font* font = Renderer::getDefaultFont(Renderer::MEDIUM);

	std::stringstream stream;
	stream << "PLAYER " << mTargetConfig->getPlayerNum() + 1 << ", press...";
	Renderer::drawText(stream.str(), 10, 10, 0x000000FF, font);

	int y = 14 + font->getHeight();
	for(int i = 0; i < mCurInputId; i++)
	{
		Renderer::drawText(inputDispName[i], 10, y, 0x00CC00FF, font);
		y += font->getHeight() + 5;
	}

	if(mCurInputId >= inputCount)
	{
		Renderer::drawCenteredText("Basic config done!", 0, (int)(Renderer::getScreenHeight() * 0.6), 0x00CC00FF, font);
		Renderer::drawCenteredText("Press any button to continue.", 0, (int)(Renderer::getScreenHeight() * 0.6) + font->getHeight() + 4, 0x000000FF, font);
	}else{
		Renderer::drawText(inputDispName[mCurInputId], 10, y, 0x000000FF, font);
	}

	if(!mErrorMsg.empty())
		Renderer::drawCenteredText(mErrorMsg, 0, Renderer::getScreenHeight() - font->getHeight() - 10, 0xFF0000FF, font);
}
