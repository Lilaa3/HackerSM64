Lights1 bookend_teeth_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 bookend_mouth_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 bookend_pages_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 bookend_case_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 bookend_spine_lights = gdSPDefLights1(
	0x7F, 0x7F, 0x7F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Gfx bookend_bookend_tooth_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 bookend_bookend_tooth_rgba16_rgba16[] = {
	#include "actors/bookend/bookend_tooth.rgba16.inc.c"
};

Gfx bookend_bookend_mouth_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 bookend_bookend_mouth_rgba16_rgba16[] = {
	#include "actors/bookend/bookend_mouth.rgba16.inc.c"
};

Gfx bookend_bookend_pages_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 bookend_bookend_pages_rgba16_rgba16[] = {
	#include "actors/bookend/bookend_pages.rgba16.inc.c"
};

Gfx bookend_bookend_cover_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 bookend_bookend_cover_rgba16_rgba16[] = {
	#include "actors/bookend/bookend_cover.rgba16.inc.c"
};

Gfx bookend_bookend_spine_rgba16_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 bookend_bookend_spine_rgba16_rgba16[] = {
	#include "actors/bookend/bookend_spine.rgba16.inc.c"
};

Vtx bookend_left_side_mesh_layer_1_vtx_0[12] = {
	{{ {39, 0, 0}, 0, {-16, 344}, {130, 238, 0, 255} }},
	{{ {38, 10, -6}, 0, {220, 890}, {130, 238, 0, 255} }},
	{{ {39, 0, -12}, 0, {170, -948}, {130, 238, 0, 255} }},
	{{ {39, 0, -12}, 0, {-10, 224}, {130, 238, 0, 255} }},
	{{ {38, 10, -19}, 0, {230, 616}, {130, 238, 0, 255} }},
	{{ {39, 0, -26}, 0, {166, -1020}, {130, 238, 0, 255} }},
	{{ {39, 0, 12}, 0, {-36, 376}, {130, 238, 0, 255} }},
	{{ {38, 10, 6}, 0, {212, 836}, {130, 238, 0, 255} }},
	{{ {39, 0, 0}, 0, {166, -580}, {130, 238, 0, 255} }},
	{{ {39, 0, 25}, 0, {0, 378}, {130, 238, 0, 255} }},
	{{ {38, 10, 18}, 0, {197, 758}, {130, 238, 0, 255} }},
	{{ {39, 0, 12}, 0, {191, -738}, {130, 238, 0, 255} }},
};

Vtx bookend_left_side_mesh_layer_1_vtx_1[4] = {
	{{ {-2, 0, 26}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {40, 0, 26}, 0, {496, 1008}, {0, 127, 0, 255} }},
	{{ {40, 0, -26}, 0, {496, -16}, {0, 127, 0, 255} }},
	{{ {-2, 0, -26}, 0, {-16, -16}, {0, 127, 0, 255} }},
};

Vtx bookend_left_side_mesh_layer_1_vtx_2[16] = {
	{{ {40, 0, 26}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2, 0, 26}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {-1, -10, 26}, 0, {496, 1004}, {0, 0, 127, 255} }},
	{{ {40, 0, 26}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-1, -10, 26}, 0, {496, 1004}, {0, 0, 127, 255} }},
	{{ {42, -10, 26}, 0, {496, -12}, {0, 0, 127, 255} }},
	{{ {-2, 0, -26}, 0, {-16, -16}, {0, 0, 129, 255} }},
	{{ {40, 0, -26}, 0, {-16, 1008}, {0, 0, 129, 255} }},
	{{ {42, -10, -26}, 0, {496, 1004}, {0, 0, 129, 255} }},
	{{ {-2, 0, -26}, 0, {-16, -16}, {0, 0, 129, 255} }},
	{{ {42, -10, -26}, 0, {496, 1004}, {0, 0, 129, 255} }},
	{{ {-1, -10, -26}, 0, {496, -12}, {0, 0, 129, 255} }},
	{{ {40, 0, -26}, 0, {-16, -12}, {122, 35, 0, 255} }},
	{{ {40, 0, 26}, 0, {-16, 1008}, {122, 35, 0, 255} }},
	{{ {42, -10, 26}, 0, {496, 1004}, {122, 35, 0, 255} }},
	{{ {42, -10, -26}, 0, {496, -12}, {122, 35, 0, 255} }},
};

