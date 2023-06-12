Lights1 blue_fish_fish_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Gfx blue_fish_blue_fish_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 blue_fish_blue_fish_rgba16[] = {
	#include "actors/blue_fish/blue_fish.rgba16.inc.c"
};

Vtx blue_fish_body_mesh_layer_1_vtx_0[14] = {
	{{ {-27, 0, 0}, 0, {0, 608}, {131, 0, 236, 255} }},
	{{ {-11, -3, -8}, 0, {272, 744}, {234, 136, 222, 255} }},
	{{ {-16, -5, 3}, 0, {196, 540}, {226, 133, 12, 255} }},
	{{ {-12, 0, -16}, 0, {260, 892}, {198, 255, 143, 255} }},
	{{ {-11, 4, -8}, 0, {272, 744}, {233, 120, 221, 255} }},
	{{ {-16, 5, 3}, 0, {196, 540}, {225, 123, 13, 255} }},
	{{ {-24, 0, 6}, 0, {48, 496}, {155, 0, 78, 255} }},
	{{ {14, 0, 32}, 0, {696, -8}, {62, 0, 111, 255} }},
	{{ {10, -3, 14}, 0, {640, 320}, {63, 150, 31, 255} }},
	{{ {12, -3, -11}, 0, {676, 776}, {61, 150, 223, 255} }},
	{{ {16, 0, -23}, 0, {728, 1000}, {76, 0, 154, 255} }},
	{{ {12, 4, -11}, 0, {676, 776}, {61, 106, 222, 255} }},
	{{ {10, 4, 14}, 0, {640, 320}, {63, 106, 32, 255} }},
	{{ {25, 0, 0}, 0, {896, 560}, {127, 0, 0, 255} }},
};

Vtx blue_fish_tail_mesh_layer_1_vtx_0[9] = {
	{{ {0, 0, -1}, 0, {1008, 384}, {138, 5, 209, 255} }},
	{{ {17, 1, -7}, 0, {688, 616}, {87, 90, 17, 255} }},
	{{ {17, 0, -15}, 0, {720, 784}, {38, 254, 135, 255} }},
	{{ {13, 0, 0}, 0, {728, 444}, {124, 244, 230, 255} }},
	{{ {17, 1, 7}, 0, {628, 312}, {87, 90, 239, 255} }},
	{{ {0, 0, 1}, 0, {1000, 336}, {135, 246, 36, 255} }},
	{{ {17, -1, 7}, 0, {628, 312}, {89, 167, 237, 255} }},
	{{ {17, -1, -7}, 0, {688, 616}, {88, 166, 18, 255} }},
	{{ {17, 0, 14}, 0, {596, 144}, {39, 255, 121, 255} }},
};


Gfx blue_fish_body_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(blue_fish_fish_lights),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, blue_fish_blue_fish_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPVertex(blue_fish_body_mesh_layer_1_vtx_0 + 0, 14, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(3, 0, 4, 0, 4, 0, 5, 0),
	gsSP2Triangles(0, 6, 5, 0, 2, 6, 0, 0),
	gsSP2Triangles(6, 2, 7, 0, 2, 8, 7, 0),
	gsSP2Triangles(9, 8, 2, 0, 1, 9, 2, 0),
	gsSP2Triangles(10, 9, 1, 0, 1, 3, 10, 0),
	gsSP2Triangles(10, 3, 4, 0, 4, 11, 10, 0),
	gsSP2Triangles(11, 4, 5, 0, 11, 5, 12, 0),
	gsSP2Triangles(7, 12, 5, 0, 5, 6, 7, 0),
	gsSP2Triangles(12, 7, 8, 0, 8, 13, 12, 0),
	gsSP2Triangles(9, 13, 8, 0, 11, 13, 9, 0),
	gsSP2Triangles(11, 9, 10, 0, 13, 11, 12, 0),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

Gfx blue_fish_tail_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPVertex(blue_fish_tail_mesh_layer_1_vtx_0 + 0, 9, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(0, 4, 3, 0, 0, 5, 4, 0),
	gsSP2Triangles(0, 6, 5, 0, 3, 6, 0, 0),
	gsSP2Triangles(0, 7, 3, 0, 2, 7, 0, 0),
	gsSP2Triangles(7, 2, 1, 0, 3, 7, 1, 0),
	gsSP2Triangles(4, 6, 3, 0, 4, 8, 6, 0),
	gsSP2Triangles(5, 8, 4, 0, 5, 6, 8, 0),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

Gfx blue_fish_material_revert_render_settings[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

