Lights1 bird_blue_lights = gdSPDefLights1(
	0xC, 0x5D, 0x72,
	0x1D, 0x91, 0xB0, 0x28, 0x28, 0x28);

Lights1 bird_white_lights = gdSPDefLights1(
	0x3F, 0x3F, 0x3F,
	0xFF, 0xFF, 0xFF, 0x28, 0x28, 0x28);

Lights1 bird_beak_lights = gdSPDefLights1(
	0x33, 0x27, 0xD,
	0xCE, 0x9D, 0x34, 0x28, 0x28, 0x28);

Vtx bird_body_mesh_layer_1_vtx_0[22] = {
	{{ {-38, -8, -4}, 0, {-16, -16}, {38, 135, 0, 255} }},
	{{ {-20, -2, 0}, 0, {-16, -16}, {38, 135, 0, 255} }},
	{{ {-38, -8, 4}, 0, {-16, -16}, {38, 135, 0, 255} }},
	{{ {-38, -5, 2}, 0, {-16, -16}, {22, 148, 63, 255} }},
	{{ {-22, -3, 0}, 0, {-16, -16}, {22, 148, 63, 255} }},
	{{ {-36, -2, 7}, 0, {-16, -16}, {22, 148, 63, 255} }},
	{{ {-36, -2, -7}, 0, {-16, -16}, {22, 148, 193, 255} }},
	{{ {-22, -3, 0}, 0, {-16, -16}, {22, 148, 193, 255} }},
	{{ {-38, -5, -2}, 0, {-16, -16}, {22, 148, 193, 255} }},
	{{ {17, 1, -10}, 0, {-16, -16}, {61, 24, 147, 255} }},
	{{ {20, -13, -5}, 0, {-16, -16}, {30, 162, 176, 255} }},
	{{ {0, 0, -14}, 0, {-16, -16}, {236, 209, 140, 255} }},
	{{ {28, 0, -2}, 0, {-16, -16}, {76, 56, 170, 255} }},
	{{ {28, -6, 0}, 0, {-16, -16}, {106, 185, 0, 255} }},
	{{ {21, -14, 0}, 0, {-16, -16}, {40, 135, 0, 255} }},
	{{ {20, -13, 5}, 0, {-16, -16}, {30, 162, 80, 255} }},
	{{ {28, 0, 2}, 0, {-16, -16}, {76, 56, 86, 255} }},
	{{ {17, 1, 10}, 0, {-16, -16}, {61, 24, 109, 255} }},
	{{ {0, 0, 14}, 0, {-16, -16}, {236, 209, 116, 255} }},
	{{ {-2, -8, 0}, 0, {-16, -16}, {225, 133, 0, 255} }},
	{{ {-26, -2, 3}, 0, {-16, -16}, {153, 212, 60, 255} }},
	{{ {-26, -2, -3}, 0, {-16, -16}, {153, 212, 196, 255} }},
};

Vtx bird_body_mesh_layer_1_vtx_1[13] = {
	{{ {17, 1, 10}, 0, {-16, -16}, {61, 24, 109, 255} }},
	{{ {2, 12, 14}, 0, {-16, -16}, {3, 72, 105, 255} }},
	{{ {0, 0, 14}, 0, {-16, -16}, {236, 209, 116, 255} }},
	{{ {4, 20, 0}, 0, {-16, -16}, {18, 126, 0, 255} }},
	{{ {17, 8, 0}, 0, {-16, -16}, {78, 101, 0, 255} }},
	{{ {28, 0, 2}, 0, {-16, -16}, {76, 56, 86, 255} }},
	{{ {28, 0, -2}, 0, {-16, -16}, {76, 56, 170, 255} }},
	{{ {17, 1, -10}, 0, {-16, -16}, {61, 24, 147, 255} }},
	{{ {2, 12, -14}, 0, {-16, -16}, {3, 72, 151, 255} }},
	{{ {0, 0, -14}, 0, {-16, -16}, {236, 209, 140, 255} }},
	{{ {-26, -2, -3}, 0, {-16, -16}, {153, 212, 196, 255} }},
	{{ {-22, 5, 0}, 0, {-16, -16}, {166, 90, 0, 255} }},
	{{ {-26, -2, 3}, 0, {-16, -16}, {153, 212, 60, 255} }},
};

Vtx bird_body_mesh_layer_1_vtx_2[4] = {
	{{ {28, -6, 0}, 0, {-16, -16}, {106, 185, 0, 255} }},
	{{ {28, 0, -2}, 0, {-16, -16}, {76, 56, 170, 255} }},
	{{ {36, 0, 0}, 0, {-16, -16}, {125, 21, 0, 255} }},
	{{ {28, 0, 2}, 0, {-16, -16}, {76, 56, 86, 255} }},
};

