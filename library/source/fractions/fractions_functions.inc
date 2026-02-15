
stock Fraction:UpdateStr(fraction_index, row[], value[]){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	static const fmt_query[] = "UPDATE fractions SET `%s` = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 128) + (-2 + 128) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, row, value, GetFractionData(fraction_index, f_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Fraction:UpdateInt(fraction_index, row[], value){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	static const fmt_query[] = "UPDATE fractions SET `%s` = '%d' WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 128) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query, row, value, GetFractionData(fraction_index, f_id));
	mysql_tquery(mysql, query);
	return true;
}

stock Fraction:UninvitePlayer(account_id){
	#pragma unused account_id
	#warning MAKE_LOGIC


	return true;
}