	// Iterator is on rdt
	en_rx = LSB (en_x [rdt]);


	#include "enems_linear.h"

	//*((unsigned char *)(en_x + rdt + rdt + 1)) = en_rx;		// I've never been this dirty before! Update LSB.
	en_x [rdt] = en_x [rdt] & 0xff00 | en_rx;
