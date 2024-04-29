#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <ShlObj_core.h>
#include <filesystem>

using namespace geode::prelude;

void deleteSongs(std::filesystem::path folderPath, std::filesystem::path songLocation, int songID) {
	auto gdFolderPath = std::filesystem::current_path();
	DeleteFile(fmt::format("{}\\{}copy.mp3",folderPath, songID).c_str());
	DeleteFile(fmt::format("{}\\{}.wav",folderPath, songID).c_str());
	DeleteFile(fmt::format("{}\\{}wav.wav",folderPath, songID).c_str());
	std::filesystem::copy(fmt::format("{}\\{}.mp3", folderPath, songID).c_str(), fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), std::filesystem::copy_options::update_existing);
	DeleteFile(fmt::format("{}\\{}.mp3",folderPath, songID).c_str());
	FLAlertLayer::create("Pitch Shifter", "<cg>Your song has been pitched!</c>", "OK")->show();
}

void pitchSong(int songID, int pitchShift) {
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
	if (appdata != NULL && pitchShift != 0) {
		if (std::filesystem::exists(fmt::format("{}\\{}.mp3", songLocation, songID))) {
			if (!std::filesystem::exists(fmt::format("{}\\pitch_backups", gdFolderPath).c_str())) CreateDirectoryA(fmt::format("{}\\pitch_backups", gdFolderPath).c_str(), NULL);
			CopyFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), fmt::format("{}\\{}copy.mp3", folderPath, songID).c_str(), true);
			CopyFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songID).c_str(), true);
			std::string convertWav = fmt::format("ffmpeg -y -i {}copy.mp3 {}wav.wav -loglevel quiet", songID, songID);
			std::string shiftWav = fmt::format("sox --no-show-progress {}wav.wav {}.wav pitch {}", songID, songID, pitchShift);
			std::string convertMp3 = fmt::format("ffmpeg -y -i {}.wav -acodec libmp3lame {}.mp3 -loglevel quiet", songID, songID);
			system(fmt::format("cd {} && {} && {} && {}", folderPath, convertWav, shiftWav, convertMp3).c_str());
			deleteSongs(folderPath, songLocation, songID);
		}
		else log::debug("Sound file not found");
	}
	else if (appdata != NULL && pitchShift == 0) {
		DeleteFile(fmt::format("{}\\{}.mp3", songLocation, songID).c_str());
		std::filesystem::copy(fmt::format("{}\\pitch_backups\\{}.mp3", gdFolderPath, songID).c_str(), fmt::format("{}\\{}.mp3", songLocation, songID).c_str(), std::filesystem::copy_options::update_existing);
	}
}


class $modify(PitchLayer, CustomSongWidget) {

int songID;

	bool init(SongInfoObject* songInfo, CustomSongDelegate* songDelegate, bool showSongSelect, bool showPlayMusic, bool showDownload, bool isRobtopSong, bool unkBool, bool isMusicLibrary) {
		if (!CustomSongWidget::init(songInfo, songDelegate, showSongSelect, showPlayMusic, showDownload, isRobtopSong, unkBool, isMusicLibrary)) return false;
		m_fields->songID = m_songInfoObject->m_songID;

		CCSprite* btnsprite = CCSprite::createWithSpriteFrameName("difficulty_10_btn2_001.png");
		CCMenu* btnmenu = CCMenu::create();
		CCMenu* settings_menu = CCMenu::create();
		CCMenuItemSpriteExtra* btn = CCMenuItemSpriteExtra::create(btnsprite, this, menu_selector(PitchLayer::songPitch));

		CCSprite* settingsGear = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    	settingsGear->setScale(0.6);
		auto settingsBtn = CCMenuItemSpriteExtra::create(settingsGear, this, menu_selector(PitchLayer::openSettings));


		btnmenu->setPosition(200, 200);
		settings_menu->setPosition({btnmenu->getPositionX(), btnmenu->getPositionY() - 30});

		btnmenu->addChild(btn);
		addChild(btnmenu);
    	settings_menu->addChild(settingsBtn);
		addChild(settings_menu);

		return true;
	}

	void songPitch(CCObject*) {
		if (Mod::get()->getSettingValue<int64_t>("pitch") != 0) {
			geode::createQuickPopup("Pitch Shifter", "This will take <cg>~10 seconds</c> to complete.\n<cr>Do not close the game while this processes.</c>", "Cancel", "OK", [this] (auto fl, bool btn2) {
                if (btn2)
				pitchSong(m_fields->songID, Mod::get()->getSettingValue<int64_t>("pitch"));
			});
		}
		else {
			pitchSong(m_fields->songID, Mod::get()->getSettingValue<int64_t>("pitch"));
			FLAlertLayer::create("Pitch Shifter", fmt::format("{}.mp3 has been reset back to its normal pitch!", m_fields->songID), "OK")->show();
		}
	}

	void openSettings(CCObject*) {
    	openSettingsPopup(Mod::get());
	}
};