#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <ShlObj_core.h>
#include <filesystem>

using namespace geode::prelude;

std::string songName;
int songID;
bool robSong;

void deleteSongs(std::filesystem::path folderPath, std::filesystem::path songLocation, int songID, bool isRobtopSong) {
	auto gdFolderPath = std::filesystem::current_path();
	if (!isRobtopSong) {
		DeleteFile(fmt::format("{}\\{}copy.mp3",folderPath, songID).c_str());
		DeleteFile(fmt::format("{}\\{}.wav",folderPath, songID).c_str());
		DeleteFile(fmt::format("{}\\{}wav.wav",folderPath, songID).c_str());
		std::filesystem::copy(fmt::format("{}\\{}.mp3", folderPath, songID).c_str(), fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), std::filesystem::copy_options::update_existing);
		DeleteFile(fmt::format("{}\\{}.mp3",folderPath, songID).c_str());
		FLAlertLayer::create("Pitch Shifter", "<cg>Your song has been pitched!</c>", "OK")->show();
	}
	else {
		DeleteFile(fmt::format("{}\\{}copy.mp3",folderPath, songName).c_str());
		DeleteFile(fmt::format("{}\\{}.wav",folderPath, songName).c_str());
		DeleteFile(fmt::format("{}\\{}wav.wav",folderPath, songName).c_str());
		std::filesystem::copy(fmt::format("{}\\{}.mp3", folderPath, songName).c_str(), fmt::format("{}\\{}.mp3", songLocation, songName).c_str(), std::filesystem::copy_options::update_existing);
		DeleteFile(fmt::format("{}\\{}.mp3",folderPath, songName).c_str());
		FLAlertLayer::create("Pitch Shifter", "<cg>Your song has been pitched!</c>", "OK")->show();
	}
}

