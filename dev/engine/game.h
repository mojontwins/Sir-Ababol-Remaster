// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

void game_init (void) {
	map_ptr = (unsigned char *) (map + level * COLUMN_SIZE * MAP_W);
	obj_strip_base = level << 5;
	cs = 0xff;
	erp_base = 0; pal_col(0,0x0f);
}

void game_loop (void) {
	game_res = 0;
	keep_going = 0;

	cls ();

	ppu_on_all ();
	ppu_mask (level ? 0x9e : 0x1e);

	enems_init_level ();
	set_vram_update (update_list);

	player_init ();		// Init player
	scroll_init ();
	camera_do ();		// Calculate camera		
	scroll_draw_screen ();
	
	oam_meta_spr (
		16, 16,
		OAM_FIXED_HUD,
		spr_hud_base); 	// Hud, fixed
	draw_hud ();		// Hud, variable.
	
	//fade_in ();
	pal_bright (4);

	if (music_flag) music_play (MUSIC_INGAME);

	while (1) {
		half_life = 1 - half_life;		// Main flip-flop
		frame_counter ++;				// Increase frame counter
		gp_ul = update_list;			// Reset pointer to update list
		oam_index = 112;				// Reset OAM index; skip fixed-pos sprites
		pad0 = pad_poll (0);			// Read pads here.

		player_move ();
		camera_do ();
		scroll_to ();
		hotspots_update ();
		hotspots_detect ();
		hitter_do ();
		enems_move ();

		//*((unsigned char*)0x2001) = 0x1f;

		*gp_ul = NT_UPD_EOF;
		scroll (cam_pos & 0x01ff, 0);
		oam_hide_rest (oam_index);
		ppu_wait_nmi ();

		//*((unsigned char*)0x2001) = 0x1e;
		
		enems_collision ();

		if (pobjs == PLAYER_MAX_OBJECTS) game_res = PLAYER_WINS_GAME;
		if (game_res) break;

		if (pad0 & PAD_START) {
			wait_no_key ();
			music_pause (1);
			sfx_play (SFX_START, SC_LEVEL);
			pal_bright (3);
			while (!(pad_poll (0) & PAD_START)) ppu_wait_nmi ();
			wait_no_key ();
			pal_bright (4);
			music_pause (0);
		}		

		/*
		if (pad0 & PAD_SELECT) {
			if (!pfiring) {
				pobjs ++;
				sfx_play (SFX_OBJECT, SC_LEVEL);
				draw_hud ();
			}
			pfiring = 1;
		} else pfiring = 0;
		*/
	}
	set_vram_update (0);

	while (keep_going --) {
		ppu_wait_nmi ();
	}

	// Change music flag
	music_flag = (game_res > 2);

	if (music_flag) music_stop ();
	fade_out ();

	oam_hide_rest (0);
	ppu_off ();

	// What do.
}

void game_over (void) {
	scroll (0, 0);
	cls ();
	un_rle_screen ((unsigned char *) scr_rle_2);
	p_s (11, 11, "GAME UNDER");
	pal_bg (pal_bg_0);
	ppu_on_all ();
	fade_in ();
	music_play (MUSIC_GAME_OVER);
	game_time = GAME_OVER_SCREEN_TIME; ticker = 0;
	wait_time_or_input ();
	music_stop ();
	fade_out ();
	ppu_off ();	
}

void game_title (void) {
	un_rle_screen ((unsigned char *) scr_rle_0);
	p_s (5, 16, "@ 2016 THE MOJON TWINS");
	p_s (7, 19, "PRESS START BUTTON");
	oam_meta_spr (120, 191, OAM_FIXED_PLAYER, spr_player [0]);

	pal_spr (pal_fg_0);
	pal_bg 	(pal_bg_0);

	ppu_on_all ();
	fade_in ();

	music_play (MUSIC_MENU);

	game_mode = GAME_MODE_SIR_ABABOL;

	rdx = 240;
	while (1) {
		pad0 = pad_poll (0);
		if (pad0 & PAD_START) break;
		rda = PAD_A | PAD_SELECT | PAD_UP;
		if ((pad0 & rda) == rda) {
			game_mode = GAME_MODE_ZAZELLE;
			break;
		}
		rda = PAD_A | PAD_B | PAD_DOWN;
		if ((pad0 & rda) == rda) {
			game_mode = GAME_MODE_LIMITE;
			break;
		}
		if (pad0 & PAD_RIGHT) {
			if (rdx == 240) {
				rdx = 8;
				pal_spr (pal_fg_2);
			}
		}
		if (rdx < 240) {
			rdx ++;
			oam_meta_spr (rdx, 32, OAM_FIXED_UMBRELLA, spr_extra2_00_a);
			if (rdx == 240) {
				oam_meta_spr (rdx, 240, OAM_FIXED_UMBRELLA, spr_empty);
				pal_spr (pal_fg_0);
			}
		} 
		ppu_wait_frame ();
	}

	music_stop ();
	sfx_play (SFX_START, SC_PLAYER);
	fade_out ();
	oam_hide_rest (0);
	delay (ticks);
	ppu_off ();
}

