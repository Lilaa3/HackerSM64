static const s16 bookend_anim_biting_values[21] = {
	0,      -16383, 1820,   3471,   6539,   8190,   6539,   
	3471,   0,      16382,  30039,  28623,  25994,  24578,  
	25994,  28623,  -1,     -1,     -1,     -2,     -1,     
	
};

static const u16 bookend_anim_biting_indicies[18] = {
	1,  0,  1,  0,  1,  0,  1,  1,  6,  2,  1,  8,  1,  9,  6,  
	10, 5,  16, 
};

static const struct Animation bookend_anim_biting = {
	0,
	0,
	0,
	0,
	6,
	2,
	bookend_anim_biting_values,
	bookend_anim_biting_indicies,
	0,
};

