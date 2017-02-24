// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

#include "assets/sspl.h"
#include "assets/sspl2.h"
#include "assets/sspl3.h"
#include "assets/ssen1.h"
#include "assets/ssen2.h"
#include "assets/ssen3.h"
#include "assets/ssen4.h"
#include "assets/ssending.h"
#include "assets/ssextra.h"
#include "assets/ssextra2.h"
#include "assets/ssmt.h"

// Enemies

// Each enemy is R2 R2 L1 L2, so actual frame is (ent << 2) + (facing [0/2]) + (frame [0/1])

const unsigned char * const spr_enems [] = {
	spr_enems2_00_a, spr_enems2_01_a, spr_enems2_00_b, spr_enems2_01_b,	// Murciélago
	spr_enems1_02_a, spr_enems1_03_a, spr_enems1_02_b, spr_enems1_03_b,	// Guerrero
	spr_enems1_00_a, spr_enems1_01_a, spr_enems1_00_b, spr_enems1_01_b,	// Canino
	spr_enems3_00_a, spr_enems3_01_a, spr_enems3_00_b, spr_enems3_01_b,	// Plataforma
	spr_enems4_00														// Explosion
};

// Player is: (right, then the same thing for left)
// Standing, Run1, Run2, Run3, Run4, Jump, Extra, Dead,

const unsigned char * const spr_player [] = {
	spr_player_00_a, spr_player_01_a, spr_player_02_a, spr_player_03_a, 
	spr_player_04_a, spr_player_05_a, spr_player_05_a, spr_player_06_a,

	spr_player_00_b, spr_player_01_b, spr_player_02_b, spr_player_03_b, 
	spr_player_04_b, spr_player_05_b, spr_player_05_b, spr_player_06_b,

	// Second (hidden) player spriteset:
	spr_player_2_00_a, spr_player_2_01_a, spr_player_2_02_a, spr_player_2_03_a, 
	spr_player_2_04_a, spr_player_2_05_a, spr_player_2_07_a, spr_player_2_06_a,

	spr_player_2_00_b, spr_player_2_01_b, spr_player_2_02_b, spr_player_2_03_b, 
	spr_player_2_04_b, spr_player_2_05_b, spr_player_2_07_b, spr_player_2_06_b,

	// Third (hidden) player spriteset:
	spr_player_3_00_a, spr_player_3_01_a, spr_player_3_02_a, spr_player_3_03_a, 
	spr_player_3_04_a, spr_player_3_05_a, spr_extra2_01_a, spr_player_3_06_a,

	spr_player_3_00_b, spr_player_3_01_b, spr_player_3_02_b, spr_player_3_03_b, 
	spr_player_3_04_b, spr_player_3_05_b, spr_extra2_01_b, spr_player_3_06_b
};

const unsigned char spr_empty [] = {
	0, 0, 0xfe, 0, 0, 0, 0xfe, 0, 0, 0, 0xfe, 0,
	0, 0, 0xfe, 0, 0, 0, 0xfe, 0, 0, 0, 0xfe, 0,
	128
};

const unsigned char spr_hud_base [] = {
	0,  0,   0x51, 1, 		8,  0,   0x54, 0, 			// Corazon x 
	0,  183, 0x52, 1, 		8,  183, 0x54, 0, 			// Ababol x
	40, 183, 0x53, 0, 		48, 183, 0x54, 0, 			// Llaves x
	128
};
