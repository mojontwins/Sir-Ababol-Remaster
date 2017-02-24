// Vostok Engine MK2 v1.0
// Copyleft 2016 by The Mojon Twins

void screen_intro (void) {
	scroll (0,0); cls ();
	pal_bg (pal_bg_0);
	pal_spr (pal_fg_0);

	p_s (5, 26, "@ 2016 THE MOJON TWINS");
	p_s (4, 27, "NOT LICENSED BY NINTENDO");

	ppu_on_all ();
	fade_in ();

	rdy = 240; ticker = 255;
	while (ticker --) {
		if (pad_poll (0) | pad_poll (1)) break;
		if (rdy == 121) sfx_play (SFX_START, SC_LEVEL);
		if (rdy > 120) rdy --;
		oam_meta_spr (102, rdy, 0, spr_logo_00);
		ppu_wait_nmi ();
	}
	oam_hide_rest (0);
	fade_out ();
	ppu_off ();
}

void screen_common (void) {
	ppu_on_all ();
	fade_in ();

	music_play (MUSIC_GAME_OVER);

	game_time = UMBRELLA_SCREEN_TIME; ticker = 0;
	wait_time_or_input ();

	music_stop ();

	fade_out ();
	ppu_off ();
}

void screen_zazelle (void) {
	cls ();

	p_s (7, 4, "HELP MISS ZAZELLE!");
	p_s (6, 6, "COLLECT BLUE FLOWERS");
	p_s (8, 8, "THEN HEAD TO THE");
	p_s (7, 10, "UNDERGROUND DUNGEON");

	oam_meta_spr (116, 120, OAM_FIXED_PLAYER, spr_player [22]);
	oam_meta_spr (128, 114, OAM_FIXED_UMBRELLA, spr_extra_00_a);

	p_s (8, 22, "PRESS B TO THROW");
	p_s (10, 24, "THE UMBRELLA");

	screen_common ();
}

void screen_limite (void) {
	cls ();
	
	p_s (7, 3, "YAY! JOHNNY LIMITE");
	p_s (6, 5, "GETS A GAME AT LAST!");
	p_s (6, 7, "(A BORROWED ONE ALSO");
	p_s (8, 9, "COUNTS, YOU KNOW)");
	p_s (6, 11, "COLLECT BLUE FLOWERS");

	oam_meta_spr (124, 120, OAM_FIXED_PLAYER, spr_player [38]);

	p_s (8, 22, "PRESS B TO FLOAT");
	p_s (8, 24, "AND FALL SLOWLY!");

	screen_common ();
}
