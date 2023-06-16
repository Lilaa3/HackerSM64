#include "src/game/envfx_snow.h"

const GeoLayout bookend_part_geo[] = {
	GEO_CULLING_RADIUS(48),
	GEO_OPEN_NODE(),
		GEO_SHADOW(0, 150, 100),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, bookend_left_side_mesh_layer_1),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, bookend_right_side_mesh_layer_1),
		GEO_CLOSE_NODE(),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
