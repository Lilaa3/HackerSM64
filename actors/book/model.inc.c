Lights1 book_pages_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 book_cover_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Gfx book_book_cover_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 book_book_cover_rgba16_rgba16[] = {
	#include "actors/book/book_cover.rgba16.inc.c"
};

Vtx book_Book_mesh_layer_1_vtx_0[8] = {
	{{ {-9, 31, -50}, 0, {496, -16}, {0, 127, 0, 255} }},
	{{ {10, 31, 0}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {10, 31, -50}, 0, {-16, -16}, {0, 127, 0, 255} }},
	{{ {-9, 31, 0}, 0, {496, 1008}, {0, 127, 0, 255} }},
	{{ {10, -30, -50}, 0, {-16, 1008}, {0, 129, 0, 255} }},
	{{ {10, -30, 0}, 0, {-16, -16}, {0, 129, 0, 255} }},
	{{ {-9, -30, 0}, 0, {496, -16}, {0, 129, 0, 255} }},
	{{ {-9, -30, -50}, 0, {496, 1008}, {0, 129, 0, 255} }},
};

Vtx book_Book_mesh_layer_1_vtx_1[20] = {
	{{ {10, 31, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {-9, 31, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-9, -30, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {10, 31, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {-9, -30, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {10, -30, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {10, 31, -50}, 0, {1008, -16}, {127, 0, 0, 255} }},
	{{ {10, 31, 0}, 0, {-16, -16}, {127, 0, 0, 255} }},
	{{ {10, -30, 0}, 0, {-16, 1008}, {127, 0, 0, 255} }},
	{{ {10, 31, -50}, 0, {1008, -16}, {127, 0, 0, 255} }},
	{{ {10, -30, 0}, 0, {-16, 1008}, {127, 0, 0, 255} }},
	{{ {10, -30, -50}, 0, {1008, 1008}, {127, 0, 0, 255} }},
	{{ {10, -30, -50}, 0, {-16, 1008}, {0, 0, 129, 255} }},
	{{ {-9, 31, -50}, 0, {1008, -16}, {0, 0, 129, 255} }},
	{{ {10, 31, -50}, 0, {-16, -16}, {0, 0, 129, 255} }},
	{{ {-9, -30, -50}, 0, {1008, 1008}, {0, 0, 129, 255} }},
	{{ {-9, -30, -50}, 0, {1008, 1008}, {129, 0, 0, 255} }},
	{{ {-9, 31, 0}, 0, {-16, -16}, {129, 0, 0, 255} }},
	{{ {-9, 31, -50}, 0, {1008, -16}, {129, 0, 0, 255} }},
	{{ {-9, -30, 0}, 0, {-16, 1008}, {129, 0, 0, 255} }},
};


Gfx book_Book_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(book_pages_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_cover_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsDPSetTileSize(0, 0, 0, 60, 124),
	gsSPVertex(book_Book_mesh_layer_1_vtx_0 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsDPPipeSync(),
	gsSPSetLights1(book_cover_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, book_book_cover_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPVertex(book_Book_mesh_layer_1_vtx_1 + 0, 20, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 15, 13, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 19, 17, 0),
	gsDPPipeSync(),
	gsDPSetCycleType(G_CYC_1CYCLE),
    // Material revert
    gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};