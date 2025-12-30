
forward LoadBusinesses();
public LoadBusinesses(){
	new rows,
		tick_time = GetTickCount();

	cache_get_row_count(rows);

	if(rows >= MAX_BUSINESS){
		printf("[LoadBusinesses - ERROR]: Количество записей в базе данных превышает размер массива (%d >= %d)", rows, MAX_BUSINESS);
		return false;
	}

	for(new i, temp_string[256]; i < rows; i++){
		cache_get_value_name_int(i, "id", GetBusinessData(i, b_id));
		cache_get_value_name_int(i, "type", GetBusinessData(i, b_type));
		cache_get_value_name(i, "name", GetBusinessData(i, b_name));
		cache_get_value_name_int(i, "owner_id", GetBusinessData(i, b_owner_id));

		cache_get_value_name_float(i, "pos_x", GetBusinessData(i, b_pos_x));
		cache_get_value_name_float(i, "pos_y", GetBusinessData(i, b_pos_y));
		cache_get_value_name_float(i, "pos_z", GetBusinessData(i, b_pos_z));
		cache_get_value_name_int(i, "vw", GetBusinessData(i, b_vw));
		cache_get_value_name_int(i, "int", GetBusinessData(i, b_int));

		cache_get_value_name_int(i, "products", GetBusinessData(i, b_products));
		cache_get_value_name_int(i, "balance", GetBusinessData(i, b_balance));
		cache_get_value_name_int(i, "price", GetBusinessData(i, b_price));

		cache_get_value_name(i, "products_prices", temp_string);
		sscanf(temp_string, "p<,>a<i>["#MAX_BUSINESS_MENU_POSITIONS"]", business_info[i][b_products_prices]);

		if(GetBusinessData(i, b_type) >= BUSINESS_TYPES_COUNT){
			printf("[LoadBusinesses - WARNING]: Тип бизнеса (ID: %d, type: %d) превышает максимальное количество типов бизнеса (%d >= %d)", 
				GetBusinessData(i, b_id), GetBusinessData(i, b_type),
				GetBusinessData(i, b_type), BUSINESS_TYPES_COUNT);
			continue;
		}

		if(GetBusinessData(i, b_owner_id) != 0 && GetBusinessData(i, b_balance) < 0){
			static const fmt_query_1[] = "UPDATE `accounts` SET `money` = `money` + '%d' WHERE id = %d";
			new query_1[sizeof(fmt_query_1) + ((-2 + 11) * 2)];
			mysql_format(mysql, query_1, sizeof(query_1), fmt_query_1,
				floatround(float(GetBusinessData(i, b_price)) * BUSINESS_SELL_PERCENT),
				GetBusinessData(i, b_owner_id));
			mysql_tquery(mysql, query_1);

			SetBusinessData(i, b_owner_id, 0);
			SetBusinessData(i, b_balance, 0);

			static const fmt_query_2[] = "UPDATE business SET owner_id = 0, balance = 0 WHERE id = %d";
			new query_2[sizeof(fmt_query_2) + (-2 + 11)];
			mysql_format(mysql, query_2, sizeof(query_2), fmt_query_2,
				GetBusinessData(i, b_id));
			mysql_tquery(mysql, query_2);
		}

		//-------------------
		
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			""cl_lime"[%s]\n"cl_white"%s\n"cl_grey"'Left ALT'",
			business_type_names[GetBusinessData(i, b_type)], GetBusinessData(i, b_name));

		GetBusinessData(i, b_enter_text) = CreateDynamic3DTextLabel(small_string, -1, 
			GetBusinessData(i, b_pos_x), GetBusinessData(i, b_pos_y), GetBusinessData(i, b_pos_z), 
			15.0, _, _, 1, 
			GetBusinessData(i, b_vw), GetBusinessData(i, b_int));
		small_string[0] = EOS;

		GetBusinessData(i, b_enter_pickup) = CreateDynamicPickup(BUSINESS_ENTER_PICKUP_MODEL_ID, 23, 
			GetBusinessData(i, b_pos_x), GetBusinessData(i, b_pos_y), GetBusinessData(i, b_pos_z), 
			GetBusinessData(i, b_vw), GetBusinessData(i, b_int));
		
		//-------------------

		GetBusinessData(i, b_exit_text) = CreateDynamic3DTextLabel(""cl_white"Выход\n"cl_grey"'Left ALT'", -1, 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_x), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_y), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_z), 
			15.0, _, _, 1, 
			GetBusinessData(i, b_id), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_int));

		GetBusinessData(i, b_exit_pickup) = CreateDynamicPickup(BUSINESS_ENTER_PICKUP_MODEL_ID, 23, 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_x), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_y), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_exit_pos_z), 
			GetBusinessData(i, b_id), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_int));
		
		//-------------------

		GetBusinessData(i, b_cashbox_text) = CreateDynamic3DTextLabel(GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_text), -1, 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pos_x), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pos_y), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pos_z), 
			15.0, _, _, 1, 
			GetBusinessData(i, b_id), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_int));

		GetBusinessData(i, b_cashbox_pickup) = CreateDynamicPickup(GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pickup_model_id), 23, 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pos_x), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pos_y), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_cashbox_pos_z), 
			GetBusinessData(i, b_id), 
			GetBusinessInteriorData(GetBusinessData(i, b_type), bi_int));

		//-------------------
	}

	printf("[LoadBusinesses]: Бизнесы загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}