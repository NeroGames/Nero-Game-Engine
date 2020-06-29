////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef CONSTANTPOOL_H
#define CONSTANTPOOL_H
////////////////////////////////////////////////////////////
namespace ng
{
	struct _ObjectPool
	{
	    //Player
	    const std::string player			    = "player_foot";
	    const std::string player_body	        = "player_body";

	    //
	    const std::string platform_part_1       = "platform_part_1";
	    const std::string platform_part_2       = "platform_part_2";
	    const std::string platform_part_3       = "platform_part_3";
	    const std::string platform_part_4       = "platform_part_4";
	};

	struct _ButtonPool
	{
	    const std::string startButton           = "new_game_button";
	    const std::string quitButton            = "quit_game_button";
	    const std::string resumeButton          = "resume_game_button";
	    const std::string restartButton         = "restart_game_button";
	    const std::string optionsButton         = "game_options_button";
	    const std::string startMenuButton       = "start_menu_button";
	    const std::string closeOptionsButton    = "close_options_button";
	    const std::string musicUpButton         = "music_up_button";
	    const std::string musicDownButton       = "music_down_button";
	    const std::string soundUpButton         = "sound_up_button";
	    const std::string soundDownButton       = "sound_down_button";
	};

	struct _TextPool
	{
	    const std::string musicVolumeText       = "music_volume_text";
	    const std::string soundVolumeText       = "sound_volume_text";
	    const std::string countCoinText        = "count_coin_text";
	    const std::string countStarText         = "count_star_text";
	};

	struct _CategoryPool
	{
	    const std::string player                = "player";
	    const std::string ground                = "ground";
	    const std::string wall                  = "wall";
	    const std::string box                   = "box";
	    const std::string platform              = "platform";
	    const std::string key                   = "key";
	    const std::string coin                  = "coin";
	    const std::string star                  = "star";
	};

	struct _SoundPool
	{
	    const std::string button_over           = "button_over";
	    const std::string button_click          = "button_click";
	};

	struct _MusicPool
	{
	    const std::string game_menu             = "game_menu";
	    const std::string netherplace           = "netherplace";
	};

	struct _ScreenPool
	{
	    const std::string game_screen           = "game_screen";
	    const std::string start_screen          = "start_screen";
	    const std::string pause_screen          = "pause_screen";
	    const std::string options_screen        = "options_screen";
	};

	struct _PlayerActionPool
	{
	   const std::string move_left              = "move_left";
	    const std::string move_right            = "move_right";
	    const std::string run_left              = "run_left";
	    const std::string run_right             = "run_right";
	    const std::string move_none             = "move_none";
	    const std::string jump                  = "jump";
	};

	struct _PlayerSequencePool
	{
	    const std::string idle                  = "idle";
	    const std::string walk                  = "walk";
	    const std::string run                   = "run";
	    const std::string jump                  = "jump";
	};

	const _ObjectPool           ObjectPool;
	const _CategoryPool         CategoryPool;
	const _SoundPool     		SoundPool;
	const _MusicPool   			MusicPool;
	const _ScreenPool           ScreenPool;
	const _PlayerActionPool		PlayerActionPool;
	const _PlayerSequencePool   PlayerSequencePool;
	const _ButtonPool           ButtonPool;
	const _TextPool             TextPool;
}

#endif // CONSTANTPOOL_H
