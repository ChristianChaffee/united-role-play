
stock Registration:LoadPlayer(playerid, Cache:cache_id){
	cache_set_active(cache_id);

	cache_get_value_name(0, "name", GetPlayerData(playerid, p_name));
	cache_get_value_name(0, "email", GetPlayerData(playerid, p_email));
	cache_get_value_name_int(0, "nationality", GetPlayerData(playerid, p_nationality));
	cache_get_value_name_int(0, "age", GetPlayerData(playerid, p_age));
	cache_get_value_name_int(0, "skin", GetPlayerData(playerid, p_skin));
	cache_get_value_name_int(0, "sex", GetPlayerData(playerid, p_sex));

	cache_delete(cache_id);

	SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
	return true;
}

stock Registration:ShowRegPassword(playerid){
	big_string[0] = EOS;

	format(big_string, sizeof(big_string),
		"\\c"cl_grey"=========================================\n\
		"cl_grey"- "cl_white"Аккаунт "cl_blue"%s "cl_white"не зарегистрирован на проекте\n\n\
		"cl_grey"\\cПридумайте пароль для прохождения \n\
		\\cдальнейшей регистрации на проекте\n\
		\\c"cl_grey"=========================================",
		GetPlayerData(playerid, p_name));

	Dialog_Open(playerid, Dialog:D_REGISTRATION_PASSWORD, DIALOG_STYLE_INPUT, 
		"Регистрация", big_string,
		"Далее", "Выйти");
	big_string[0] = EOS;
	return true;
}

stock Registration:ShowRegEmail(playerid){
	Dialog_Open(playerid, Dialog:D_REGISTRATION_EMAIL, DIALOG_STYLE_INPUT, 
		"Регистрация",
		""cl_grey"- "cl_white"Введите ваш "cl_blue"действующий "cl_white"адрес электронной почты (e-mail)\n\n\
		"cl_grey"[Он поможет Вам восстановить доступ к аккаунту в случае утери пароля]",
		"Далее", "Пропустить");
	return true;
}

stock Registration:ShowRegCharacter(playerid){
	new 
		age_str[10+6+3],
		skin_str[10+6+3];
	
	if(!GetPlayerData(playerid, p_age)){
		format(age_str, sizeof(age_str), ""cl_grey"Не выбрано");
	}
	else format(age_str, sizeof(age_str), ""cl_blue"%d", GetPlayerData(playerid, p_age));

	if(!GetPlayerData(playerid, p_skin)){
		format(skin_str, sizeof(skin_str), ""cl_grey"Не выбрано");
	}
	else format(skin_str, sizeof(skin_str), ""cl_blue"%d", GetPlayerData(playerid, p_skin));

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Параметр\t"cl_grey"Значение\n\
		"cl_grey"1. "cl_white"Имя и фамилия\t"cl_blue"%s\n\
		"cl_grey"2. "cl_white"Возраст\t%s\n\
		"cl_grey"3. "cl_white"Гражданство\t%s%s\n\
		"cl_grey"4. "cl_white"Пол\t"cl_blue"%s\n\
		"cl_grey"5. "cl_white"Внешний вид\t%s\n\
		"cl_green"— Завершить регистрацию",
		GetPlayerData(playerid, p_name), age_str,
		(!GetPlayerData(playerid, p_nationality)) ? (""cl_grey"") : (""cl_blue""),
		character_nationality[GetPlayerData(playerid, p_nationality)],
		(GetPlayerData(playerid, p_sex)) ? ("Женский") : ("Мужской"),
		skin_str);

	Dialog_Open(playerid, Dialog:D_REGISTRATION_CHARACTER, DIALOG_STYLE_TABLIST_HEADERS, 
		"Настройка песонажа", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Registration:ShowRegAge(playerid){
	Dialog_Open(playerid, Dialog:D_REGISTRATION_AGE, DIALOG_STYLE_INPUT, 
		"Возраст персонажа",
		""cl_grey"- "cl_white"Введите возраст персонажа\n\
		"cl_blue"Примечание: "cl_white"возраст персонажа должен быть от "cl_grey""#CHARACTER_MIN_AGE" "cl_white"до "cl_grey""#CHARACTER_MAX_AGE" "cl_white"лет",
		"Ввести", "");
	return true;
}

stock Registration:ShowRegNationality(playerid){
	big_string[0] = EOS;
	for(new i = 1; i < sizeof(character_nationality); i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_grey"%d. "cl_white"%s",
			big_string, i, character_nationality[i]);
	}
	Dialog_Open(playerid, Dialog:D_REGISTRATION_NATIONALITY, DIALOG_STYLE_LIST,
		"Национальность персонажа", big_string,
		"Выбрать", "");
	big_string[0] = EOS;
	return true;
}

stock Registration:FinishRegistration(playerid){
	static const 
		fmt_query[] = "INSERT INTO accounts \
		(name, password, email, nationality, age, skin, sex) VALUES \
		('%s', MD5('%s'), '%s', '%d', '%d', '%d', '%d')";

	new query[sizeof(fmt_query) - 2 + 24 - 2 + 32 - 2 + 32 - 2 + 11 - 2 + 11 - 2 + 11 - 2 + 11];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(playerid, p_name), GetPlayerData(playerid, p_password), GetPlayerData(playerid, p_email),
		GetPlayerData(playerid, p_nationality), GetPlayerData(playerid, p_age), GetPlayerData(playerid, p_skin),
		GetPlayerData(playerid, p_sex));
	mysql_tquery(mysql, query, "CreatePlayerAccount");
	return true;
}

stock Registration:ShowLogPassword(playerid){
	big_string[0] = EOS;

	format(big_string, sizeof(big_string),
		"\\c"cl_grey"=========================================\n\
		"cl_grey"- "cl_white"Аккаунт "cl_blue"%s "cl_white"зарегистрирован на проекте\n\n\
		"cl_grey"\\cВведите пароль для авторизации\n\
		\\c"cl_grey"=========================================",
		GetPlayerData(playerid, p_name));

	Dialog_Open(playerid, Dialog:D_LOGIN_PASSWORD, DIALOG_STYLE_PASSWORD, 
		"Авторизация", big_string,
		"Далее", "Выйти");
	big_string[0] = EOS;
}

//==============================================================================
stock Registration:SetCamera(playerid){
	SetPlayerPos(playerid, REGISTRATION_CAMERA_POS + 5.0);
	SetPlayerVirtualWorld(playerid, REGISTRATION_VW);
	SetPlayerInterior(playerid, REGISTRATION_INT);
	SetPlayerCameraPos(playerid, REGISTRATION_CAMERA_POS);
	SetPlayerCameraLookAt(playerid, REGISTRATION_CAMERA_LOOK_POS);

	TogglePlayerControllable(playerid, false);
	return true;
}