Gfx exclamation_mark_exclamation_mark_rgba16_aligner[] = {gsSPEndDisplayList()};
u8 exclamation_mark_exclamation_mark_rgba16[] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x81, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf9, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf9, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf9, 0xc1, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfa, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfa, 0xc1, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfb, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfb, 0xc1, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfd, 0x41, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfd, 0xc1, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	
};

Vtx exclamation_mark_Cube_mesh_layer_4_vtx_0[4] = {
	{{ {12, 0, 0}, 0, {193, 481}, {0, 0, 127, 255} }},
	{{ {12, 98, 0}, 0, {193, 32}, {0, 0, 127, 255} }},
	{{ {-12, 98, 0}, 0, {63, 32}, {0, 0, 127, 255} }},
	{{ {-12, 0, 0}, 0, {63, 481}, {0, 0, 127, 255} }},
};

Gfx exclamation_mark_Cube_mesh_layer_4_tri_0[] = {
	gsSPVertex(exclamation_mark_Cube_mesh_layer_4_vtx_0 + 0, 4, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSP1Triangle(0, 2, 3, 0),
	gsSPEndDisplayList(),
};


Gfx mat_exclamation_mark_f3dlite_material[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0, 0, 0, 0, TEXEL0),
	gsDPSetTextureFilter(G_TF_POINT),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, exclamation_mark_exclamation_mark_rgba16),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0, G_TX_CLAMP | G_TX_MIRROR, 2, 0),
	gsDPLoadBlock(7, 0, 0, 63, 2048),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 4, 0, G_TX_CLAMP | G_TX_MIRROR, 2, 0),
	gsDPSetTileSize(0, 0, 0, 28, 60),
	gsSPEndDisplayList(),
};

Gfx mat_revert_exclamation_mark_f3dlite_material[] = {
	gsDPPipeSync(),
	gsDPSetTextureFilter(G_TF_BILERP),
	gsSPEndDisplayList(),
};

Gfx exclamation_mark_Cube_mesh_layer_4[] = {
	gsSPDisplayList(mat_exclamation_mark_f3dlite_material),
	gsSPDisplayList(exclamation_mark_Cube_mesh_layer_4_tri_0),
	gsSPDisplayList(mat_revert_exclamation_mark_f3dlite_material),
	gsSPEndDisplayList(),
};

Gfx exclamation_mark_material_revert_render_settings[] = {
	gsDPPipeSync(),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPClearGeometryMode(G_TEXTURE_GEN),
	gsDPSetCombineLERP(0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT, 0, 0, 0, SHADE, 0, 0, 0, ENVIRONMENT),
	gsSPTexture(65535, 65535, 0, 0, 0),
	gsDPSetEnvColor(255, 255, 255, 255),
	gsDPSetAlphaCompare(G_AC_NONE),
	gsSPEndDisplayList(),
};

