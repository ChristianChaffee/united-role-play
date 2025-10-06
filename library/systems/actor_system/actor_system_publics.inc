
forward LoadActors();
public LoadActors(){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	if(rows > MAX_ACTORS){
		printf("[LoadActors - ERROR]: Количество записей в базе данных превышает размер массива (%d > %d)", rows, MAX_ACTORS);
		return false;
	}

	for(new i, actor_id; i < rows; i++){
		cache_get_value_name(i, "name", GetActorInfo(i, a_name));
		cache_get_value_name_int(i, "model_id", GetActorInfo(i, a_model_id));

		cache_get_value_name_float(i, "pos_x", GetActorInfo(i, a_pos_x));
		cache_get_value_name_float(i, "pos_y", GetActorInfo(i, a_pos_y));
		cache_get_value_name_float(i, "pos_z", GetActorInfo(i, a_pos_z));
		cache_get_value_name_float(i, "pos_angle", GetActorInfo(i, a_pos_angle));
		cache_get_value_name_int(i, "vw", GetActorInfo(i, a_vw));
		cache_get_value_name_int(i, "int", GetActorInfo(i, a_int));

		cache_get_value_name_int(i, "invulnerable", GetActorInfo(i, a_invulnerable));

		cache_get_value_name(i, "anim_lib", GetActorInfo(i, a_anim_lib));
		cache_get_value_name(i, "anim_name", GetActorInfo(i, a_anim_name));

		actor_id = CreateDynamicActor(GetActorInfo(i, a_model_id), 
			GetActorInfo(i, a_pos_x), GetActorInfo(i, a_pos_y), GetActorInfo(i, a_pos_z), GetActorInfo(i, a_pos_angle), 
			GetActorInfo(i, a_invulnerable), 100.0, 
			GetActorInfo(i, a_vw), GetActorInfo(i, a_int));

		if(strcmp(GetActorInfo(i, a_anim_lib), "none") && strcmp(GetActorInfo(i, a_anim_name), "none"))
			ApplyDynamicActorAnimation(actor_id, GetActorInfo(i, a_anim_lib), GetActorInfo(i, a_anim_name), 4.1, true, true, true, false, 0);
	}

	printf("[LoadActors]: Актеры (NPC) загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}