void pitchSong(int songID, int pitchShift, bool isRobtopSong) {
	PWSTR appdata = NULL;
	std::string songLocation;
	if (SHGetKnownFolderPath(FOLDERID_LocalAppData, KF_FLAG_CREATE, NULL, &appdata) == S_OK) {
		char dest[MAX_PATH];
		wcstombs(dest, appdata, MAX_PATH);
		songLocation = dest;
		songLocation += "\\GeometryDash";
	}
	else {
		log::info("error getting appdata path");
	}
	auto gdFolderPath = std::filesystem::current_path();
	auto folderPath = gdFolderPath;
	folderPath += "\\geode\\unzipped\\xanii.pitch_shifter\\resources\\xanii.pitch_shifter";
	auto robtopSongLoc = gdFolderPath += "\\Resources";
	switch(songID) {
		case 0: songName = "StereoMadness"; break;
		case 1: songName = "BackOnTrack"; break;
		case 2: songName = "Polargeist"; break;
		case 3: songName = "DryOut"; break;
		case 4: songName = "BaseAfterBase"; break;
		case 5: songName = "CantLetGo"; break;
		case 6: songName = "Jumper"; break;
		case 7: songName = "TimeMachine"; break;
		case 8: songName = "Cycles"; break;
		case 9: songName = "xStep"; break;
		case 10: songName = "Clutterfunk"; break;
		case 11: songName = "TheoryOfEverything"; break;
		case 12: songName = "Electroman"; break;
		case 13: songName = "Clubstep"; break;
		case 14: songName = "Electrodynamix"; break;
		case 15: songName = "HexagonForce"; break;
		case 16: songName = "BlastProcessing"; break;
		case 17: songName = "TheoryOfEverything2"; break;
		case 18: songName = "GeometricalDominator"; break;
		case 19: songName = "Deadlocked"; break;
		case 20: songName = "Fingerdash"; break;
		case 21: songName = "Dash"; break;
	}
	log::info("{} {}", songName, songID);
	if (appdata != NULL && pitchShift != 0) {
		if (!isRobtopSong) {
			if (std::filesystem::exists(fmt::format("{}\\{}.mp3", songLocation, songID))) {
				if (!std::filesystem::exists(fmt::format("{}\\pitch_backups", gdFolderPath).c_str())) CreateDirectoryA(fmt::format("{}\\pitch_backups", gdFolderPath).c_str(), NULL);
				CopyFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), fmt::format("{}\\{}copy.mp3", folderPath, songID).c_str(), true);
				CopyFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songID).c_str(), true);
				std::string convertWav = fmt::format("ffmpeg -y -i {}copy.mp3 {}wav.wav -loglevel quiet", songID, songID);
				std::string shiftWav = fmt::format("sox --no-show-progress {}wav.wav {}.wav pitch {}", songID, songID, pitchShift);
				std::string convertMp3 = fmt::format("ffmpeg -y -i {}.wav -acodec libmp3lame {}.mp3 -loglevel quiet", songID, songID);
				system(fmt::format("cd {} && {} && {} && {}", folderPath, convertWav, shiftWav, convertMp3).c_str());
				deleteSongs(folderPath, songLocation, songID, isRobtopSong);
			}
			else log::debug("Original Song Location not found.");
		}
		else {
			if (std::filesystem::exists(fmt::format("{}\\{}.mp3", robtopSongLoc, songName))) {
				if (!std::filesystem::exists(fmt::format("{}\\pitch_backups", gdFolderPath).c_str())) CreateDirectoryA(fmt::format("{}\\pitch_backups", gdFolderPath).c_str(), NULL);
				CopyFile(fmt::format("{}\\{}.mp3", robtopSongLoc, songName).c_str(), fmt::format("{}\\{}copy.mp3", folderPath, songName).c_str(), true);
				CopyFile(fmt::format("{}\\{}.mp3", robtopSongLoc, songName).c_str(), fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songName).c_str(), true);
				std::string convertWav = fmt::format("ffmpeg -y -i {}copy.mp3 {}wav.wav -loglevel quiet", songName, songName);
				std::string shiftWav = fmt::format("sox --no-show-progress {}wav.wav {}.wav pitch {}", songName, songName, pitchShift);
				std::string convertMp3 = fmt::format("ffmpeg -y -i {}.wav -acodec libmp3lame {}.mp3 -loglevel quiet", songName, songName);
				system(fmt::format("cd {} && {} && {} && {}", folderPath, convertWav, shiftWav, convertMp3).c_str());
				deleteSongs(folderPath, robtopSongLoc, songID, isRobtopSong);
			}
			else log::debug("Original Song Location not found.");
		}
	}
	else if (appdata != NULL && pitchShift == 0) {
		if (!isRobtopSong) {
			std::string resourcesFolder = gdFolderPath.string();
			std::string oldPitchFolder = resourcesFolder.substr(0, resourcesFolder.length() - 9);
			if (std::filesystem::exists(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songID))) {
				DeleteFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str());
				std::filesystem::copy(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songID).c_str(), fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), std::filesystem::copy_options::update_existing);
				FLAlertLayer::create("Pitch Shifter", fmt::format("{}.mp3 has been reset back to its normal pitch!", songID), "OK")->show();
			}
			else if (std::filesystem::exists(fmt::format("{}\\pitch_backups\\{}.mp3", oldPitchFolder, songID))) {
				DeleteFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str());
				std::filesystem::copy(fmt::format("{}\\pitch_backups\\{}.mp3", oldPitchFolder, songID).c_str(), fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), std::filesystem::copy_options::update_existing);
				FLAlertLayer::create("Pitch Shifter", fmt::format("{}.mp3 has been reset back to its normal pitch!", songID), "OK")->show();
			}
			else FLAlertLayer::create("Error Occured", "You must pitch a song before you can reset it!\n<cr>(You set pitch value to 0)</c>", "OK")->show();
		}
		else{
		if (std::filesystem::exists(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songName))) {
			DeleteFile(fmt::format("{}\\{}.mp3", gdFolderPath, songName).c_str());
			std::filesystem::copy(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songName).c_str(), fmt::format("{}\\{}.mp3", gdFolderPath, songName).c_str(), std::filesystem::copy_options::update_existing);
			FLAlertLayer::create("Pitch Shifter", fmt::format("{}.mp3 has been reset back to its normal pitch!", songName), "OK")->show();
			}
			else FLAlertLayer::create("Error Occured", "You must pitch a song before you can reset it!\n<cr>(You set pitch value to 0)</c>", "OK")->show();
		}
	}
}


class $modify(CustomSongWidget) {
	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary, int unk) {
		bool result = CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary, unk);
		if (!CCDirector::sharedDirector()->getRunningScene()->getChildByIDRecursive("LevelEditorLayer")) songID = m_songInfoObject->m_songID;
		return result;
	}
};

class $modify(PitchLayer, LevelInfoLayer) {

