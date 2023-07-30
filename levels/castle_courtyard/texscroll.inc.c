void scroll_gfx_mat_castle_courtyard_dl_f3dlite_material_022() {
	Gfx *mat = segmented_to_virtual(mat_castle_courtyard_dl_f3dlite_material_022);
	static int interval_tex_castle_courtyard_dl_f3dlite_material_022 = 7;
	static int cur_interval_tex_castle_courtyard_dl_f3dlite_material_022 = 7;

	if (--cur_interval_tex_castle_courtyard_dl_f3dlite_material_022 <= 0) {
		shift_s(mat, 9, PACK_TILESIZE(0, 128));
		cur_interval_tex_castle_courtyard_dl_f3dlite_material_022 = interval_tex_castle_courtyard_dl_f3dlite_material_022;
	}

};

void scroll_gfx_mat_castle_courtyard_dl_f3dlite_material_014() {
	Gfx *mat = segmented_to_virtual(mat_castle_courtyard_dl_f3dlite_material_014);
	static int interval_tex_castle_courtyard_dl_f3dlite_material_014 = 3;
	static int cur_interval_tex_castle_courtyard_dl_f3dlite_material_014 = 3;

	if (--cur_interval_tex_castle_courtyard_dl_f3dlite_material_014 <= 0) {
		shift_s(mat, 9, PACK_TILESIZE(0, 128));
		cur_interval_tex_castle_courtyard_dl_f3dlite_material_014 = interval_tex_castle_courtyard_dl_f3dlite_material_014;
	}

};

void scroll_gfx_mat_castle_courtyard_dl_f3dlite_material_012() {
	Gfx *mat = segmented_to_virtual(mat_castle_courtyard_dl_f3dlite_material_012);
	static int interval_tex_castle_courtyard_dl_f3dlite_material_012 = 16;
	static int cur_interval_tex_castle_courtyard_dl_f3dlite_material_012 = 16;

	if (--cur_interval_tex_castle_courtyard_dl_f3dlite_material_012 <= 0) {
		shift_s(mat, 9, PACK_TILESIZE(0, 128));
		cur_interval_tex_castle_courtyard_dl_f3dlite_material_012 = interval_tex_castle_courtyard_dl_f3dlite_material_012;
	}

};

void scroll_castle_courtyard() {
	scroll_gfx_mat_castle_courtyard_dl_f3dlite_material_022();
	scroll_gfx_mat_castle_courtyard_dl_f3dlite_material_014();
	scroll_gfx_mat_castle_courtyard_dl_f3dlite_material_012();
};
