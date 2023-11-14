s32 simpleInt = 0;

f32 simpleFloat = 1 .4f;
f32 simpleFloat2 = 3.43f;
f32 simpleFloat3 = 3.f;

f32 simpleDouble = 1.4;

char *simpleString = "This is a string test\n\
This is another line which uses \ to seperate!""This is a string seperated by another set of quotes";

char *simpleString2 = 'This is another string test';

// One line comment test

/*  Another comment test, this time with
    multi-line
*/

ALIGNED8 static const Texture includeTest[1] = {
#include "actors/amp/amp_electricity.rgba16.inc.c"
};

static const Vtx vertexArrayTest[] = {
    { { { 224, 0, -89 }, 0, { 0, 480 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 187, 149, 0 }, 0, { 223, 1078 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 224, 0, 90 }, 0, { 479, 478 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 187, -148, 0 }, 0, { 224, -122 }, { 0xff, 0xff, 0xff, 0xff } } },
    { { { 224, 0, -89 }, 0, { 0, 478 }, { 0xff, 0xff, 0xff, 0xff } } },
};

f32 mathProcessorTest = ((20.7f * 2.1) / 5) % 4 + 4 - 1; // Random math, completly random

void bhv_homing_amp_init(void) {vec3f_copy(&o->oHomeVec, &o->oPosVec);
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
            do_2();
            break;
    }
}