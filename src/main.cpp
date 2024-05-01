#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
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
			else FLAlertLayer::create("Pitch Shifter Error", "You must pitch a song before you can reset it!\n<cr>(You set pitch value to 0)</c>", "OK")->show();
		}
		else{
		if (std::filesystem::exists(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songName))) {
			DeleteFile(fmt::format("{}\\{}.mp3", gdFolderPath, songName).c_str());
			std::filesystem::copy(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songName).c_str(), fmt::format("{}\\{}.mp3", gdFolderPath, songName).c_str(), std::filesystem::copy_options::update_existing);
			FLAlertLayer::create("Pitch Shifter", fmt::format("{}.mp3 has been reset back to its normal pitch!", songName), "OK")->show();
			}
			else FLAlertLayer::create("Pitch Shifter Error", "You must pitch a song before you can reset it!\n<cr>(You set pitch value to 0)</c>", "OK")->show();
		}
	}
}

class $modify(CustomSongWidget) {
	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary) {
		bool result = CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary);
		songID = m_songInfoObject->m_songID; // customSongWidget also renders outside of levelinfolayer
		return result;
	}
};

class $modify(PitchLayer, LevelInfoLayer) {
	static void onModify(auto & self)
	{
		(void) self.setHookPriority("LevelInfoLayer::init", INT64_MIN + 1);
	}
	bool init(GJGameLevel* level, bool isRobTopLevel) {
		if (!LevelInfoLayer::init(level, isRobTopLevel)) { return false; }

		auto customSongWidgetButtons = m_songWidget->getChildByID("buttons-menu");
		
		CCSprite* pitchSettingsButton = CCSprite::create("logo.png"_spr);
		CCSprite* pitchBackgroundSprite = CCSprite::createWithSpriteFrameName("GJ_longBtn02_001.png");
		
		CCLabelBMFont* pitchText = CCLabelBMFont::create("Pitch", "bigFont.fnt");

		pitchBackgroundSprite->setScale(0.5);
		pitchText->setScale(0.35);
		pitchSettingsButton->setScale(0.2);
		
		CCMenuItemSpriteExtra* pitchBtn = CCMenuItemSpriteExtra::create(pitchBackgroundSprite, this, menu_selector(PitchLayer::songPitch));
		CCMenuItemSpriteExtra* settingsBtn = CCMenuItemSpriteExtra::create(pitchSettingsButton, this, menu_selector(PitchLayer::openSettings));

		pitchText->setPosition({pitchBackgroundSprite->getPositionX(), pitchBackgroundSprite->getPositionY() + 1.25f});
		pitchBtn->addChild(pitchText, 11);
		
		auto moreBtn = customSongWidgetButtons->getChildByID("more-button");
		pitchBtn->setPosition({moreBtn->getPositionX(), moreBtn->getPositionY() - moreBtn->getContentHeight() + 3.5f});
		settingsBtn->setPosition({pitchBtn->getPositionX() + (pitchBtn->getContentWidth() / 1.25f), pitchBtn->getPositionY()});

		pitchBtn->setID("pitch-button"_spr);
		settingsBtn->setID("pitch-settings-button"_spr);
		
		customSongWidgetButtons->addChild(pitchBtn);
		customSongWidgetButtons->addChild(settingsBtn);
		
		customSongWidgetButtons->updateLayout();
		
		return true;
	}

	void songPitch(CCObject* sender) {
		if (songID >= 22) { robSong = false; }
		else { robSong = true; }
		if (Mod::get()->getSettingValue<int64_t>("pitch") != 0) {
			geode::createQuickPopup(
				"Pitch Shifter",
				"This will take <cg>about 10 seconds</c> to complete.\n<cr>Do NOT close the game until this is complete.</c>",
				"Cancel",
				"OK",
				[this] (auto fl, bool btn2) {
					if (btn2) { pitchSong(songID, Mod::get()->getSettingValue<int64_t>("pitch"), robSong); }
				}
			);
		}
		else {
			pitchSong(songID, Mod::get()->getSettingValue<int64_t>("pitch"), robSong);
		}
	}

	void openSettings(CCObject*) {
    	openSettingsPopup(Mod::get());
	}
};