// Vostok Engine MK2 v1.0
// Copyleft 2016 by The Mojon Twins

/*
void enter_screen (const unsigned char *pal, void (*func) (void)) {
	scroll (0, 0);
	
	cls ();
	(*func) ();
	
	pal_bg (pal);
	ppu_on_all ();
	fade_in ();
}

void exit_cleanly (void) {
	set_vram_update (0);
	oam_hide_rest (0);
	music_stop ();
	fade_out ();
	ppu_off ();	
}
*/
void wait_time_or_input (void) {
	rda = (pad_poll (0) != 0);
	while (game_time) {
		ticker ++; if (ticker == ticks) {
			ticker = 0;
			game_time --;
		}

		if ((pad0 = pad_poll (0)) & keymask) {
			if (!rda) break;
		} else {
			rda = 0;
		}

		ppu_wait_nmi ();
	}
}
/*
void do_general_screen (unsigned char gt, const unsigned char *pal, void (*func) (void)) {
	enter_screen (pal, func);
	game_time = gt; ticker = 0;
	wait_time_or_input ();
	exit_cleanly ();
}
*/
void wait_no_key () {
	while (pad_poll (0));
}

signed int add_sign (signed int sign, signed int value) {
	return sign == 0 ? 0 : sign < 0 ? -value : value;
}

signed int saturate (signed int v, signed int max) {
	return v >= 0 ? (v > max ? max : v) : (v < -max ? -max : v);
}

// Only called once, inline for speed. And obfuscation. Pichude.
/*
unsigned char cl (signed int x1, unsigned char y1, signed int x2, unsigned char y2) {
	return x1 + 4 >= x2 && x1 <= x2 + 12 && y1 + 12 >= y2 && y1 <= y2 + 12;
}
*/

void cm_two_points (void) {
	// Collide map, two points
	// with vertical adjust.
	
	rdx = cx1 >> 1;
	at1 = behs [*(map_ptr + (rdx << 4) + (rdx << 3) + rdx + ((cx1 & 1) ? 10 : 0) + ((cy1 < 2) ? 0 : ((cy1 > 11) ? 9 : cy1 - 2)))];
	
	rdx = cx2 >> 1;
	at2 = behs [*(map_ptr + (rdx << 4) + (rdx << 3) + rdx + ((cx2 & 1) ? 10 : 0) + ((cy2 < 2) ? 0 : ((cy2 > 11) ? 9 : cy2 - 2)))];
}
