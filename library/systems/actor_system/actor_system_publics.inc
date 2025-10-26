
forward LoadActors();
public LoadActors(){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	if(rows > MAX_ACTORS){
		printf("[LoadActors - ERROR]: Количество записей в базе данных превышает размер массива (%d > %d)", rows, MAX_ACTORS);
		return false;
	}

	for(new i, actor_id, show_text; i < rows; i++){
		show_text = 0;
		cache_get_value_name_int(i, "id", GetActorData(i, a_id));
		cache_get_value_name(i, "name", GetActorData(i, a_name));
		cache_get_value_name_int(i, "model_id", GetActorData(i, a_model_id));

		cache_get_value_name_float(i, "pos_x", GetActorData(i, a_pos_x));
		cache_get_value_name_float(i, "pos_y", GetActorData(i, a_pos_y));
		cache_get_value_name_float(i, "pos_z", GetActorData(i, a_pos_z));
		cache_get_value_name_float(i, "pos_angle", GetActorData(i, a_pos_angle));
		cache_get_value_name_int(i, "vw", GetActorData(i, a_vw));
		cache_get_value_name_int(i, "int", GetActorData(i, a_int));

		cache_get_value_name_int(i, "invulnerable", GetActorData(i, a_invulnerable));

		cache_get_value_name(i, "anim_lib", GetActorData(i, a_anim_lib));
		cache_get_value_name(i, "anim_name", GetActorData(i, a_anim_name));

		cache_get_value_name_int(i, "name_show", GetActorData(i, a_name_show));
		cache_get_value_name_int(i, "useble", GetActorData(i, a_useble));

		actor_id = CreateDynamicActor(GetActorData(i, a_model_id), 
			GetActorData(i, a_pos_x), GetActorData(i, a_pos_y), GetActorData(i, a_pos_z), GetActorData(i, a_pos_angle), 
			GetActorData(i, a_invulnerable), 100.0, 
			GetActorData(i, a_vw), GetActorData(i, a_int));

		if(strcmp(GetActorData(i, a_anim_lib), "none") && strcmp(GetActorData(i, a_anim_name), "none"))
			ApplyDynamicActorAnimation(actor_id, GetActorData(i, a_anim_lib), GetActorData(i, a_anim_name), 4.1, true, true, true, false, 0);
	
		if(GetActorData(i, a_name_show) && GetActorData(i, a_useble)){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_blue"%s\n"cl_grey"'Left ALT'",
				GetActorData(i, a_name));
			show_text = 1;
		}
		else if(GetActorData(i, a_name_show) && !GetActorData(i, a_useble)){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_blue"%s",
				GetActorData(i, a_name));
			show_text = 1;
		}
		else if(!GetActorData(i, a_name_show) && GetActorData(i, a_useble)){
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_grey"'Left ALT'");
			show_text = 1;
		}

		if(show_text){
			CreateDynamic3DTextLabel(small_string, -1, 
				GetActorData(i, a_pos_x), 
				GetActorData(i, a_pos_y),
				GetActorData(i, a_pos_z) + 1.45, 
				15.0, _, _, 1, 
				GetActorData(i, a_vw), GetActorData(i, a_int));
		}
	}

	printf("[LoadActors]: Актеры (NPC) загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}