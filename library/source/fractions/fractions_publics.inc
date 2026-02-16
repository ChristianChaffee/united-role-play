
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

		for(new g, rank_str[8]; g < MAX_FRACTION_RANKS; g++){
			format(rank_str, sizeof(rank_str), "rank_%d", g + 1);
			cache_get_value_name(i, rank_str, fraction_ranks[fraction_index][g]);
		}
	}

	if(old_fraction_index == -1){
		printf("[LoadFractions]: Фракции загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
		load_info[loaded_fractions] = true;
	}
	return true;
}