Vtx bookend_left_side_mesh_layer_1_vtx_3[4] = {
	{{ {-1, -10, -27}, 0, {496, -16}, {0, 129, 0, 255} }},
	{{ {46, -10, 27}, 0, {-16, 1008}, {0, 129, 0, 255} }},
	{{ {-1, -10, 27}, 0, {496, 1008}, {0, 129, 0, 255} }},
	{{ {46, -10, -27}, 0, {-16, -16}, {0, 129, 0, 255} }},
};

Vtx bookend_left_side_mesh_layer_1_vtx_4[4] = {
	{{ {-2, 0, 27}, 0, {-16, 1008}, {130, 237, 0, 255} }},
	{{ {-1, -10, -27}, 0, {496, -16}, {130, 237, 0, 255} }},
	{{ {-1, -10, 27}, 0, {496, 1008}, {130, 237, 0, 255} }},
	{{ {-2, 0, -27}, 0, {-16, -16}, {130, 237, 0, 255} }},
};

Vtx bookend_right_side_mesh_layer_1_vtx_0[4] = {
	{{ {-2, 0, 26}, 0, {-16, -16}, {0, 127, 0, 255} }},
	{{ {40, 0, -26}, 0, {496, 1008}, {0, 127, 0, 255} }},
	{{ {-2, 0, -26}, 0, {-16, 1008}, {0, 127, 0, 255} }},
	{{ {40, 0, 26}, 0, {496, -16}, {0, 127, 0, 255} }},
};

Vtx bookend_right_side_mesh_layer_1_vtx_1[16] = {
	{{ {-1, -10, 26}, 0, {496, 1004}, {0, 0, 127, 255} }},
	{{ {40, 0, 26}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {-2, 0, 26}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {42, -10, -26}, 0, {496, 1004}, {0, 0, 129, 255} }},
	{{ {-1, -10, -26}, 0, {496, -12}, {0, 0, 129, 255} }},
	{{ {-2, 0, -26}, 0, {-16, -16}, {0, 0, 129, 255} }},
	{{ {42, -10, -26}, 0, {496, 1004}, {0, 0, 129, 255} }},
	{{ {-2, 0, -26}, 0, {-16, -16}, {0, 0, 129, 255} }},
	{{ {40, 0, -26}, 0, {-16, 1008}, {0, 0, 129, 255} }},
	{{ {-1, -10, 26}, 0, {496, 1004}, {0, 0, 127, 255} }},
	{{ {42, -10, 26}, 0, {496, -12}, {0, 0, 127, 255} }},
	{{ {40, 0, 26}, 0, {-16, -16}, {0, 0, 127, 255} }},
	{{ {42, -10, 26}, 0, {496, 1004}, {122, 35, 0, 255} }},
	{{ {42, -10, -26}, 0, {496, -12}, {122, 35, 0, 255} }},
	{{ {40, 0, -26}, 0, {-16, -12}, {122, 35, 0, 255} }},
	{{ {40, 0, 26}, 0, {-16, 1008}, {122, 35, 0, 255} }},
};

