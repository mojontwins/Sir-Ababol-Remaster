// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

// 50 or 60 depending on TV system
unsigned char ticks;		
unsigned char keymask;		

// Update list for VBlank updates
static unsigned char update_list [UPDATE_LIST_SIZE * 3 + 1];

// Fade in / fade out
signed char fader;

// Level
unsigned char level;
unsigned char game_res;

// Attributes table buffer
unsigned char attr_table [64];

// Enems
unsigned char en_t [MAX_ENEMS];
unsigned char en_r [MAX_ENEMS];
signed int en_x [MAX_ENEMS];
unsigned char en_y [MAX_ENEMS];
signed char en_mx [MAX_ENEMS], en_my [MAX_ENEMS];
unsigned char en_cell;
unsigned char en_slot [MAX_ENEMS];

// Player
unsigned char plives, pobjs, pkeys;

// I could calculate these from the values in ROM in runtime
// but I need to save every cycle and there's plenty of free RAM.
unsigned char en_x1 [MAX_ENEMS], en_y1 [MAX_ENEMS];
unsigned char en_x2 [MAX_ENEMS], en_y2 [MAX_ENEMS];

unsigned char en_n_pant, en_ct_aux;

unsigned char do_game;

// Objects/locks. Interlaced. even is lock, odd is object.
// 0 equals none; deactivated. 1 refill 2 item 3 key / 1 lock
unsigned char obj_x [MAP_W / 8 * MAP_H * 2];
unsigned char obj_y [MAP_W / 8 * MAP_H * 2];
unsigned char obj_t [MAP_W / 8 * MAP_H * 2];
unsigned char obj_strip_base;

// Current scroll section
signed int *section;
signed int section_x0, section_x1;

// Music flag
unsigned char music_flag;
