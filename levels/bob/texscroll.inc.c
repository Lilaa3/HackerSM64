void scroll_gfx_mat_bob_dl_Water_layer5_area1() {
	Gfx *mat = segmented_to_virtual(mat_bob_dl_Water_layer5_area1);


	shift_t(mat, 13, PACK_TILESIZE(0, 2));
	shift_t(mat, 15, PACK_TILESIZE(0, 2));

};

void scroll_gfx_mat_bob_dl_f3dlite_material_005_layer4() {
	Gfx *mat = segmented_to_virtual(mat_bob_dl_f3dlite_material_005_layer4);


	shift_t(mat, 11, PACK_TILESIZE(0, 3));
	shift_t(mat, 13, PACK_TILESIZE(0, 8));

};

void scroll_gfx_mat_bob_dl_f3dlite_material_layer1() {
	Gfx *mat = segmented_to_virtual(mat_bob_dl_f3dlite_material_layer1);


	shift_s(mat, 11, PACK_TILESIZE(0, 1));
	shift_t(mat, 11, PACK_TILESIZE(0, 2));
	shift_s_down(mat, 13, PACK_TILESIZE(0, 1));
	shift_t(mat, 13, PACK_TILESIZE(0, 1));

};

void scroll_bob() {
	scroll_gfx_mat_bob_dl_Water_layer5_area1();
	scroll_gfx_mat_bob_dl_f3dlite_material_005_layer4();
	scroll_gfx_mat_bob_dl_f3dlite_material_layer1();
};
