#include "src/game/envfx_snow.h"

const GeoLayout pillar_geo[] = {
	GEO_CULLING_RADIUS(400),
	GEO_OPEN_NODE(),
		GEO_ASM(0, geo_pillar_puzzle),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, pillar_Pillar_mesh_layer_1),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, pillar_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
