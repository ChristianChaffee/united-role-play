
forward LoadFractions(fraction_index);
public LoadFractions(fraction_index){
	new rows,
		tick_time = GetTickCount(),
		old_fraction_index = fraction_index;

	cache_get_row_count(rows);
	if(rows >= MAX_FRACTIONS){
		printf("[LoadFractions - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_FRACTIONS);
		return false;
	}

	for(new i; i < rows; i++){
		if(old_fraction_index == -1){
			fraction_index = i;
		}

		cache_get_value_name_int(i, "id", GetFractionData(fraction_index, f_id));
		cache_get_value_name(i, "name", GetFractionData(fraction_index, f_name));
		cache_get_value_name(i, "tag", GetFractionData(fraction_index, f_tag));
		cache_get_value_name_int(i, "type", GetFractionData(fraction_index, f_type));
		cache_get_value_name_int(i, "leader_account_id", GetFractionData(fraction_index, f_leader_account_id));

		cache_get_value_name(i, "color_str", GetFractionData(fraction_index, f_color_str));
		cache_get_value_name_int(i, "color_value", GetFractionData(fraction_index, f_color_value));

		cache_get_value_name_int(i, "ranks_count", GetFractionData(fraction_index, f_ranks_count));

		cache_get_value_name_float(i, "spawn_x", GetFractionData(fraction_index, f_spawn_x));
		cache_get_value_name_float(i, "spawn_y", GetFractionData(fraction_index, f_spawn_y));
		cache_get_value_name_float(i, "spawn_z", GetFractionData(fraction_index, f_spawn_z));
		cache_get_value_name_float(i, "spawn_angle", GetFractionData(fraction_index, f_spawn_angle));
		cache_get_value_name_int(i, "spawn_vw", GetFractionData(fraction_index, f_spawn_vw));
		cache_get_value_name_int(i, "spawn_int", GetFractionData(fraction_index, f_spawn_int));

		cache_get_value_name(i, "icon_url", GetFractionData(fraction_index, f_icon_url));

		cache_get_value_name_float(i, "dressroom_x", GetFractionData(fraction_index, f_dressroom_x));
		cache_get_value_name_float(i, "dressroom_y", GetFractionData(fraction_index, f_dressroom_y));
		cache_get_value_name_float(i, "dressroom_z", GetFractionData(fraction_index, f_dressroom_z));
		cache_get_value_name_int(i, "dressroom_vw", GetFractionData(fraction_index, f_dressroom_vw));
		cache_get_value_name_int(i, "dressroom_int", GetFractionData(fraction_index, f_dressroom_int));

		for(new g, rank_str[8]; g < MAX_FRACTION_RANKS; g++){
			format(rank_str, sizeof(rank_str), "rank_%d", g + 1);
			cache_get_value_name(i, rank_str, fraction_ranks[fraction_index][g]);
		}

		Fraction:UpdateDressroom(fraction_index);
	}

	if(old_fraction_index == -1){
		printf("[LoadFractions]: Фракции загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
		load_info[loaded_fractions] = true;
	}
	return true;
}

forward LoadFractionSkins(fraction_skin_index);
public LoadFractionSkins(fraction_skin_index){
	new rows,
		tick_time = GetTickCount(),
		old_fraction_skin_index = fraction_skin_index;

	cache_get_row_count(rows);
	if(rows >= MAX_FRACTION_SKINS){
		printf("[LoadFractionSkins - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_FRACTION_SKINS);
		return false;
	}

	for(new i; i < rows; i++){
		if(old_fraction_skin_index == -1){
			fraction_skin_index = i;
		}

		cache_get_value_name_int(i, "id", GetFractionSkinData(fraction_skin_index, fs_id));
		cache_get_value_name_int(i, "fraction_id", GetFractionSkinData(fraction_skin_index, fs_fraction_id));
		cache_get_value_name_int(i, "skin_id", GetFractionSkinData(fraction_skin_index, fs_skin_id));
		cache_get_value_name_int(i, "rank", GetFractionSkinData(fraction_skin_index, fs_rank));
		cache_get_value_name_int(i, "sex", GetFractionSkinData(fraction_skin_index, fs_sex));
	}

	if(old_fraction_skin_index == -1){
		printf("[LoadFractionSkins]: Скины фракций загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
		load_info[loaded_fractions_skins] = true;
	}
	return true;
}