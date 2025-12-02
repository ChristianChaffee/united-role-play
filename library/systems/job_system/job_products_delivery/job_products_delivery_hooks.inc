
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	job_products_delivery_price = RandomEx(JOB_PD_MIN_PRICE, JOB_PD_MAX_PRICE);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_lime"[Загрузка продуктов]\n\
		"cl_grey"Закупочная цена: "cl_white"%d$\n\
		"cl_grey"Для закупки используйте '/delivery'",
		job_products_delivery_price);

	job_products_delivery_text = CreateDynamic3DTextLabel(small_string, -1, JOB_PD_LOAD_POS, 25.0, _, _, 1, 0, 0);
	small_string[0] = EOS;

	job_products_change_price_time = JOB_PD_CHANGE_PRICE_TIME;
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock SecondTimerStock(){
	if(job_products_change_price_time > 0){
		job_products_change_price_time--;

		if(job_products_change_price_time <= 0){
			job_products_delivery_price = RandomEx(JOB_PD_MIN_PRICE, JOB_PD_MAX_PRICE);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				""cl_lime"[Загрузка продуктов]\n\
				"cl_grey"Закупочная цена: "cl_white"%d$\n\
				"cl_grey"Для закупки используйте '/delivery'",
				job_products_delivery_price);

			UpdateDynamic3DTextLabelText(job_products_delivery_text, -1, small_string);
			small_string[0] = EOS;

			job_products_change_price_time = JOB_PD_CHANGE_PRICE_TIME;

		}
	}
	return continue();
}