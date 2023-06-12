#include "src/game/envfx_snow.h"

const GeoLayout blargg_geo[] = {
	GEO_CULLING_RADIUS(240),
	GEO_OPEN_NODE(),
		GEO_ANIMATED_PART(LAYER_OPAQUE, 0, 0, 0, blargg_lower_body_mesh_layer_1),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 76, 0, 0, blargg_head_mesh_layer_1),
			GEO_OPEN_NODE(),
				GEO_ANIMATED_PART(LAYER_OPAQUE, 1, 1, 0, blargg_jaw_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, blargg_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