void showtwolines (const unsigned char *s1, const unsigned char *s2) {
	cls ();
	p_s (10, 13, (unsigned char *) s1);
	p_s (15, 15, (unsigned char *) s2);
	ppu_on_all ();
	fade_in ();
	game_time = 2; ticker = 0;
	wait_time_or_input ();
	fade_out ();
	ppu_off ();
}

void game_ending (void) {
	scroll (0, 0);

	un_rle_screen ((unsigned char *) scr_rle_1);
	switch (game_mode) {
		case GAME_MODE_SIR_ABABOL:
			p_s (4, 3, "-THANK YOU SIR ABABOL");
			p_s (5, 4, "BUT YOUR TORTILLA IS");
			p_s (5, 5, "IN ANOTHER CASTLE");
			p_s (4, 7, "-PODEWWWWR!!");
			oam_meta_spr (96, 183, 0, spr_ending_00);
			oam_meta_spr (144, 183, 36, spr_ending_01);
			break;
		case GAME_MODE_ZAZELLE:
			p_s (4, 3, "-THANK YOU ZAZELLE");
			p_s (5, 4, "BUT YOUR TORTILLA IS");
			p_s (5, 5, "IN ANOTHER CASTLE");
			p_s (4, 7, "-GREAT TOTO!");
			oam_meta_spr (96, 191, 0, spr_player [16]);
			oam_meta_spr (144, 183, 36, spr_ending_01);
			break;
		case GAME_MODE_LIMITE:
			p_s (4, 3, "-THANK YOU JOHNNY LIMITE");
			p_s (5, 4, "BUT YOU STILL DON'T GET");
			p_s (5, 5, "YOUR OWN GAME...");
			p_s (4, 7, "-APUNTALO!");
			oam_meta_spr (96, 191, 0, spr_player [32]);
			oam_meta_spr (144, 183, 36, spr_ending_01);
			break;
	}

	ppu_on_all ();
	fade_in ();
	music_play (MUSIC_ENDING);

	game_time = ENDING_SCREEN_TIME; ticker = 0;
	wait_time_or_input ();

	fade_out ();
	oam_hide_rest (0);
	ppu_off ();

	showtwolines (txt_credits_1, txt_credits_2);
	showtwolines (txt_credits_3, txt_credits_4);
	showtwolines (txt_credits_15, txt_credits_4);
	showtwolines (txt_credits_5, txt_credits_2);
	showtwolines (txt_credits_7, txt_credits_8);
	showtwolines (txt_credits_6, txt_credits_4);
	showtwolines (txt_credits_9, txt_credits_8);
	showtwolines (txt_credits_10, txt_credits_11);
	showtwolines (txt_credits_10, txt_credits_12);
	showtwolines (txt_credits_10, txt_credits_14);
	showtwolines (txt_credits_10, txt_credits_13);

	cls ();
	p_s (5, 16, "@ 2016 THE MOJON TWINS");

	switch (game_mode) {
		case GAME_MODE_SIR_ABABOL:
			p_s (6, 23, "FOR A NICE SURPRISE,");
			p_s (6, 25, "SELECT+A+UP IN TITLE");
			break;
		case GAME_MODE_ZAZELLE:
			p_s (4, 23, "FOR YET ANOTHER SURPRISE,");
			p_s (8, 25, "A+B+DOWN IN TITLE");
			break;
		case GAME_MODE_LIMITE:
			p_s (4, 23, "SORRY, BUT JOHNNY LIMITE");
			p_s (7, 25, "DOESN'T GET A GIFT");
			break;
	}
	oam_meta_spr (102, 104, 0, spr_logo_00);
	
	ppu_on_all ();
	fade_in ();
	
	game_time = THE_END_SCREEN_TIME; ticker = 0;
	wait_time_or_input ();
		
	music_stop ();
	fade_out ();
	oam_hide_rest (0);
	ppu_off ();
}
