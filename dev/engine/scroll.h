// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

/*
	Map is divided in columns. Each column is 25 bytes.

	First 20 bytes are 2x10 metatiles, column-ordered, as in:

	0x00 0x0a
	0x01 0x0b
	0x02 0x0c
	0x03 0x0d
	0x04 0x0e
	0x05 0x0f
	0x06 0x10
	0x07 0x11
	0x08 0x12
	0x09 0x13

	Next 4 bytes are 5 attributes for such metatiles, ordered top->bottom

	Each 16x16 metatile is composed by 2x2 tiles:

 	(t << 2)      (t << 2 + 2)
 	(t << 2 + 1)  (t << 2 + 3)

 	Each frame, 1/4 of the tiles and all of the attributes are sent to 
 	the nametable via NMI update.
*/

void scroll_init (void) {
	col_ctr = 0;

	// Select scroll section
	section_x0 = 0; section_x1 = MAP_W * 32 - 256;
	section = (signed int *) sections [level]; gpit = 1; while (section [gpit] < MAP_W * 32) {
		if (prx < section [gpit]) {
			section_x1 = section [gpit] - 256;
			break;
		} 
		section_x0 = section [gpit];
		gpit ++;
	}
}

void scroll_paint_chunk (void) {
	// Globals needed:
	//	col_idx			- Column index.
	//	map_ptr			- Is map + level offset (points to the beginning of strip)
	//	gp_ul			- Pointer to the update list

	// Calculate memory address of current column [col_idx * 25]
	col_ptr = map_ptr + (col_idx << 4) + (col_idx << 3) + col_idx;

	// VRAM address
	rdb = (col_idx << 2) & 63;		// Column # is directly based upon col_idx!
	rdc = rdb & 31;

	// which nametable?
	if (rdb < 32) {
		col_adr = NAMETABLE_A + (TOP_ADJUST << 5) + rdc;
		col_attr_adr = NAMETABLE_A + ((TOP_ADJUST >> 2) << 3) + 0x03c0 + (rdc >> 2);
	} else {
		col_adr = NAMETABLE_B + (TOP_ADJUST << 5) + rdc;
		col_attr_adr = NAMETABLE_B + ((TOP_ADJUST >> 2) << 3) + 0x03c0 + (rdc >> 2);
	}

	gp_addr = col_adr + col_ctr;

	// subcolumn in chunk
	gp_gen = col_ptr + ((col_ctr >> 1) ? 10 : 0);

	// metatile offset
	col_offs = (col_ctr & 1) << 1;

	// fill in 10 metatiles...

	// starting address
	*gp_ul++ = MSB (gp_addr) | NT_UPD_VERT;
	*gp_ul++ = LSB (gp_addr);

	// 20 sequential writes
	*gp_ul++ = 20;

	// write 1 column of 10 metatiles, unrolled
	rda = *gp_gen ++;					// Read metatile from map
	rda = (rda << 2) + col_offs;		// This is pattern number
	*gp_ul ++ = rda;					// Write fist pattern
	*gp_ul ++ = rda + 1;				// Write next pattern

	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	rda = ((*gp_gen ++) << 2) + col_offs; *gp_ul ++ = rda; *gp_ul ++ = rda + 1;
	
	// Any object here?
	rdx = col_idx >> 3;					// Screen where we are
	rdt = obj_strip_base + (rdx << 1);	// Index to object arrays.
	rda = (col_idx << 1) + (col_ctr >> 1);	// Tile x coord in screen

	rdy = 0;
	if (obj_x [rdt] == rda && !obj_t [rdt]) rdy = obj_y [rdt] << 1;
	if (obj_x [rdt + 1] == rda && !obj_t [rdt + 1]) rdy = obj_y [rdt + 1] << 1;
	if (rdy) {
		*(gp_ul - 20 + rdy) = 0;
		*(gp_ul - 19 + rdy) = 0;
	}

	// And now, the attributes
	if (!col_ctr) {
		gp_addr = col_attr_adr;
		gp_gen = col_ptr + 20;
		scit = 5; while (scit --) {
			*gp_ul ++ = MSB (gp_addr);
			*gp_ul ++ = LSB (gp_addr);
			*gp_ul ++ = *gp_gen ++;
			gp_addr += 8;
		}
	}

	// Next pattern column to process
	col_ctr = (col_ctr + 1) & 3;
}

void scroll_draw_one_chunk_completely (void) {
	col_ctr = 0;
	gp_ul = update_list;
	scroll_paint_chunk ();
	scroll_paint_chunk ();
	
	*gp_ul = NT_UPD_EOF;
	ppu_wait_nmi ();
	gp_ul = update_list;
	
	scroll_paint_chunk ();
	scroll_paint_chunk ();
	*gp_ul = NT_UPD_EOF;
	ppu_wait_nmi ();
}

void scroll_to (void) {
	// Updates nametables upon direction
	if (cam_pos > cam_pos_old) {
		col_idx = (cam_pos >> 5) + 9;
		scroll_paint_chunk ();
	} else if (cam_pos < cam_pos_old) {
		col_idx = (cam_pos >> 5) - 1;
		scroll_paint_chunk ();
	}

	cam_pos_old = cam_pos;
}

void scroll_draw_screen (void) {
	// Redraw
	scroll (cam_pos & 0x01ff, 0);
	//cx1 = (cam_pos >> 5) - 1; cx2 = cx1 + 9;
	// Safer:
	cx1 = (cam_pos >> 5); if (cx1) cx1 --;
	cx2 = cx1 + 10;
	for (col_idx = cx1; col_idx <= cx2; col_idx ++) {
		scroll_draw_one_chunk_completely ();
	}
}
