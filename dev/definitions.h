// Vostok Engine MK2 v1.0
// Copyleft 2016 by The Mojon Twins

#define COMM_POOL				((unsigned char*)0x0300)	// Mapper 113 stuff.

#define UPDATE_LIST_SIZE		32
#define MSB(x)					((x)>>8)
#define LSB(x)					((x)&0xff)
#define SPRITE_ADJUST			-1
#define TOP_ADJUST				4
#define FIX_BITS				4
#define FADE_DELAY 				1

#define SGNI(n)					((n)<0?-1:1)
#define SGNC(a,b,c)				((a)<(b)?-(c):(c))
#define SATURATE_Z(n)			((n)<0?0:(n))
#define SATURATE(v,max) 		((v)>=0?((v)>(max)?(max):(v)):((v)<-(max)?-(max):(v)))
#define ABS(n)					((n)<0?-(n):(n))
#define DELTA(a,b)				((a)<(b)?(b)-(a):(a)-(b))
#define CL(x1,y1,x2,y2)			((x1)+4>=(x2)&&(x1)<=(x2)+12&&(y1)+12>=(y2)&&(y1)<=(y2)+12)
#define UPDATE					update_list [update_list_index ++]
#define SUBSTR_SAT(a,b)			((a)>(b)?(a)-(b):0)

// Map

#define COLUMN_SIZE				25
#define MAP_W					128		// In chunks
#define MAP_H					3 		// In screens

// Enemies

#define MAX_ENEMS				48
#define ENEM_SPRITE_SLOTS		6
#define ENEM_PROCESS_MAX		6		// 3 enemies per virtual "screen"
#define ENEMS_PER_SCREEN		3 		// I know it's not needed, but I like to read.
#define SLOT_EMPTY				0xff
#define NO_SLOT					0xff

// Engine types

#define ENGINE_TYPE_V_JUMP		0
#define ENGINE_TYPE_V_JETP		1
#define ENGINE_TYPE_V_CHOC		2

#define ENGINE_TYPE_H_WALK		0

// Animation cells

#define PCELL_STANDING			0
#define PCELL_WALK_BASE			1
#define PCELL_JUMPING			5
#define PCELL_SPECIAL 			6
#define PCELL_KILLED			7

// Player movement values

#define PLAYER_VY_FALLING_MAX	64
#define PLAYER_VY_FALLING_MAX_FLOAT	8
#define PLAYER_G				4
#define PLAYER_G_FLOAT			1
#define PLAYER_VY_JUMP_INITIAL	16
#define PLAYER_VY_JUMP_MAX		64	
#define PLAYER_VY_JUMP_BUTT		32
#define PLAYER_VY_JUMP_A_STEPS  16
#define PLAYER_AY_JUMP 			8	
#define PLAYER_VX_MAX			32	
#define PLAYER_AX				4	
#define PLAYER_RX				4	
#define PLAYER_AX_SLIP			1	
#define PLAYER_RX_SLIP			1	
#define PLAYER_VX_MIN (PLAYER_AX << 2)
#define PLAYER_V_REBOUND		56

// Facing

#define PFACING_LEFT 		8
#define PFACING_RIGHT		0

// Camera

#define CAMERA_MIN 			96
#define CAMERA_MAX			160

// Screen times in seconds

#define ENDING_SCREEN_TIME		10
#define GAME_OVER_SCREEN_TIME	10
#define LEVEL_SCREEN_TIME		10
#define UMBRELLA_SCREEN_TIME	10
#define THE_END_SCREEN_TIME		60

// Sound priorities

#define SC_PLAYER				0
#define SC_ENEMS				2
#define SC_LEVEL				1

// SFX

#define SFX_JUMP				7
#define SFX_START				0
#define SFX_DIE					4
#define SFX_SINK				5
#define SFX_STEP_OVER_ENEMY		6
#define SFX_USE_KEY				1
#define SFX_EXTRA_LIFE			2
#define SFX_OBJECT				3

// OGT

#define MUSIC_INGAME			0
#define MUSIC_MENU				1
#define MUSIC_DIE				2
#define MUSIC_GAME_OVER			3
#define MUSIC_ENDING			4

// Psignal

#define PLAYER_EXIT_TOP			1
#define PLAYER_EXIT_BOTTOM 		2
#define PLAYER_ONE_LIFE_LESS	3
#define PLAYER_WINS_GAME		4

// Stuff

#define PLAYER_MAX_OBJECTS		24
#define PLAYER_INITIAL_LIVES	5

// OAM fixed positions

#define OAM_FIXED_PLAYER		4
#define OAM_FIXED_HUD			24+OAM_FIXED_PLAYER
#define OAM_FIXED_VALUES		24+OAM_FIXED_HUD
#define OAM_FIXED_UMBRELLA		16+OAM_FIXED_VALUES

// Game modes

#define GAME_MODE_SIR_ABABOL	0
#define GAME_MODE_ZAZELLE		1
#define GAME_MODE_LIMITE 		2
