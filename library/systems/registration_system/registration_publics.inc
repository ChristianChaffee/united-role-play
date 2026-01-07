
forward CheckPlayerRegistration(playerid);
public CheckPlayerRegistration(playerid){
	new rows;
	cache_get_row_count(rows);

	if(rows){
		Registration:ShowLogPassword(playerid);
	}
	else{
		Registration:ShowRegPassword(playerid);
	}
}

forward CheckAccountPassword(playerid);
public CheckAccountPassword(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "Пароль введён не верно!");
		Registration:ShowLogPassword(playerid);
		return false;
	}
	else{
		cef_emit_event(playerid, "hideAuthWindow");
		LoadPlayer(playerid, cache_save());
		return true;
	}
}

forward CheckAccountEmail(playerid, email[]);
public CheckAccountEmail(playerid, email[]){
	new rows;
	cache_get_row_count(rows);

	if(rows){
		SendErrorMessage(playerid, "Данный адрес электронной почты уже занят другим игроком!");
		Registration:ShowRegEmail(playerid);
		return false;
	}
	else{
		for(new i, j = GetPlayerPoolSize(); i <= j; i++){
			if(!strcmp(email, GetPlayerData(i, p_email)) && i != playerid){
				SendErrorMessage(playerid, "Данный адрес электронной почты уже занят другим игроком!");
				Registration:ShowRegEmail(playerid);
				return false;
			}
		}
	}

	format(GetPlayerData(playerid, p_email), 32, "%s", email);
	Registration:ShowRegCharacter(playerid);
	return true;
}

forward CreatePlayerAccount(playerid);
public CreatePlayerAccount(playerid){
	GetPlayerData(playerid, p_id) = cache_insert_id();

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Игрок %s[%d] зарегистрировался на проекте!",
		GetPlayerData(playerid, p_name), playerid);
	SendClientMessageToAll(COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
}

forward OnPlayerCefAuthorization(playerid, const arguments[]);
public OnPlayerCefAuthorization(playerid, const arguments[]){
	new response, password[32];
	
	if(!sscanf(arguments, "is[32]", response, password)){
		if(!response){
			KickPlayer(playerid);
			return false;
		}

		static const fmt_query[] = "SELECT * FROM accounts WHERE name = '%s' AND password = MD5('%s')";
		new query[sizeof(fmt_query) - 2 + 24 - 2 + 32];
		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerData(playerid, p_name), password);
		mysql_tquery(mysql, query, "CheckAccountPassword", "i", playerid);
	}
	return true;
}

forward RegistrationTimer(playerid);
public RegistrationTimer(playerid){
	GetPlayerName(playerid, GetPlayerData(playerid, p_name), MAX_PLAYER_NAME);
	static const fmt_query[] = "SELECT * FROM accounts WHERE name = '%s' LIMIT 1";

	new query[sizeof(fmt_query) - 2 + 24];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_name));
	mysql_tquery(mysql, query, "CheckPlayerRegistration", "i", playerid);
	return true;
}