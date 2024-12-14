#pragma once

#include <list>
#include <vector>
#include <string>

#include <Windows.h>
/*
#include <mmsystem.h>
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")
*/

class MCIChannel
{
public:
	MCIChannel() : CurrentSound("NULLSOUND") {}
	~MCIChannel() { stop(); }

	void play(std::string _variableName);
	void stop();
	void volume(int level);
private:
	std::string CurrentSound;

};


void MCIChannel::play(std::string _variableName)
{
	std::string fullCommand = "";
	stop();
	if (_variableName == "NULLSOUND")
	{
		CurrentSound = "NULLSOUND";
		return;
	}
	std::string fullCommand = "play " + _variableName;
	MCIERROR mciErr = mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
	if (mciErr == 0) CurrentSound = _variableName;
}

void MCIChannel::stop()
{
	if (CurrentSound == "NULLSOUND") return;

	std::string fullCommand = "stop " + CurrentSound;
	mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
	CurrentSound = "NULLSOUND";
}

void MCIChannel::volume(int level)
{
	std::string fullCommand = "setaudio " + CurrentSound + " volume to ";
	if (level <= 0)
	{
		fullCommand += "0";
	}
	else { fullCommand += level; }
	mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
}



class MCISoundManager
{
public:
	MCISoundManager() {}
	~MCISoundManager() { release(); }

	void stopAllSound();
	void loadSound(std::string _Path, std::string _variableName);
	void release();
private:
	std::list<MCIChannel*> channelList;

};


void MCISoundManager::stopAllSound()
{
	if (channelList.empty()) return;

	for (auto& sound : channelList)
	{
		sound->stop();
	}

}

void MCISoundManager::loadSound(std::string _Path, std::string _variableName)
{
	std::string fullCommand = "open \"" + _Path + "\" type mpegvideo alias " + _variableName;
	MCIERROR mciErr = mciSendStringA(fullCommand.c_str(), NULL, 0, NULL);
	//if (mciErr != 0) MessageBox();
}

void MCISoundManager::release()
{
	if (channelList.empty()) return;
	for (auto& sound : channelList)
	{
		delete sound;
	}
	channelList.clear();
}

//x64
#include "D:\PJ\C++\FMods\x64\fmod.h"
#include "D:\PJ\C++\FMods\x64\fmod.hpp"
#pragma comment (lib, "D:\PJ\C++\FMods\x64\fmodL_vc.lib")

#include <map>


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	static void instantiate();
	static void destroy();

	void init();
	void update();
	void release();

	void loadSound(std::string path, std::string fileName);
	void playBGM(std::string soundName);
	FMOD::Channel* playNew(std::string soundName);
	void stopSound(FMOD::Channel* channel);
	bool checkPlaying(FMOD::Channel* channel);
	void changeVolume(FMOD::Channel* channel, float value);


private:
	FMOD::System* fmodSys;
	std::map<std::string, FMOD::Sound*> sounds;
	FMOD::Channel* BGMchannel;

};


SoundManager* Sound;



SoundManager::SoundManager() : sounds{}, fmodSys(nullptr), BGMchannel(nullptr)
{
}

SoundManager::~SoundManager()
{

}

void SoundManager::instantiate()
{
	if (Sound) return;

	Sound = new SoundManager();
	Sound->init();
}

void SoundManager::init()
{
	FMOD::System_Create(&fmodSys);
	// 최대 32채널 사용
	fmodSys->init(32, FMOD_INIT_NORMAL, 0);
	fmodSys->update();
}

void SoundManager::update()
{
	fmodSys->update();
}

void SoundManager::release()
{
	if (BGMchannel)
		stopSound(BGMchannel);

	for (auto& sound : sounds)
	{
		sound.second->release();
	}
	sounds.clear();

	fmodSys->close();
	fmodSys->release();
}

void SoundManager::loadSound(std::string path, std::string fileName)
{
	FMOD::Sound* newSound = nullptr;
	fmodSys->createSound((path + fileName).c_str(), FMOD_DEFAULT, 0, &newSound);
	sounds[fileName] = newSound;
}

