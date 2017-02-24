// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

// Shiru's

#include "neslib.h"

// Some needed stuff...

#include "definitions.h"

// Main const includes

#include "assets/palettes.h"
#include "assets/metasprites.h"
#include "assets/map.h"
#include "assets/enems.h"
#include "assets/tspals.h"
#include "assets/precalcs.h"
#include "assets/rle_screens.h"
#include "assets/strings.h"

// Some variables

#pragma bss-name (push,"ZEROPAGE")
#pragma data-name(push,"ZEROPAGE")

#include "ram/globals_zp.h"

#pragma bss-name (push,"BSS")
#pragma data-name(push,"BSS")

#include "ram/globals_bss.h"

// Engine functions

#include "engine/printer.h"
#include "engine/general.h"
#include "engine/scroll.h"
#include "engine/hitter.h"
#include "engine/player.h"
#include "engine/enems.h"
#include "engine/camera.h"
#include "engine/hotspots.h"
#include "engine/screens.h"
#include "engine/game.h"

// Functions

void main (void) {
	ticks = ppu_system () ? 60 : 50;
	keymask = PAD_A | PAD_B | PAD_START | PAD_SELECT;

	bank_bg (0);
	bank_spr (1);
	oam_size (0);
	pal_bright (0);

	screen_intro ();

	engine_type_v = ENGINE_TYPE_V_JUMP;
	engine_type_h = ENGINE_TYPE_H_WALK;

	while (1) {
		// title screen
		game_title ();

		// KISS for now, expand "when" needed.
		switch (game_mode) {
			case GAME_MODE_SIR_ABABOL:
				level = 0;
				px = 16 << FIX_BITS;
				py = 32 << FIX_BITS;
				pbase = 0;
				pal_spr (pal_fg_0);
				pal_bg 	(pal_bg_0);
				break;
			case GAME_MODE_ZAZELLE:
				level = 1;
				px = (31*16) << FIX_BITS;
				py = 64 << FIX_BITS;
				pbase = 16;
				pal_spr (pal_fg_1);
				pal_bg 	(pal_bg_1);
				screen_zazelle ();
				break;
			case GAME_MODE_LIMITE:
				level = 0;
				px = 16 << FIX_BITS;
				py = 32 << FIX_BITS;
				pbase = 32;
				pal_spr (pal_fg_2);
				pal_bg 	(pal_bg_2);
				screen_limite ();
				break;
		}

		plives = PLAYER_INITIAL_LIVES;
		pobjs = 0;
		pkeys = 0;
		pvx = 0; pvy = 0;
		safe_level = level; safe_prx = px >> FIX_BITS; safe_pry = py >> FIX_BITS;

		do_game = 1;
		music_flag = 1;
		chunk_objs_init ();

		while (do_game) {
			game_init ();		// Inits game		
			game_loop ();		// Plays game

			// What do?
			switch (game_res) {
				case PLAYER_EXIT_TOP:
					level --;
					pvx = 0; pvy = -PLAYER_VY_JUMP_MAX;
					py = 2816;
					break;
				case PLAYER_EXIT_BOTTOM:
					level ++;
					pvx = pvy = 0;
					py = 0;
					break;
				case PLAYER_ONE_LIFE_LESS:
					level = safe_level;
					px = safe_prx << FIX_BITS;
					py = safe_pry << FIX_BITS;
					pvx = 0; pvy = 0;
					pflickering = 150;

					if (plives) plives --; else {
						game_over ();
						do_game = 0;
					}
					break;
				case PLAYER_WINS_GAME:
					game_ending ();
					do_game = 0;
					break;
			}
		}	
	}
}
