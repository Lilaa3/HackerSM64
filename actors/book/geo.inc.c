#include "src/game/envfx_snow.h"

const GeoLayout bookend_geo[] = {
	GEO_CULLING_RADIUS(52),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, book_Book_mesh_layer_1),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
