// Sillyscrolly Engine 
// Copyleft 2016 by The Mojon Twins

const unsigned char bitmasks [] = { 0xfc, 0xf3, 0xcf, 0x3f };

const unsigned char behs [] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 4, 8, 8, 0, 0, 0, 0, 0, 0, 0, 4, 8, 1, 8, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 
};

const char jitter [] = {
	0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1
};

const signed int sections_level_0 [] = {0, 4096};
const signed int sections_level_1 [] = {0, 480, 1008, 4096};
//const signed int sections_level_2 [] = {0, 4096}; // me lo ahorro, es igual que el 0.
const signed int * const sections [] = { 
	sections_level_0, sections_level_1, /*sections_level_2*/ sections_level_0
};
