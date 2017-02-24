// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

void chunk_objs_init (void) {
	gp_gen = (unsigned char *) bolts;
	gp_aux = (unsigned char *) objs	;
	gpjt = 0; gpit = 48; while (gpit --) {
		rdb = ((gpjt >> 1) & 15) << 4;

		// locks is XY
		rda = *gp_gen ++;
		obj_x [gpjt] = rdb + (rda >> 4); obj_y [gpjt] = rda & 15; obj_t [gpjt ++] = !!rda;

		// obj is X TY
		obj_x [gpjt] = rdb + *gp_aux ++; rda = *gp_aux ++;
		obj_y [gpjt] = rda & 15; obj_t [gpjt ++] = rda >> 4;
	} 
}

void hotspots_update (void) {
	rda = MSB (prx);
	if (rda != cs) {
		cs = rda;
		rda <<= 1; rda += obj_strip_base;
		cs_bx = obj_x [rda] << 4; 
		cs_by = 32 + (obj_y [rda] << 4);
		cs_ba = obj_t [rda ++];
		cs_ox = obj_x [rda] << 4;
		cs_oy = 32 + (obj_y [rda] << 4);
		cs_ot = obj_t [rda];
	}
}

void hotspots_detect (void) {
	rdc = obj_strip_base + cs + cs;
	if (cs_ot) {
		if (prx + 7 >= cs_ox && prx <= cs_ox + 15 && pry + 15 >= cs_oy && pry <= cs_oy + 15) {
			rda = SFX_OBJECT;
			switch (cs_ot) {
				case 1:
					plives ++; rda = SFX_EXTRA_LIFE; break;
				case 2:
					pobjs ++; break;
				case 3:
					pkeys ++; break;
			}
			cs_ot = obj_t [rdc + 1] = 0;
			sfx_play (rda, SC_LEVEL);
			col_idx = cs_ox >> 5;
			scroll_draw_one_chunk_completely ();
			draw_hud ();
		}
	}

	if (cs_ba) {
		if (prx + 8 >= cs_bx && prx <= cs_bx + 16 && pry + 16 >= cs_by && pry <= cs_by + 16) {
			if (pkeys) {
				cs_ba = obj_t [rdc] = 0;
				pkeys --;
				sfx_play (SFX_USE_KEY, SC_LEVEL);
				col_idx = cs_bx >> 5;
				scroll_draw_one_chunk_completely ();
				draw_hud ();
			} else {
				if (prx < cs_bx) {
					px = (cs_bx - 8) << FIX_BITS;
					pvx = -PLAYER_VX_MAX;
				} else {
					px = (cs_bx + 16) << FIX_BITS;
					pvx = PLAYER_VX_MAX;
				}
			}
		}
	}
}
