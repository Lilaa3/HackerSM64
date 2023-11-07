s32 simpleInt = 0;

f32 simpleFloat = 1.4f;

f32 simpleDouble = 1.4;

// One line comment test

/*  Another comment test, this time with
    multi-line
*/

ALIGNED8 static const Texture dAmpElectricityTexture[] = {
#include "actors/amp/amp_electricity.rgba16.inc.c"
};

enum RenderLayers {
    LAYER_FORCE,
    LAYER_OPAQUE,
    LAYER_OPAQUE_INTER,
    LAYER_OPAQUE_DECAL,
    LAYER_ALPHA,
#if SILHOUETTE
    LAYER_ALPHA_DECAL,
    LAYER_SILHOUETTE_OPAQUE,
    LAYER_SILHOUETTE_ALPHA,
    LAYER_OCCLUDE_SILHOUETTE_OPAQUE,
    LAYER_OCCLUDE_SILHOUETTE_ALPHA,
#endif
    LAYER_TRANSPARENT_DECAL,
    LAYER_TRANSPARENT,
    LAYER_TRANSPARENT_INTER,
    LAYER_COUNT
};

static const Vtx dAmpElectricityVertices[] = {
    { { { 224, 0, -89 }, 0, { 0, 480 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 187, 149, 0 }, 0, { 223, 1078 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 224, 0, 90 }, 0, { 479, 478 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 187, -148, 0 }, 0, { 224, -122 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 224, 0, -89 }, 0, { 0, 478 }, { 0xff, 0xff, 0xff, 0xff } } },
};

const Gfx dAmpElectricityDl[] = {
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
    gsSPClearGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, G_TX_LOADTILE, 0, G_TX_WRAP | G_TX_NOMIRROR,
                G_TX_NOMASK, G_TX_NOLOD, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOLOD),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPTileSync(),
    gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 4, 0, G_TX_RENDERTILE, 0, G_TX_CLAMP, 5, G_TX_NOLOD,
                G_TX_CLAMP, 4, G_TX_NOLOD),
    gsDPSetTileSize(0, 0, 0, (16 - 1) << G_TEXTURE_IMAGE_FRAC, (32 - 1) << G_TEXTURE_IMAGE_FRAC),
    gsSPDisplayList(dAmpElectricitySubDl),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
    gsDPPipeSync(),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
    gsSPSetGeometryMode(G_LIGHTING | G_CULL_BACK),
    gsSPEndDisplayList(),
};

static struct ObjectHitbox sAmpHitbox = {
    /* interactType:      */ INTERACT_SHOCK,
    /* downOffset:        */ 40,
    /* damageOrCoinValue: */ 1,
    /* health:            */ 0,
    /* numLootCoins:      */ 0,
    /* radius:            */ 40,
    /* height:            */ 50,
    /* hurtboxRadius:     */ 50,
    /* hurtboxHeight:     */ 60,
};

// Function Test
void bhv_homing_amp_init(void) {
    vec3f_copy(&o->oHomeVec, &o->oPosVec);
    o->oGravity = 0.0f;
    o->oHomingAmpAvgY = o->oHomeY;

    cur_obj_scale(0.1f);

    o->header.gfx.node.flags |= GRAPH_RENDER_INVISIBLE;

    switch (o->unknownVariable) {
        case 0:
        case 1:
            do_0_1();
            break;
        case 2:
            do_2;
    }
}