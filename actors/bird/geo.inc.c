#include "src/game/envfx_snow.h"

const GeoLayout birds_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_SHADOW(0, 150, 100),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, bird_body_mesh_layer_1),
			GEO_OPEN_NODE(),
				GEO_ANIMATED_PART(LAYER_OPAQUE, 0, -3, 9, bird_left_wing_mesh_layer_1),
				GEO_ANIMATED_PART(LAYER_OPAQUE, 0, -3, -9, bird_right_wing_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, bird_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