void SoundManager::playBGM(std::string soundName)
{
	if (checkPlaying(BGMchannel))
		BGMchannel->stop();

	auto sound = sounds.find(soundName);
	if (sound == sounds.end()) return;

	fmodSys->playSound(sound->second, 0, false, &BGMchannel);
	BGMchannel->setMode(FMOD_LOOP_NORMAL);
	BGMchannel->setLoopCount(-1);
}

FMOD::Channel* SoundManager::playNew(std::string soundName)
{
	FMOD::Channel* newChannel = nullptr;

	auto sound = sounds.find(soundName);
	if (sound == sounds.end()) return nullptr;

	fmodSys->playSound(sound->second, 0, false, &BGMchannel);
	return newChannel;
}

void SoundManager::stopSound(FMOD::Channel* channel)
{
	if (!checkPlaying(channel)) return;
	channel->stop();
}

bool SoundManager::checkPlaying(FMOD::Channel* channel)
{
	bool playing = false;
	if (channel->isPlaying(&playing) != FMOD_OK) return false;
	return playing;
}

void SoundManager::changeVolume(FMOD::Channel* channel, float value)
{
	if (value > 1.0f) value = 1.0f;
	else if (value < 0.0f) value = 0.0f;
	channel->setVolume(value);
}

void SoundManager::destroy()
{
	Sound->release();
	delete Sound;
	Sound = nullptr;
}

//x86
// 이건 윈API는 직접적으로 쓰진 않지만
// 외부 api라 추가로 깔아야됨
//#include "D:\PJ\C++\FMods\fmod.h"
//#include "D:\PJ\C++\FMods\fmod.hpp"
//#pragma comment(lib, "fmodex_vc.lib")
//
//class SoundManagerx86
//{
//public:
//	SoundManagerx86() : fmodSys() { FMOD_System_Create(&fmodSys); }
//	~SoundManagerx86() { release(); }
//
//	void init();
//	void update() { FMOD_System_Update(fmodSys); }
//	FMOD_SOUND* loadSound(std::string filePath);
//	void play(FMOD_SOUND* sound, FMOD_CHANNEL* channel);
//	void pause(FMOD_CHANNEL* channel, bool pause) { FMOD_Channel_SetPaused(channel, pause); }
//	void stop(FMOD_CHANNEL* channel);
//	void release();
//
//private:
//	std::list<FMOD_SOUND*> soundList;
//	FMOD_CHANNEL* bgmChannel;
//	std::vector<FMOD_CHANNEL*> channelList;
//	FMOD_SYSTEM* fmodSys;
//
//};
//
//void SoundManagerx86::init()
//{
//	FMOD_System_Init(fmodSys, 32, FMOD_INIT_NORMAL, NULL);
//}
//
//FMOD_SOUND* SoundManagerx86::loadSound(std::string filePath)
//{
//	FMOD_SOUND* newSound;
//	FMOD_System_CreateSound(fmodSys, filePath.c_str(), FMOD_DEFAULT, 0, &newSound);
//	return newSound;
//}
//
//void SoundManagerx86::play(FMOD_SOUND* sound, FMOD_CHANNEL* channel)
//{
//	stop(channel);
//	FMOD_System_PlaySound(fmodSys, FMOD_CHANNEL_FREE, sound, false, &channel);
//}
//
//void SoundManagerx86::stop(FMOD_CHANNEL* channel)
//{
//	int condition;
//	FMOD_RESULT result = FMOD_Channel_IsPlaying(channel, &condition);
//
//	if (result != FMOD_OK) return;
//	if (condition) FMOD_Channel_Stop(channel);
//}
//
//void SoundManagerx86::release()
//{
//	if (!channelList.empty())
//	{
//		for (auto& channel : channelList)
//		{
//			stop(channel);
//		}
//	}
//
//	if (!soundList.empty())
//	{
//		for (auto& sound : soundList)
//		{
//			FMOD_Sound_Release(sound);
//		}
//		soundList.clear();
//	}
//	FMOD_System_Release(fmodSys);
//	FMOD_System_Close(fmodSys);
//}