Vtx bird_left_wing_mesh_layer_1_vtx_0[9] = {
	{{ {13, 4, 2}, 0, {-16, -16}, {0, 127, 252, 255} }},
	{{ {13, -2, -14}, 0, {-16, -16}, {9, 121, 217, 255} }},
	{{ {-1, -2, -16}, 0, {-16, -16}, {249, 123, 227, 255} }},
	{{ {24, 3, 3}, 0, {-16, -16}, {10, 127, 253, 255} }},
	{{ {12, 1, 12}, 0, {-16, -16}, {254, 124, 28, 255} }},
	{{ {7, 2, 4}, 0, {-16, -16}, {238, 126, 5, 255} }},
	{{ {0, 1, 8}, 0, {-16, -16}, {239, 126, 2, 255} }},
	{{ {44, 0, 10}, 0, {-16, -16}, {14, 126, 250, 255} }},
	{{ {30, -1, -8}, 0, {-16, -16}, {12, 123, 226, 255} }},
};

Vtx bird_right_wing_mesh_layer_1_vtx_0[9] = {
	{{ {7, 2, -4}, 0, {-16, -16}, {240, 126, 0, 255} }},
	{{ {-1, -2, 16}, 0, {-16, -16}, {0, 125, 23, 255} }},
	{{ {13, -2, 14}, 0, {-16, -16}, {2, 122, 34, 255} }},
	{{ {0, 1, -8}, 0, {-16, -16}, {239, 126, 254, 255} }},
	{{ {12, 1, -12}, 0, {-16, -16}, {254, 124, 228, 255} }},
	{{ {13, 4, -2}, 0, {-16, -16}, {248, 126, 8, 255} }},
	{{ {30, -1, 8}, 0, {-16, -16}, {15, 122, 32, 255} }},
	{{ {24, 3, -3}, 0, {-16, -16}, {11, 127, 1, 255} }},
	{{ {44, 0, -10}, 0, {-16, -16}, {14, 126, 6, 255} }},
};


Gfx bird_body_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_LIGHTING | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsSPSetLights1(bird_blue_lights),
	gsSPVertex(bird_body_mesh_layer_1_vtx_0 + 0, 22, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
	gsSP2Triangles(10, 9, 12, 0, 12, 13, 10, 0),
	gsSP2Triangles(10, 13, 14, 0, 14, 13, 15, 0),
	gsSP2Triangles(15, 13, 16, 0, 16, 17, 15, 0),
	gsSP2Triangles(18, 15, 17, 0, 15, 18, 19, 0),
	gsSP2Triangles(18, 20, 19, 0, 19, 20, 21, 0),
	gsSP2Triangles(21, 11, 19, 0, 19, 11, 10, 0),
	gsSP2Triangles(10, 14, 19, 0, 19, 14, 15, 0),
	gsDPPipeSync(),
	gsSPSetLights1(bird_white_lights),
	gsSPVertex(bird_body_mesh_layer_1_vtx_1 + 0, 13, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(0, 4, 3, 0, 4, 0, 5, 0),
	gsSP2Triangles(4, 5, 6, 0, 6, 7, 4, 0),
	gsSP2Triangles(3, 4, 7, 0, 7, 8, 3, 0),
	gsSP2Triangles(9, 8, 7, 0, 8, 9, 10, 0),
	gsSP2Triangles(10, 11, 8, 0, 11, 10, 12, 0),
	gsSP2Triangles(1, 11, 12, 0, 12, 2, 1, 0),
	gsSP2Triangles(1, 3, 11, 0, 11, 3, 8, 0),
	gsDPPipeSync(),
	gsSPSetLights1(bird_beak_lights),
	gsSPVertex(bird_body_mesh_layer_1_vtx_2 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
	gsSP1Triangle(3, 0, 2, 0),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

Gfx bird_left_wing_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPSetLights1(bird_blue_lights),
	gsSPVertex(bird_left_wing_mesh_layer_1_vtx_0 + 0, 9, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(0, 4, 3, 0, 5, 4, 0, 0),
	gsSP2Triangles(0, 2, 5, 0, 5, 2, 6, 0),
	gsSP2Triangles(6, 4, 5, 0, 3, 4, 7, 0),
	gsSP2Triangles(7, 8, 3, 0, 3, 8, 1, 0),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

Gfx bird_right_wing_mesh_layer_1[] = {
	gsDPPipeSync(),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_1PRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 3, G_AC_NONE | G_ZS_PIXEL),
	gsSPVertex(bird_right_wing_mesh_layer_1_vtx_0 + 0, 9, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 1, 0, 0),
	gsSP2Triangles(0, 4, 3, 0, 5, 4, 0, 0),
	gsSP2Triangles(0, 2, 5, 0, 5, 2, 6, 0),
	gsSP2Triangles(5, 6, 7, 0, 7, 6, 8, 0),
	gsSP2Triangles(8, 4, 7, 0, 7, 4, 5, 0),
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};

Gfx bird_material_revert_render_settings[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

