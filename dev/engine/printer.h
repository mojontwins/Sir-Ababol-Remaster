// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

void cls (void) {
	vram_adr (NAMETABLE_A);
	vram_fill (0, 2048);		// Both Nametables A & B (0x2000, 0x2400)
}

void fade_out (void) { fader = 5; while (fader --) { pal_bright (fader); ppu_wait_nmi (); } }
	
void fade_in (void) { fader = 5; while (fader --) { pal_bright (4 - fader); ppu_wait_nmi (); } }

void reset_attributes (void) {
	gpit = 64; while (gpit --) attr_table [gpit] = 0;
}

void draw_tile (void) {
	// Draws metatile rdt at rdx, rdy. rdx = 0->31, metatile coordinates
	gp_addr = (rdx > 15 ? NAMETABLE_B : NAMETABLE_A) + ((rdx & 15) << 1) + (rdy << 6);
	rdct = rdt + 16;
	rdc = rdct << 2;
	vram_adr (gp_addr); vram_put (rdc ++); vram_put (rdc ++);
	vram_adr (gp_addr + 1); vram_put (rdc ++); vram_put (rdc);
	rdc = (rdx >> 1) + ((rdy >> 1) << 3);
	rdb = (rdx & 1) + ((rdy & 1) << 1);
	rda = (attr_table [rdc] & bitmasks [rdb]) | (ts_pals [rdct - 16] << (rdb << 1));
	attr_table [rdc] = rda;
}

void p_s (unsigned char x, unsigned char y, unsigned char *s) {
	vram_adr (NAMETABLE_A + (y << 5) + x);
	while (rda = *s ++) vram_put (rda - 32);
}

void draw_hud (void) {
	gpit = plives < 10 ? plives : 9;
	oam_spr (32, 16, 0x55 + gpit, 0, 52);
	oam_spr (32, 199, 0x55 + (pobjs / 10), 0, 56);
	oam_spr (40, 199, 0x55 + (pobjs % 10), 0, 60);
	oam_spr (72, 199, 0x55 + pkeys, 0, 64);
}

void un_rle_screen (unsigned char *rle) {
	rdx = 0; rdy = 0; 
	gpit = 240;
	vram_inc (1);
	while (gpit) {
		rdt = (*rle);
		rle ++;
		if (rdt & 128) {
			gpjt = rdt & 127;
			rdt = (*rle);
			rle ++;
			while (gpjt --) {
				draw_tile ();
				rdx = (rdx + 1) & 15; if (!rdx) rdy ++;
				gpit --;
			}
		} else {
			draw_tile ();
			rdx = (rdx + 1) & 15; if (!rdx) rdy ++;
			gpit --;
		}
	}
	vram_inc (0);
	vram_adr (0x23c0);
	vram_write (rle, 64);
}
