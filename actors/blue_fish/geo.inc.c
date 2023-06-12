#include "src/game/envfx_snow.h"

const GeoLayout fish_geo[] = {
	GEO_CULLING_RADIUS(42),
	GEO_OPEN_NODE(),
		GEO_ANIMATED_PART(LAYER_OPAQUE, -4, 0, 0, blue_fish_body_mesh_layer_1),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, 24, 0, 0, blue_fish_tail_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, blue_fish_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};

const GeoLayout shadow_fish_geo[] = {
	GEO_CULLING_RADIUS(42),
	GEO_OPEN_NODE(),
		GEO_SHADOW(1, 155, 50),
		GEO_OPEN_NODE(),
			GEO_ANIMATED_PART(LAYER_OPAQUE, -4, 0, 0, blue_fish_body_mesh_layer_1),
			GEO_OPEN_NODE(),
				GEO_ANIMATED_PART(LAYER_OPAQUE, 24, 0, 0, blue_fish_tail_mesh_layer_1),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, blue_fish_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
