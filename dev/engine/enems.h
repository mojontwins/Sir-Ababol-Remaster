// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

void enems_init_level (void) {
	// Copies (expands & prepares) ROM data to RAM.
	// ROM format is T XY1 XY2 S

	gp_gen = (unsigned char *) enems_ROM + level * MAX_ENEMS * 4;

	en_n_pant = 0; en_ct_aux = 0;
	for (gpit = 0; gpit < MAX_ENEMS; gpit ++) {
		// Read type
		en_t [gpit] = *gp_gen ++; if (!en_t [gpit]) en_t [gpit] = 0xf8; // Custom fix

		// Read XY1
		rda = *gp_gen ++;
		en_x [gpit] = (en_n_pant << 8) + (rda & 0xf0);
		en_y [gpit] = rda << 4;

		// Read XY2
		rdb = *gp_gen ++;

		// Read S
		rdc = *gp_gen ++;

		/*
		switch ((en_t [gpit] >> 4) & 7) {
			case 1:
			case 7:
		*/
				// Linear sprites
				en_x1 [gpit] = LSB (en_x [gpit]);
				en_y1 [gpit] = en_y [gpit];
				en_x2 [gpit] = rdb & 0xf0;
				en_y2 [gpit] = rdb << 4;
				/*
				if (rdc > 128) {
					en_r [gpit] = rdc - 128; rdc = 1;
				} else en_r [gpit] = 0;
				*/
				en_mx [gpit] = add_sign (en_x2 [gpit] - en_x1 [gpit], rdc);
				en_my [gpit] = add_sign (en_y2 [gpit] - en_y1 [gpit], rdc);
				//break;
		//}

		// Keep up
		en_ct_aux ++; if (en_ct_aux == 3) { en_ct_aux = 0; en_n_pant ++; }
	}
}

void enems_kill (void) {
	en_t [rdt] = (en_t [rdt] & 0x07) | 0x08;
	sfx_play (SFX_STEP_OVER_ENEMY, SC_LEVEL);
}

void enems_move (void) {
	// Need to reset this.
	pgtmy = pgtmx = pgotten = 0;

	// Update enemies from current "virtual" screen and the next.
	rda = MSB (cam_pos); 		
	rda = rda + rda + rda; 
	/*
	rdb = rda <= MAX_ENEMS - ENEM_PROCESS_MAX ? rda + ENEM_PROCESS_MAX : MAX_ENEMS;
	for (rdt = rda; rdt < rdb; rdt ++) {
	*/
	// Process them in a more interesting manner (so we get flickering instead of disappearing sprites)
	// We know we have to process 6 enemies in this game, fuck the constants.
	erp_base ++; if (erp_base == 6) erp_base = 0;
	erp_iter = erp_base;
	rdb = 6; while (rdb --) {	
		erp_iter += 5; if (erp_iter >= 6) erp_iter -= 6;
		rdt = rda + erp_iter;
		if (rdt >= MAX_ENEMS) continue;
		
		if ((en_t [rdt] & 0xf8) != 0xf8) {
			ens = ((en_t [rdt] & 0x07) - 1) << 2;

			// Gotten preliminary - You have to tweak this for each game (sprite feet)
			gpjt = (prx + 7 >= en_x [rdt] && prx <= en_x [rdt] + 15);
		
			// Update
			#include "enemmods/enems_update.h"

			// Collision
			// #include "enemmods/enems_collision.h" 
			// moved elsewhere!
			if ((en_t [rdt] & 0x07) == 4) {
				if (gpjt && !pgotten && !pj) {
					if (en_mx [rdt]) {
						if (pry + 16 >= en_y [rdt] && pry + 12 <= en_y [rdt]) {
							pgotten = 1;
							pgtmx = en_mx [rdt] << FIX_BITS;
							pry = en_y [rdt] - 16; py = pry << FIX_BITS;
							pvy = 0;
						}
					} else {
						if (
							(en_my [rdt] < 0 && pry + 17 >= en_y [rdt] && pry + 12 <= en_y [rdt]) ||
							(en_my [rdt] > 0 && pry + 16 + en_my [rdt] >= en_y [rdt] && pry + 12 <= en_y [rdt])
						) {
							pgotten = 1;
							pgtmy = en_my [rdt] << FIX_BITS;
							pry = en_y [rdt] - 16; py = pry << FIX_BITS;
							pvy = 0;
						}
					}
				}
			} 
			
			if (en_x [rdt] >= cam_pos && en_x [rdt] < cam_pos + 240) {
				// Add to OAM
				oam_index = oam_meta_spr (
					en_x [rdt] - cam_pos, en_y [rdt] + SPRITE_ADJUST,
					oam_index,
					spr_enems [en_cell]
				);
			}
		}
	}
}

// Collision moved here, separated from main loop
void enems_collision () {
	rda = MSB (cam_pos); 		
	rda = rda + rda + rda; 
	rdb = rda <= MAX_ENEMS - ENEM_PROCESS_MAX ? rda + ENEM_PROCESS_MAX : MAX_ENEMS;
	for (rdt = rda; rdt < rdb; rdt ++) {
		#include "enemmods/enems_collision.h" 
	}
}
