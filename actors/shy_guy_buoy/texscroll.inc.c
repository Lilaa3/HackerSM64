void scroll_gfx_mat_shy_guy_buoy_f3dlite_material() {
	Gfx *mat = segmented_to_virtual(mat_shy_guy_buoy_f3dlite_material);
	static int interval_tex_0_shy_guy_buoy_f3dlite_material = 2;
	static int cur_interval_tex_0_shy_guy_buoy_f3dlite_material = 2;

	if (--cur_interval_tex_0_shy_guy_buoy_f3dlite_material <= 0) {
		shift_t(mat, 9, PACK_TILESIZE(0, 16));
		cur_interval_tex_0_shy_guy_buoy_f3dlite_material = interval_tex_0_shy_guy_buoy_f3dlite_material;
	}

};

void scroll_actor_geo_shy_guy_buoy() {
	scroll_gfx_mat_shy_guy_buoy_f3dlite_material();
};
