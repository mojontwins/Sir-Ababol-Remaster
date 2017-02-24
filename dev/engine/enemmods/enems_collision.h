	// Iterator is on rdt

	// Only if alive
	if (!(en_t [rdt] & 0x08)) {
		if (en_t [rdt] != 4) {
			if (prx + 4 >= en_x [rdt] && prx <= en_x [rdt] + 12 && pry + 12 >= en_y [rdt] && pry <= en_y [rdt] + 12) {
			
				// Collide from top 
				if (pry + 4 < en_y [rdt] && pvy > 0 && game_mode == GAME_MODE_SIR_ABABOL) {
					pvy = -PLAYER_VY_JUMP_BUTT;
					// Kill enemy.
					enems_kill ();
				} else if (!pflickering) {
					music_stop ();
					sfx_play (SFX_DIE, SC_LEVEL);
					delay (25);
					player_kill ();
				}
			}

			// Hitter
			if (game_mode == GAME_MODE_ZAZELLE) {
				if (hitter_frame >= HIT_F0 && hitter_frame <= HIT_F1) {
					if (hitter_x + 12 >= en_x [rdt] && hitter_x <= en_x [rdt] + 12 &&
						hitter_y + 12 >= en_y [rdt] && hitter_y <= en_y [rdt] + 12) {
						enems_kill ();
					}
				}
			}
		}
	}