	bool init(GJGameLevel* level, bool isRobTopLevel) {
		if (!LevelInfoLayer::init(level, isRobTopLevel)) return false;
			CCSprite* pitchBackgroundSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn02_001.png");
			CCMenu* backMenu = static_cast<CCMenu*>(this->getChildByIDRecursive("back-menu"));
			CCMenu* pitchMenu = CCMenu::create();
			CCMenu* settings_menu = CCMenu::create();
			CCMenuItemSpriteExtra* pitchBtn = CCMenuItemSpriteExtra::create(pitchBackgroundSprite, this, menu_selector(PitchLayer::songPitch));
			CCSprite* pitchOptions = CCSprite::create("pitchoptions.png"_spr);
			CCLabelBMFont* pitchText = CCLabelBMFont::create("Pitch", "bigFont.fnt");

			pitchBackgroundSprite->setScale(0.75);
			pitchOptions->setScale(0.8);

			CCSprite* settingsGear = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
			settingsGear->setScale(0.6);
			auto settingsBtn = CCMenuItemSpriteExtra::create(settingsGear, this, menu_selector(PitchLayer::openSettings));

			pitchMenu->setPosition({backMenu->getPositionX() + 29, backMenu->getPositionY()});
			settings_menu->setPosition({pitchMenu->getPositionX() + 57, pitchMenu->getPositionY()});
			pitchMenu->addChild(pitchBtn);

			pitchText->setPosition({47, 16});
			pitchText->setScale(0.45);

			pitchOptions->setPosition({pitchMenu->getPositionX() + 25, pitchMenu->getPositionY() - 23});
			pitchOptions->setRotation(-5.f);

			pitchMenu->setID("xanii.pitch_shifter/pitch-menu");
			settings_menu->setID("xanii.pitch_shifter/pitch-settings-menu");
			pitchOptions->setID("xanii.pitch_shifter/pitch-options");

			settings_menu->addChild(settingsBtn, 10);
			pitchBtn->addChild(pitchText, 11);
			addChild(pitchMenu, 10);
			addChild(settings_menu, 10);
			addChild(pitchOptions, 10);
		
			return true;	
		}

	void songPitch(CCObject*) {
		if (songID >= 22) robSong = false;
		else robSong = true;
		if (Mod::get()->getSettingValue<int64_t>("pitch") != 0) {
			geode::createQuickPopup("Pitch Shifter", "This will take <cg>~10 seconds</c> to complete.\n<cr>Do not close the game while this processes.</c>", "Cancel", "OK", [this] (auto fl, bool btn2) {
				if (btn2) pitchSong(songID, Mod::get()->getSettingValue<int64_t>("pitch"), robSong);});
		}
		else pitchSong(songID, Mod::get()->getSettingValue<int64_t>("pitch"), robSong);
	}

	void openSettings(CCObject*) {
    	openSettingsPopup(Mod::get());
	}
};

class $modify(EditPitchLayer, EditLevelLayer) {
	bool init(GJGameLevel* p0) {
		bool result = EditLevelLayer::init(p0);
		songID = p0->m_songID;
		CCSprite* pitchBackgroundSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn02_001.png");
		pitchBackgroundSprite->setScale(.625);
		CCMenu* levelSongName = static_cast<CCMenu*>(this->getChildByIDRecursive("level-song"));
		CCMenu* pitchMenu = CCMenu::create();
		CCMenu* settings_menu = CCMenu::create();
		CCMenuItemSpriteExtra* pitchBtn = CCMenuItemSpriteExtra::create(pitchBackgroundSprite, this, menu_selector(EditPitchLayer::songPitch));
		CCLabelBMFont* pitchText = CCLabelBMFont::create("Pitch", "bigFont.fnt");

		CCSprite* settingsGear = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
		settingsGear->setScale(0.4);
		auto settingsBtn = CCMenuItemSpriteExtra::create(settingsGear, this, menu_selector(EditPitchLayer::openSettings));

		pitchMenu->setPosition({levelSongName->getPositionX() - 6, levelSongName->getPositionY() - 23});
		settings_menu->setPosition({pitchMenu->getPositionX() + 42, pitchMenu->getPositionY()});
		pitchMenu->addChild(pitchBtn);

		pitchText->setPosition({29, 10.5});
		pitchText->setScale(0.4);

		pitchMenu->setID("xanii.pitch_shifter/pitch-menu");
		settings_menu->setID("xanii.pitch_shifter/pitch-settings-menu");

		settings_menu->addChild(settingsBtn, 10);
		pitchBtn->addChild(pitchText, 11);
		addChild(pitchMenu, 10);
		addChild(settings_menu, 10);
		return result;
	}

	void songPitch(CCObject*) {
		if (songID >= 22) robSong = false;
		else robSong = true;
		if (Mod::get()->getSettingValue<int64_t>("pitch") != 0) {
			geode::createQuickPopup("Pitch Shifter", "This will take <cg>~10 seconds</c> to complete.\n<cr>Do not close the game while this processes.</c>", "Cancel", "OK", [this] (auto fl, bool btn2) {
				if (btn2) pitchSong(songID, Mod::get()->getSettingValue<int64_t>("pitch"), robSong);});
		}
		else pitchSong(songID, Mod::get()->getSettingValue<int64_t>("pitch"), robSong);
	}

	void openSettings(CCObject*) {
    	openSettingsPopup(Mod::get());
	}
};