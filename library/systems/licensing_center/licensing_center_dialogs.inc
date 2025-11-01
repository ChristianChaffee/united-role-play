
DialogResponse:D_LICENSING_CENTER_MENU(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPVarInt(playerid, "LicensingCenter_Index", listitem);
	LicensingCenter:ShowAccept(playerid);
	return true;
}

DialogResponse:D_LICENSING_CENTER_ACCEPT(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "LicensingCenter_Index");
		LicensingCenter:ShowMenu(playerid);
		return false;
	}

	if(GetPlayerData(playerid, p_money) < GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_category_price)){
		SendErrorMessage(playerid, "У Вас недостаточно денежных средств для проведения практического экзамена/продления срока действия.");
		DeletePVar(playerid, "LicensingCenter_Index");
		return false;
	}

	new license_global_index = GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_global_index);
	if(player_info[playerid][p_licenses][license_global_index] && player_info[playerid][p_licenses_times][license_global_index] > 0){
		if(floatround((float(player_info[playerid][p_licenses_times][license_global_index]) - float(gettime())) / 86400.0) > GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_time) * 2){
			SendErrorMessage(playerid, "Ваша лицензия и так имеет слишком большой срок действия.");
			DeletePVar(playerid, "LicensingCenter_Index");
			return false;
		}

		GiveMoney(playerid, -GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_category_price));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Вы продлили \"%s\" на %d дней.",
			GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_category_name),
			GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_time));
		SendClientMessage(playerid, COLOR_GREEN, small_string);
		small_string[0] = EOS;

		GivePlayerLicense(playerid, license_global_index, floatround((float(player_info[playerid][p_licenses_times][license_global_index]) - float(gettime())) / 86400.0) + GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_time));

		DeletePVar(playerid, "LicensingCenter_Index");
		return true;
	}

	GiveMoney(playerid, -GetLicensingCenterData(GetPVarInt(playerid, "LicensingCenter_Index"), lc_category_price));
	LicensingCenter:StartExam(playerid);
	return true;
}