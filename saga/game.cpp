/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004 The ScummVM project
 *
 * The ReInherit Engine is (C)2000-2003 by Daniel Balsom.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 *
 */

// Game detection, general game parameters

#include "saga/saga.h"

#include "common/file.h"
#include "common/md5.h"
#include "base/plugins.h"
#include "base/gameDetector.h"
#include "backends/fs/fs.h"

#include "saga/rscfile_mod.h"
#include "saga/interface.h"
#include "saga/scene.h"

#include "saga/game.h"

namespace Saga {

// Inherit the Earth - DOS Demo version

static GAME_FILEDESC ITEDEMO_GameFiles[] = {
	{"ITE.RSC", GAME_RESOURCEFILE},
	{"ITE.DMO", GAME_DEMOFILE},
	{"SCRIPTS.RSC", GAME_SCRIPTFILE},
	{"VOICES.RSC", GAME_SOUNDFILE | GAME_VOICEFILE}
};

static GAME_FONTDESC ITEDEMO_GameFonts[] = {
	{GAME_FONT_SMALL, 0},
	{GAME_FONT_MEDIUM, 1}
}; 

static GAME_SOUNDINFO ITEDEMO_GameSound = {
	GAME_SOUND_VOC, 0, 0, 0
};

static GAME_FONTDESC ITEMACDEMO_GameFonts[] = {
	{GAME_FONT_MEDIUM, 0},
	{GAME_FONT_SMALL, 2}
}; 

// Inherit the Earth - Wyrmkeep Win32 Demo version
static GAME_FILEDESC ITEWINDEMO2_GameFiles[] = {
	{"ITED.RSC", GAME_RESOURCEFILE},
	{"SCRIPTSD.RSC", GAME_SCRIPTFILE},
	{"SOUNDSD.RSC", GAME_SOUNDFILE},
	{"VOICESD.RSC", GAME_VOICEFILE}
};

// Inherit the Earth -  Wyrmkeep Linux Demo version
static GAME_FILEDESC ITELINDEMO_GameFiles[] = {
	{"ITED.RSC", GAME_RESOURCEFILE},
	{"SCRIPTSD.RSC", GAME_SCRIPTFILE},
	{"SOUNDSD.RSC", GAME_SOUNDFILE},
	{"VOICESD.RSC", GAME_VOICEFILE},
	{"MUSICD.RSC", GAME_MUSICFILE}
};

static GAME_FONTDESC ITEWINDEMO_GameFonts[] = {
	{GAME_FONT_MEDIUM, 0},
	{GAME_FONT_SMALL, 2}
}; 

// Inherit the Earth - Mac Wyrmkeep version
static GAME_FILEDESC ITEMACCD_GameFiles[] = {
	{"ite.rsc", GAME_RESOURCEFILE},
	{"scripts.rsc", GAME_SCRIPTFILE},
	{"sounds.rsc", GAME_SOUNDFILE},
	{"Inherit the Earth Voices", GAME_VOICEFILE},
	{"music.rsc", GAME_MUSICFILE}
};

// Inherit the Earth - win32 Wyrmkeep Demo version older release
static GAME_FILEDESC ITEWINDEMO1_GameFiles[] = {
	{"ITED.RSC", GAME_RESOURCEFILE},
	{"SCRIPTSD.RSC", GAME_SCRIPTFILE},
	{"SOUNDSD.RSC", GAME_SOUNDFILE},
	{"VOICESD.RSC", GAME_VOICEFILE}
};

// Inherit the Earth - Diskette version
static GAME_FILEDESC ITEDISK_GameFiles[] = {
	{"ITE.RSC", GAME_RESOURCEFILE},
	{"SCRIPTS.RSC", GAME_SCRIPTFILE},
	{"VOICES.RSC", GAME_SOUNDFILE | GAME_VOICEFILE}
};

static GAME_FONTDESC ITEDISK_GameFonts[] = {
	{GAME_FONT_MEDIUM, 0},
	{GAME_FONT_LARGE, 1},
	{GAME_FONT_SMALL, 2}
};

static GAME_RESOURCEDESC ITE_Resources = {
	RID_ITE_SCENE_LUT,  // Scene lookup table RN
	RID_ITE_SCRIPT_LUT, // Script lookup table RN
	RID_ITE_COMMAND_PANEL,
	RID_ITE_DIALOGUE_PANEL
};

static GAME_SOUNDINFO ITE_GameSound = {
	GAME_SOUND_VOC, 0, 0, 0
};

// Inherit the Earth - CD Enhanced version
static GAME_FILEDESC ITECD_GameFiles[] = {
	{"ITE.RSC", GAME_RESOURCEFILE},
	{"SCRIPTS.RSC", GAME_SCRIPTFILE},
	{"SOUNDS.RSC", GAME_SOUNDFILE},
	{"VOICES.RSC", GAME_VOICEFILE}
};

static GAME_FONTDESC ITECD_GameFonts[] = {
	{GAME_FONT_MEDIUM, 0},
	{GAME_FONT_LARGE, 1},
	{GAME_FONT_SMALL, 2}
};

static GAME_SOUNDINFO ITECD_GameSound = {
	GAME_SOUND_PCM,
	22050,
	16,
	0
};

// I Have No Mouth and I Must Scream - Demo version
static GAME_FILEDESC IHNMDEMO_GameFiles[] = {
	{"SCREAM.RES", GAME_RESOURCEFILE},
	{"SCRIPTS.RES", GAME_SCRIPTFILE},
	{"SFX.RES", GAME_SOUNDFILE},
	{"VOICESD.RES", GAME_VOICEFILE}
};

// I Have No Mouth and I Must Scream - Retail CD version
static GAME_FILEDESC IHNMCD_GameFiles[] = {
	{"MUSICFM.RES", GAME_MUSICFILE_FM},
	{"MUSICGM.RES", GAME_MUSICFILE_GM},
	{"SCREAM.RES", GAME_RESOURCEFILE},
	{"SCRIPTS.RES", GAME_SCRIPTFILE},
	{"SFX.RES", GAME_SOUNDFILE},
	{"VOICES1.RES", GAME_VOICEFILE},
	{"VOICES2.RES", GAME_VOICEFILE},
	{"VOICES3.RES", GAME_VOICEFILE},
	{"VOICES4.RES", GAME_VOICEFILE},
	{"VOICES5.RES", GAME_VOICEFILE},
	{"VOICES6.RES", GAME_VOICEFILE},
	{"VOICESS.RES", GAME_VOICEFILE}
};

static GAME_FONTDESC IHNMCD_GameFonts[] = {
	{GAME_FONT_MEDIUM, 2},
	{GAME_FONT_LARGE, 3},
	{GAME_FONT_SMALL, 4},
	{GAME_FONT_SMALL2, 5},
	{GAME_FONT_MEDIUM2, 6},
	{GAME_FONT_LARGE2, 7},
	{GAME_FONT_LARGE3, 8}
};

static GAME_RESOURCEDESC IHNM_Resources = {
	RID_IHNM_SCENE_LUT,  // Scene lookup table RN
	RID_IHNM_SCRIPT_LUT, // Script lookup table RN
	RID_IHNM_COMMAND_PANEL,
	RID_IHNM_DIALOGUE_PANEL
};

static GAME_SOUNDINFO IHNM_GameSound = {
	GAME_SOUND_WAV, 0, 0, 0
};

struct GAME_MD5 {
	GAME_IDS id;
	const char *md5;
	const char *filename;
	uint32 len;
};

static GAME_MD5 game_md5[] = {
	{ GID_ITE_DISK_G,   "8f4315a9bb10ec839253108a032c8b54", "ite.rsc", 5000 },
	{ GID_ITE_DISK_G,   "516f7330f8410057b834424ea719d1ef", "scripts.rsc", 5000 },
	{ GID_ITE_DISK_G,   "c46e4392fcd2e89bc91e5567db33b62d", "voices.rsc", 5000 },

	{ GID_ITE_CD_G,     "8f4315a9bb10ec839253108a032c8b54", "ite.rsc", 5000 },
	{ GID_ITE_CD_G,     "a891405405edefc69c9d6c420c868b84", "scripts.rsc", 5000 },
	{ GID_ITE_CD_G,     "e2ccb61c325d6d1ead3be0e731fe29fe", "sounds.rsc", 5000 },
	{ GID_ITE_CD_G,     "41bb6b95d792dde5196bdb78740895a6", "voices.rsc", 5000 },

	{ GID_ITE_DEMO_G,   "986c79c4d2939dbe555576529fd37932", "ite.rsc", 5000 },
	{ GID_ITE_DEMO_G,   "d5697dd3240a3ceaddaa986c47e1a2d7", "scripts.rsc", 5000 },
	{ GID_ITE_DEMO_G,   "c58e67c506af4ffa03fd0aac2079deb0", "voices.rsc", 5000 },

	{ GID_ITE_MACCD,    "4f7fa11c5175980ed593392838523060", "ite.rsc", 5000 },
	{ GID_ITE_MACCD,    "adf1f46c1d0589083996a7060c798ad0", "scripts.rsc", 5000 },
	{ GID_ITE_MACCD,    "1a91cd60169f367ecb6c6e058d899b2f", "music.rsc", 5000 },
	{ GID_ITE_MACCD,    "95863b89a0916941f6c5e1789843ba14", "sounds.rsc", 5000 },
	{ GID_ITE_MACCD,    "c14c4c995e7a0d3828e3812a494301b7", "Inherit the Earth Voices", 5000 },

	{ GID_ITE_DISK_DE,  "869fc23c8f38f575979ec67152914fee", "ite.rsc", 5000 },
	{ GID_ITE_DISK_DE,  "516f7330f8410057b834424ea719d1ef", "scripts.rsc", 5000 },
	{ GID_ITE_DISK_DE,  "0c9113e630f97ef0996b8c3114badb08", "voices.rsc", 5000 },

	{ GID_ITE_WINDEMO2, "3a450852cbf3c80773984d565647e6ac", "ited.rsc", 5000 },
	{ GID_ITE_WINDEMO2, "3f12b67fa93e56e1a6be39d2921d80bb", "scriptsd.rsc", 5000 },
	{ GID_ITE_WINDEMO2, "95a6c148e22e99a8c243f2978223583c", "soundsd.rsc", 5000 },
	{ GID_ITE_WINDEMO2, "e139d86bab2ee8ba3157337f894a92d4", "voicesd.rsc", 5000 },

	{ GID_ITE_LINDEMO,  "3a450852cbf3c80773984d565647e6ac", "ited.rsc", 5000 },
	{ GID_ITE_LINDEMO,  "3f12b67fa93e56e1a6be39d2921d80bb", "scriptsd.rsc", 5000 },
	{ GID_ITE_LINDEMO,  "d6454756517f042f01210458abe8edd4", "musicd.rsc", 5000 },
	{ GID_ITE_LINDEMO,  "95a6c148e22e99a8c243f2978223583c", "soundsd.rsc", 5000 },
	{ GID_ITE_LINDEMO,  "e139d86bab2ee8ba3157337f894a92d4", "voicesd.rsc", 5000 },

	{ GID_ITE_MACDEMO2, "addfc9d82bc2fa1f4cab23743c652c08", "ited.rsc", 5000 },
	{ GID_ITE_MACDEMO2, "fded5c59b8b7c5976229f960d21e6b0b", "scriptsd.rsc", 5000 },
	{ GID_ITE_MACDEMO2, "495bdde51fd9f4bea2b9c911091b1ab2", "musicd.rsc", 5000 },
	{ GID_ITE_MACDEMO2, "b3a831fbed337d1f1300fee1dd474f6c", "soundsd.rsc", 5000 },
	{ GID_ITE_MACDEMO2, "e139d86bab2ee8ba3157337f894a92d4", "voicesd.rsc", 5000 },

	{ GID_ITE_WINDEMO1, "3a450852cbf3c80773984d565647e6ac", "ited.rsc", 5000 },
	{ GID_ITE_WINDEMO1, "3f12b67fa93e56e1a6be39d2921d80bb", "scriptsd.rsc", 5000 },
	{ GID_ITE_WINDEMO1, "a741139dd7365a13f463cd896ff9969a", "soundsd.rsc", 5000 },
	{ GID_ITE_WINDEMO1, "0759eaf5b64ae19fd429920a70151ad3", "voicesd.rsc", 5000 },

	{ GID_ITE_MACDEMO1, "addfc9d82bc2fa1f4cab23743c652c08", "ited.rsc", 5000 },
	{ GID_ITE_MACDEMO1, "fded5c59b8b7c5976229f960d21e6b0b", "scriptsd.rsc", 5000 },
	{ GID_ITE_MACDEMO1, "1a91cd60169f367ecb6c6e058d899b2f", "musicd.rsc", 5000 },
	{ GID_ITE_MACDEMO1, "b3a831fbed337d1f1300fee1dd474f6c", "soundsd.rsc", 5000 },
	{ GID_ITE_MACDEMO1, "e139d86bab2ee8ba3157337f894a92d4", "voicesd.rsc", 5000 },
};

static bool gameMD5check(int descNum);

static GAMEDESC GameDescs[] = {
	// Inherit the earth - DOS Demo version
	{
		"ite-demo",
		GType_ITE,
		GID_ITE_DEMO_G, // Game id
		"Inherit the Earth (DOS Demo)", // Game title
		320, 200, // Logical resolution
		137, // Scene viewport height
		ITE_DEFAULT_SCENE, // Starting scene number
		&ITE_Resources,
		ARRAYSIZE(ITEDEMO_GameFiles), // Game datafiles
		ITEDEMO_GameFiles,
		ARRAYSIZE(ITEDEMO_GameFonts),
		ITEDEMO_GameFonts,
		&ITEDEMO_GameSound,
		0, // features
	},

	// Inherit the earth - MAC Demo version
	{
		"ite-demo",
		GType_ITE,
		GID_ITE_MACDEMO2,
		"Inherit the Earth (MAC Demo)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO2_GameFiles),
		ITEWINDEMO2_GameFiles,
		ARRAYSIZE(ITEMACDEMO_GameFonts),
		ITEMACDEMO_GameFonts,
		&ITECD_GameSound,
		GF_VOX_VOICES | GF_BIG_ENDIAN_DATA | GF_MAC_RESOURCES
	},

	// Inherit the earth - early MAC Demo version
	{
		"ite-demo",
		GType_ITE,
		GID_ITE_MACDEMO1,
		"Inherit the Earth (early MAC Demo)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO2_GameFiles),
		ITEWINDEMO2_GameFiles,
		ARRAYSIZE(ITEMACDEMO_GameFonts),
		ITEMACDEMO_GameFonts,
		&ITECD_GameSound,
		GF_BIG_ENDIAN_DATA | GF_MAC_RESOURCES
	},

	// Inherit the earth - MAC CD Wyrmkeep version
	{
		"ite",
		GType_ITE,
		GID_ITE_MACCD,
		"Inherit the Earth (MAC CD)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEMACCD_GameFiles),
		ITEMACCD_GameFiles,
		ARRAYSIZE(ITEMACDEMO_GameFonts),
		ITEMACDEMO_GameFonts,
		&ITECD_GameSound,
		GF_BIG_ENDIAN_DATA | GF_MAC_RESOURCES
	},

	// Inherit the earth - Linux Demo version
	// Note: it should be before GID_ITE_WINDEMO2 version
	{
		"ite-demo",
		GType_ITE,
		GID_ITE_LINDEMO,
		"Inherit the Earth (Linux Demo)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITELINDEMO_GameFiles),
		ITELINDEMO_GameFiles,
		ARRAYSIZE(ITECD_GameFonts),
		ITECD_GameFonts,
		&ITECD_GameSound,
		GF_VOX_VOICES
	},
	
	// Inherit the earth - Linux Demo version
	{
		"ite-demo",
		GType_ITE,
		GID_ITE_WINDEMO2,
		"Inherit the Earth (Win32 Demo)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO2_GameFiles),
		ITEWINDEMO2_GameFiles,
		ARRAYSIZE(ITECD_GameFonts),
		ITECD_GameFonts,
		&ITECD_GameSound,
		GF_VOX_VOICES
	},
	
	// Inherit the earth - Win32 Demo version
	{
		"ite-demo",
		GType_ITE,
		GID_ITE_WINDEMO1,
		"Inherit the Earth (early Win32 Demo)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEWINDEMO1_GameFiles),
		ITEWINDEMO1_GameFiles,
		ARRAYSIZE(ITEWINDEMO_GameFonts),
		ITEWINDEMO_GameFonts,
		&ITECD_GameSound,
		GF_VOX_VOICES
	},
	
	// Inherit the earth - CD version
	{
		"ite",
		GType_ITE,
		GID_ITE_CD_G,
		"Inherit the Earth (DOS/Win32 CD Version)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITECD_GameFiles),
		ITECD_GameFiles,
		ARRAYSIZE(ITECD_GameFonts),
		ITECD_GameFonts,
		&ITECD_GameSound,
		0
	},

	// Inherit the earth - German Floppy version
	{
		"ite",
		GType_ITE,
		GID_ITE_DISK_DE,
		"Inherit the Earth (De DOS Floppy)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEDISK_GameFiles),
		ITEDISK_GameFiles,
		ARRAYSIZE(ITEDISK_GameFonts),
		ITEDISK_GameFonts,
		&ITE_GameSound,
		GF_LANG_DE
	},

	// Inherit the earth - Disk version
	{
		"ite",
		GType_ITE,
		GID_ITE_DISK_G,
		"Inherit the Earth (DOS Floppy)",
		320, 200,
		137,
		ITE_DEFAULT_SCENE,
		&ITE_Resources,
		ARRAYSIZE(ITEDISK_GameFiles),
		ITEDISK_GameFiles,
		ARRAYSIZE(ITEDISK_GameFonts),
		ITEDISK_GameFonts,
		&ITE_GameSound,
		0
	},

	// I Have No Mouth And I Must Scream - Demo version
	{
		"ihnm-demo",
		GType_IHNM,
		GID_IHNM_DEMO,
		"I Have No Mouth and I Must Scream (DOS Demo)",
		640, 480,
		7,
		0,
		&IHNM_Resources,
		ARRAYSIZE(IHNMDEMO_GameFiles),
		IHNMDEMO_GameFiles,
		0,
		NULL,
		&IHNM_GameSound,
		GF_DEFAULT_TO_1X_SCALER
	},

	// I Have No Mouth And I Must Scream - CD version
	{
		"ihnm",
		GType_IHNM,
		GID_IHNM_CD,
		"I Have No Mouth and I Must Scream (DOS)",
		640, 480,
		7,
		152,
		&IHNM_Resources,
		ARRAYSIZE(IHNMCD_GameFiles),
		IHNMCD_GameFiles,
		ARRAYSIZE(IHNMCD_GameFonts),
		IHNMCD_GameFonts,
		&IHNM_GameSound,
		GF_DEFAULT_TO_1X_SCALER
	}
};

static GAMEMODULE GameModule;

int SagaEngine::initGame() {
	uint16 game_n;

	if (detectGame(&game_n) != SUCCESS) {
		warning("No valid games were found in the specified directory.");
		return FAILURE;
	}

	if (loadGame(game_n) != SUCCESS) {
		warning("Error loading game resource files.");
		return FAILURE;
	}

	// Load dialogue file 
	loadLanguage();

	return SUCCESS;
}

int SagaEngine::loadLanguage() {
	char lang_file[MAXPATH];
	uint16 game_n;

	File test_file;

	game_n = GameModule.game_number;

	if (GameDescs[game_n].gd_game_type == GType_ITE) {
		snprintf(lang_file, MAXPATH, "%s%s.%s", GAME_ITE_LANG_PREFIX, GameModule.game_language, GAME_LANG_EXT);
		if (!test_file.open(lang_file)) {
			debug(0, "Couldn't open language file %s. Using default (US English)", lang_file);
			return SUCCESS;
		}

		test_file.close();

		if (_vm->_interface->registerLang() != SUCCESS) {
			warning("Error registering interface language cvars");
			return FAILURE;
		}

		if (_vm->_scene->ITEIntroRegisterLang() != SUCCESS) {
			warning("Error registering intro sequence language cvars");
			return FAILURE;
		}

		debug(0, "Using language file %s.", lang_file);
		// FIXME
		//CFG_Read(lang_path);
	} else {
		debug(0, "Language support for this game not implemented.");
	}

	return SUCCESS;
}

RSCFILE_CONTEXT *SagaEngine::getFileContext(uint16 type, int param) {
	RSCFILE_CONTEXT *found_ctxt = NULL;
	uint16 i;

	for (i = 0; i < GameModule.gfile_n; i++) {
		if (GameModule.gfile_data[i].file_types & type) {
			found_ctxt = GameModule.gfile_data[i].file_ctxt;
		}
	}

	return found_ctxt;
}

DetectedGameList GAME_ProbeGame(const FSList &fslist) {
	uint16 game_count = ARRAYSIZE(GameDescs);
	uint16 game_n;
	DetectedGameList detectedGames;

	uint16 file_count;
	uint16 file_n;
	File test_file;

	int file_missing = 0;
	int found_game = 0;

	for (game_n = 0; (game_n < game_count) && !found_game; game_n++) {
		file_count = GameDescs[game_n].gd_filect;
		file_missing = 0;

		// Try to open all files for this game
		for (file_n = 0; file_n < file_count; file_n++) {
			file_missing = 1;
			// Iterate over all files in the given directory
			for (FSList::const_iterator file = fslist.begin(); file != fslist.end(); ++file) {
				if (!file->isDirectory()) {
					const char *gameName = file->displayName().c_str();

					if (0 == scumm_stricmp(GameDescs[game_n].gd_filedescs[file_n].gf_fname, 
										gameName)) {
						file_missing = 0;
						break;
					}
				}
			}

			if (file_missing)
				break;
		}

		// Try the next game, couldn't find all files for the current 
		// game
		if (file_missing) {
			continue;
		} else {
			detectedGames.push_back(GameDescs[game_n].toGameSettings());
			return detectedGames;
		}
	}

	return detectedGames;
}

int SagaEngine::detectGame(uint16 *game_n_p) {
	uint16 game_count = ARRAYSIZE(GameDescs);
	uint16 game_n;

	uint16 file_count;
	uint16 file_n;
	File test_file;

	int file_missing = 0;

	if (game_n_p == NULL) {
		return FAILURE;
	}

	for (game_n = 0; game_n < game_count; game_n++) {
		file_count = GameDescs[game_n].gd_filect;
		file_missing = 0;

		// Try to open all files for this game
		for (file_n = 0; file_n < file_count; file_n++) {
			if (!test_file.open(GameDescs[game_n].gd_filedescs[file_n].gf_fname)) {
				file_missing = 1;
				break;
			}
			test_file.close();
		}

		// Try the next game, couldn't find all files for the current 
		// game
		if (file_missing) {
			continue;
		} else {
			debug(0, "Pre-Found game: %s", GameDescs[game_n].gd_title);
			if (!gameMD5check(game_n))
				continue;
		}

		debug(0, "Found game: %s", GameDescs[game_n].gd_title);
		*game_n_p = game_n;
		return SUCCESS;
	}

	return FAILURE;
}

int SagaEngine::loadGame(uint16 game_n) {
	RSCFILE_CONTEXT *load_ctxt;
	uint16 game_count = ARRAYSIZE(GameDescs);
	const char *game_fname;
	uint16 game_filect;
	uint16 i;

	if (game_n >= game_count) {
		return FAILURE;
	}

	GameModule.game_number = game_n;
	GameModule.gamedesc = &GameDescs[game_n];

	_gameId = GameModule.gamedesc->gd_game_id;
	_gameType = GameModule.gamedesc->gd_game_type;
	_features = GameModule.gamedesc->features;

	game_filect = GameDescs[game_n].gd_filect;
	
	GameModule.gfile_data = (GAME_FILEDATA *)malloc(game_filect * sizeof *GameModule.gfile_data);
	if (GameModule.gfile_data == NULL) {
		return MEM;
	}

	GameModule.gfile_n = game_filect;

	// Load game resource files
	for (i = 0; i < game_filect; i++) {
		load_ctxt = RSC_CreateContext();
		game_fname = GameDescs[game_n].gd_filedescs[i].gf_fname;
		if (RSC_OpenContext(load_ctxt, game_fname) != SUCCESS) {
			return FAILURE;
		}

		debug(0, "Opened resource file: %s", game_fname);
		GameModule.gfile_data[i].file_ctxt = load_ctxt;
		GameModule.gfile_data[i].file_types = GameDescs[game_n].gd_filedescs[i].gf_type;
		GameModule.gfile_data[i].file_flags = 0;
	}

	// Load game font data
	GameModule.gd_fontct = GameDescs[game_n].gd_fontct;
	GameModule.gd_fontdescs = GameDescs[game_n].gd_fontdescs;

	return SUCCESS;
}

const GAME_RESOURCEDESC SagaEngine::getResourceInfo(void) {
	return *GameModule.gamedesc->gd_resource_desc;
}

const GAME_SOUNDINFO SagaEngine::getSoundInfo(void) {
	return *GameModule.gamedesc->gd_soundinfo;
}

int SagaEngine::getDisplayInfo(GAME_DISPLAYINFO *disp_info) {
	int game_n;

	assert(disp_info != NULL);

	game_n = GameModule.game_number;

	disp_info->logical_w = GameDescs[game_n].gd_logical_w;
	disp_info->logical_h = GameDescs[game_n].gd_logical_h;
	disp_info->scene_h = GameDescs[game_n].gd_scene_h;

	return SUCCESS;
}

int SagaEngine::getFontInfo(GAME_FONTDESC **gf_desc, int *font_n) {
	assert((gf_desc != NULL) && (font_n != NULL));

	*gf_desc = GameModule.gd_fontdescs;
	*font_n = GameModule.gd_fontct;

	return SUCCESS;
}

int SagaEngine::getSceneInfo(GAME_SCENEDESC *gs_desc) {
	assert(gs_desc != NULL);

	gs_desc->first_scene = GameModule.gamedesc->gd_startscene;
	gs_desc->scene_lut_rn = RSC_ConvertID(GameModule.gamedesc->gd_resource_desc->scene_lut_rn);

	return SUCCESS;
}

static bool gameMD5check(int descNum) {
	bool result = true;
	char md5str[32+1];
	uint8 md5sum[16];

	for (int i = 0; i < ARRAYSIZE(game_md5); i++) {
		if (game_md5[i].id == GameDescs[descNum].gd_game_id) {
			if (md5_file(game_md5[i].filename, md5sum, NULL, game_md5[i].len)) {
				for (int j = 0; j < 16; j++) {
					sprintf(md5str + j*2, "%02x", (int)md5sum[j]);
				}
				
				if (strcmp(game_md5[i].md5, md5str)) {
					result = false;
					debug(0, "!MD5 for %s wrong", game_md5[i].filename);
					break;
				} else {
					debug(0, "MD5 for %s correct", game_md5[i].filename);
				}
			} else {
				result = false;
			}
		}
	}

	return result;
}

} // End of namespace Saga
