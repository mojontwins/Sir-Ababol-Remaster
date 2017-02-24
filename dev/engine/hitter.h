// Vostok Engine MK2 v1.0
// Copyleft 2016 by The Mojon Twins

#define HITTER_STEPS	9
#define HIT_F0			4
#define HIT_F1			6

//                                              H  H  H
const unsigned char hoffs_x [] = { -2, 0, 2, 4, 6, 8, 8, 4, 0 };

void hitter_hide (void) {
	oam_meta_spr (
		0, 240, 
		OAM_FIXED_UMBRELLA,
		spr_extra_00_b
	);
}

void hitter_do (void) {
	if (hitter_on) {
		hitter_y = pry - 6;
		hitter_x = pfacing ? prx - 8 - hoffs_x [hitter_frame] : prx + hoffs_x [hitter_frame];

		oam_meta_spr (
			hitter_x - cam_pos, hitter_y + SPRITE_ADJUST,
			OAM_FIXED_UMBRELLA,
			pfacing ? spr_extra_00_b : spr_extra_00_a
		);

		if (half_life) { hitter_frame ++; if (hitter_frame == HITTER_STEPS) hitter_on = 0; }
	} else {
		hitter_hide ();
	}
}
