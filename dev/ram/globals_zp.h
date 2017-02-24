// Vostok Engine MK2 v1.0
// Copyleft 2016 by The Mojon Twins

// Pad control
unsigned char pad0, pad;						

// Game counters
unsigned char half_life;
unsigned char frame_counter;
unsigned char keep_going;

// General all-purpose, all-around vars
unsigned char i, gpit, gpjt, scit;			
unsigned char *gp_gen, *gp_aux, *gp_ul;
unsigned int gp_addr;
unsigned char rda, rdb, rdc, rdd;
unsigned char rdx, rdy, rdt, rdct;
signed int rdxi;
unsigned char rdyi;

// Counts ticks
unsigned char ticker, game_time;

// Collision
unsigned char cx1, cy1, cx2, cy2, at1, at2;

// Scroller
unsigned char col_idx; 		// Column to draw (0x00 - 0xff)
unsigned int col_adr;		// VRAM address to draw to (patterns)
unsigned int col_attr_adr;	// VRAM address to draw to (attributes)
unsigned char *col_ptr; 	// Points to current column
unsigned char *map_ptr;		// Contains map + (level offset)
unsigned char col_ctr;		// Column counter, counts 0 1 2 3
unsigned char col_offs;		// Offset in metatile. 0 or 2.

// Camera
signed int cam_pos, cam_pos_old;

// Sprite system
unsigned char oam_index;

// Enems
unsigned char en_rx, en_facing, en_fr, ent, ens, slot;
unsigned char en_jc;
unsigned char orda, enems_half;
signed char delta;

// Teh player
unsigned int px;
signed int prx;
signed int py;
unsigned char pry;
signed char pgtmx, pgtmy; 
unsigned char pgotten, ppossee, phit, pslip;
unsigned char pfacing, pfr, pkilled, pbase;
unsigned char pidlt;
signed int pvx, pvy;
unsigned char pj, pctj, pjb, pbutt;
unsigned char pen, penhit;
unsigned char do_game_loop;
unsigned char pflickering;
unsigned char pfiring, pfree, pfloat;

unsigned char engine_type_v;
unsigned char engine_type_h;

unsigned char safe_level, safe_pry;
signed int safe_prx;

// Current screen object & bolt positions & status
unsigned char cs_oy, cs_ot, cs_by, cs_ba;
unsigned int cs_ox, cs_bx;
unsigned char cs;

// Enemy randoprimer
unsigned char erp_base;
unsigned char erp_iter;

// Hitter
unsigned char hitter_on, hitter_frame, hitter_y;
signed int hitter_x;

// Game mode
unsigned char game_mode;
