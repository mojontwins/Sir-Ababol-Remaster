// Vostok Engine MK2 v1.0
// Copyleft 2016 by The Mojon Twins

void player_init (void) {
	prx = px >> FIX_BITS;
	pry = py >> FIX_BITS;
	pgotten = 0; ppossee = 0; pkilled = 0;
	phit = penhit = 0;
	pfacing = 0; pfr = 0;
	pj = pjb = 0;
	do_game_loop = 1;
	hitter_on = 0;
	pfloat = 0;
}

void player_kill () {
	if (game_mode == GAME_MODE_ZAZELLE) hitter_hide ();

	music_play (MUSIC_DIE);

	keep_going = 200;
	rdxi = prx - cam_pos;
	rdyi = pry + SPRITE_ADJUST;
	while (rdyi --) {
		-- keep_going; 
		oam_meta_spr (
			rdxi, rdyi,
			OAM_FIXED_PLAYER,
			spr_player [pbase + PCELL_KILLED]
		);
		ppu_wait_nmi ();
	}
	oam_meta_spr (0, 240, OAM_FIXED_PLAYER, spr_empty);
	
	game_res = PLAYER_ONE_LIFE_LESS;
}

void player_sink () {
	if (game_mode == GAME_MODE_ZAZELLE) hitter_hide ();

	music_stop ();
	rdxi = prx - cam_pos;
	rdyi = pry + SPRITE_ADJUST;
	pry = rdyi + 16;
	while (pry > rdyi ++) {
		oam_meta_spr (
			rdxi, rdyi,
			OAM_FIXED_PLAYER,
			spr_player [pbase + pfacing]
		);
		if (rdyi & 1) sfx_play (SFX_SINK, SC_LEVEL);
		ppu_wait_nmi (); ppu_wait_nmi ();
	}
	sfx_play (SFX_DIE, SC_LEVEL);
	delay (25);
}