Vtx bookend_right_side_mesh_layer_1_vtx_2[12] = {
	{{ {39, 0, 26}, 0, {17, 286}, {130, 238, 0, 255} }},
	{{ {38, 10, 20}, 0, {232, 574}, {130, 238, 0, 255} }},
	{{ {39, 0, 13}, 0, {215, -978}, {130, 238, 0, 255} }},
	{{ {39, 0, 0}, 0, {37, 374}, {130, 238, 0, 255} }},
	{{ {38, 10, -6}, 0, {216, 620}, {130, 238, 0, 255} }},
	{{ {39, 0, -12}, 0, {214, -966}, {130, 238, 0, 255} }},
	{{ {39, 0, 13}, 0, {-43, 336}, {130, 238, 0, 255} }},
	{{ {38, 10, 6}, 0, {223, 554}, {130, 238, 0, 255} }},
	{{ {39, 0, 0}, 0, {207, -994}, {130, 238, 0, 255} }},
	{{ {39, 0, -12}, 0, {-13, 342}, {130, 238, 0, 255} }},
	{{ {38, 10, -18}, 0, {222, 608}, {130, 238, 0, 255} }},
	{{ {39, 0, -25}, 0, {170, -1018}, {130, 238, 0, 255} }},
};

Vtx bookend_right_side_mesh_layer_1_vtx_3[4] = {
	{{ {-1, -10, -27}, 0, {-16, 1008}, {0, 129, 0, 255} }},
	{{ {46, -10, 28}, 0, {1008, -16}, {0, 129, 0, 255} }},
	{{ {-1, -10, 28}, 0, {-16, -16}, {0, 129, 0, 255} }},
	{{ {46, -10, -27}, 0, {1008, 1008}, {0, 129, 0, 255} }},
};

Vtx bookend_right_side_mesh_layer_1_vtx_4[4] = {
	{{ {-1, -10, -27}, 0, {496, 1008}, {130, 237, 0, 255} }},
	{{ {-1, -10, 28}, 0, {496, -16}, {130, 237, 0, 255} }},
	{{ {-2, 0, 28}, 0, {-16, -16}, {130, 237, 0, 255} }},
	{{ {-2, 0, -27}, 0, {-16, 1008}, {130, 237, 0, 255} }},
};


Gfx bookend_left_side_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, SHADE, 0, 0, 0, 0, PRIMITIVE),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsSPSetLights1(bookend_teeth_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_tooth_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 4, 0),
	gsDPSetTileSize(0, 0, 0, 60, 124),
	gsSPVertex(bookend_left_side_mesh_layer_1_vtx_0 + 0, 12, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING),
	gsSPSetLights1(bookend_mouth_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_mouth_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsSPVertex(bookend_left_side_mesh_layer_1_vtx_1 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsDPPipeSync(),
	gsSPSetLights1(bookend_pages_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_pages_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsSPVertex(bookend_left_side_mesh_layer_1_vtx_2 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsDPPipeSync(),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING),
	gsSPSetLights1(bookend_case_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_cover_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPVertex(bookend_left_side_mesh_layer_1_vtx_3 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsDPPipeSync(),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING),
	gsSPSetLights1(bookend_spine_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_spine_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsDPSetTileSize(0, 0, 0, 60, 124),
	gsSPVertex(bookend_left_side_mesh_layer_1_vtx_4 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsDPPipeSync(),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPEndDisplayList(),
};

Gfx bookend_right_side_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPSetLights1(bookend_mouth_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_mouth_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsSPVertex(bookend_right_side_mesh_layer_1_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsDPPipeSync(),
	gsSPSetLights1(bookend_pages_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_pages_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsSPVertex(bookend_right_side_mesh_layer_1_vtx_1 + 0, 16, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, SHADE, 0, 0, 0, 0, PRIMITIVE),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_LIGHTING),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsSPSetLights1(bookend_teeth_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_tooth_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 4, 0),
	gsSPVertex(bookend_right_side_mesh_layer_1_vtx_2 + 0, 12, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE, TEXEL0, 0, SHADE, 0, 0, 0, 0, SHADE),
	gsSPSetLights1(bookend_case_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_cover_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPVertex(bookend_right_side_mesh_layer_1_vtx_3 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
	gsDPPipeSync(),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING),
	gsSPSetLights1(bookend_spine_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, bookend_bookend_spine_rgba16_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 512),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0),
	gsDPSetTileSize(0, 0, 0, 60, 124),
	gsSPVertex(bookend_right_side_mesh_layer_1_vtx_4 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
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

