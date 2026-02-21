
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM taxes", "LoadTaxes", "i", -1);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock PayDay(hour){
	for(new tax_index; tax_index < MAX_TAXES; tax_index++){
		if(!GetTaxData(tax_index, t_id)) continue;
		if(GetTaxData(tax_index, t_time_type) == TAX_TIME_TYPE_DAY && hour != 0) continue;

		if(!GetTaxData(tax_index, t_is_offline)){
			Taxes:GiveOnlineTax(tax_index);
		}
		else{
			Taxes:GiveOfflineTax(tax_index);
		}
	}

	static const fmt_query[] = "SELECT * FROM player_taxes WHERE account_id = %d AND timestamp + "#TAX_PAYMENT_TIME" < %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];

	for(new playerid, j = GetPlayerPoolSize(); playerid <= j; playerid++){
		if(!IsPlayerConnected(playerid) || !player_logged[playerid]) continue;

		mysql_format(mysql, query, sizeof(query), fmt_query,
			GetPlayerData(playerid, p_id), gettime());
		mysql_tquery(mysql, query, "TaxWantedPlayer", "i", playerid);
	}
	return continue(hour);
}