void player_move (void) {
	if (pflickering) pflickering --;

	// ========
	// Vertical
	// ========

	// Gravity
	if (!pgotten) {
		if (pfloat) {
			if (pvy < PLAYER_VY_FALLING_MAX_FLOAT) pvy += PLAYER_G_FLOAT; else pvy = PLAYER_VY_FALLING_MAX_FLOAT;
		} else {
			if (pvy < PLAYER_VY_FALLING_MAX) pvy += PLAYER_G; else pvy = PLAYER_VY_FALLING_MAX;
		}
	}

	// Move
	py = py + pvy;

	if (py < 0) { if (level) { game_res = PLAYER_EXIT_TOP; return; } else py = 0; }
	if (py > 3072) { game_res = PLAYER_EXIT_BOTTOM; return; }
	
	pry = py >> FIX_BITS;
	
	// Collide
	// cm_two_points handles 2-point collision. Just give two points 
	// (cx1, cy1), (cx2, cy2), and then check at1 and at2

	cx1 = prx >> 4;
	cx2 = (prx + 7) >> 4;

	if (pvy + pgtmy < 0) {
		cy1 = cy2 = pry >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pgotten = pvy = 0;
		pry = (cy1 + 1) << 4;
			py = pry << FIX_BITS;
		} 
	} else if (pvy + pgtmy > 0 && ((pry - 1) & 15) < 4) {
		cy1 = cy2 = (pry + 15) >> 4;
		cm_two_points ();
		if ((at1 & 12) || (at2 & 12)) {
			pgotten = pvy = 0;
			pry = (cy1 - 1) << 4;
			py = pry << FIX_BITS;
		} else if ((at1 & 1) || (at2 & 1)) {
			if (pflickering) {
				pvy = -PLAYER_VY_JUMP_MAX;
				sfx_play (SFX_STEP_OVER_ENEMY, SC_LEVEL); //!
			} else {
				player_sink ();
				player_kill ();
			}
		}
	}
	cy1 = cy2 = (pry + 16) >> 4;
	cm_two_points ();
	ppossee = ((at1 & 12) || (at2 & 12));
	pslip = (ppossee && ((at1 & 16) || (at2 & 16)));

	// Switchable engines
	switch (engine_type_v) {
		case ENGINE_TYPE_V_JUMP:
			if (pad0 & PAD_A) {
				if (!pjb) {
					pjb = 1;
					if (!pj) {
						if (pgotten || ppossee || phit) {
							pj = 1; pctj = 0; 
							pvy = -PLAYER_VY_JUMP_INITIAL;
							sfx_play (SFX_JUMP, SC_PLAYER);

							// Safe:
							if (ppossee) {
								safe_level = level;
								safe_prx = prx;
								safe_pry = pry;
							}
						}
					} 
				}
				if (pj) {
					rda = PLAYER_AY_JUMP - (pctj >> 2) - (pctj >> 3);
					pvy -= (rda > 1 ? rda : 1);
					if (pvy < -PLAYER_VY_JUMP_MAX) pvy = -PLAYER_VY_JUMP_MAX;
					pctj ++; if (pctj == PLAYER_VY_JUMP_A_STEPS) pj = 0;
				}
			} else {
				pjb = 0;
				pj = 0;
			}

			break;
		case ENGINE_TYPE_V_JETP:
			break;
		case ENGINE_TYPE_V_CHOC:
			break;
	}

	// Insert this here...

	if (game_mode == GAME_MODE_ZAZELLE) {
		if (pad0 & PAD_B) {
			if (!pfiring) {
				if (!hitter_on) {
					hitter_on = 1; hitter_frame = 0;
					sfx_play (SFX_SINK, SC_PLAYER);
				}
				pfiring = 1;
			}
		} else pfiring = 0;
	}

	pfree = !(hitter_on && ppossee);

	// ==========
	// Horizontal
	// ==========

	if ((pad0 & PAD_LEFT) && pfree) {
		pfacing = PFACING_LEFT;
		if (pvx > -PLAYER_VX_MAX) pvx -= (pslip ? PLAYER_AX_SLIP : PLAYER_AX);
	} else if ((pad0 & PAD_RIGHT) && pfree) {
		pfacing = PFACING_RIGHT;
		if (pvx < PLAYER_VX_MAX) pvx += (pslip ? PLAYER_AX_SLIP : PLAYER_AX);
	} else {
		if (pvx > 0) {
			pvx -= (pslip ? PLAYER_RX_SLIP : PLAYER_RX); if (pvx < 0) pvx = 0;
		} else if (pvx < 0) {
			pvx += (pslip ? PLAYER_RX_SLIP : PLAYER_RX); if (pvx > 0) pvx = 0;
		}
	}

	// Move
	px += pvx;
	px += pgtmx;
	if (px > 0xff80) px = 0;				// Negative values. Cheesy but seems to work.
	if (px > 0xff00) px = 0xff00;

	prx = px >> FIX_BITS;
	
	// Collide
	cy1 = pry >> 4;
	cy2 = (pry + 15) >> 4;
	if (pvx + pgtmx < 0) {
		cx1 = cx2 = prx >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0;
			prx = (cx1 + 1) << 4;
			px = prx << FIX_BITS;
		}
	} else if (pvx + pgtmx > 0) {
		cx1 = cx2 = (prx + 7) >> 4;
		cm_two_points ();
		if ((at1 & 8) || (at2 & 8)) {
			pvx = 0;
			prx = ((cx1 - 1) << 4) + 8;
			px = prx << FIX_BITS;
		}
	}

	// More so
	// ...

	if (game_mode == GAME_MODE_LIMITE) {
		if (!ppossee && !pj && (pad0 & PAD_B)) {
			if (!pfloat) pvy = 0;
			pfloat = 1;
		} else pfloat = 0;
	}

	// Calc cell
	if (hitter_on) {
		pfr = PCELL_SPECIAL;
	} else if (ppossee || pgotten) {
		if (ABS (pvx) > PLAYER_VX_MIN) {
			pfr = PCELL_WALK_BASE + ((prx >> 4) & 3);
		} else pfr = PCELL_STANDING;
	} else if (pvy != 0) {
		if (pfloat) {
			pfr = PCELL_SPECIAL;
		} else {
			pfr = PCELL_JUMPING;
		}
	} 

	// Move sprite
	rdxi = prx - cam_pos;
	rdyi = pry + SPRITE_ADJUST;

	if (rdxi < 4 || rdxi > 244) rdyi = 240;

	if (!game_res) oam_meta_spr (
		rdxi, rdyi,
		OAM_FIXED_PLAYER,
		(!pflickering || half_life) ? spr_player [pbase + pfacing + pfr] : spr_empty
	);
}
