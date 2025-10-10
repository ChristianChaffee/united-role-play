
#define Bank:%0(%1)								Bnk_%0(%1)

#define GetPlayerBankCardData(%0,%1)  			player_bank_card_info[%0][%1]
#define SetPlayerBankCardData(%0,%1,%2)  		player_bank_card_info[%0][%1] = %2
#define AddPlayerBankCardData(%0,%1,%2)  		player_bank_card_info[%0][%1] += %2

//==============================================================================
#define BANK_MAP_ICON_POS 						1457.08, -1009.92, 26.84
#define BANK_MAP_ICON_TYPE						52

#define BANK_VW									5
#define BANK_INT 								5
#define BANK_CASHBOX_PICKUP_MODEL_ID			1274

#define BANK_CARD_CREATE_PRICE 					35
#define BANK_CARD_CREATE_TIME 					1

#define BANK_CARD_PIN_CODE_VALID_TIME 			60

//==============================================================================
new const Float:bank_cashboxes_pos[6][3] = {
	{1311.25, 24.06, 2068.52},
	{1312.82, 24.06, 2068.52},
	{1314.60, 24.06, 2068.52},
	{1324.21, 24.06, 2068.52},
	{1326.01, 24.06, 2068.51},
	{1327.62, 24.06, 2068.51}
};

new const bank_card_types[4][32] = {
	""cl_white"Standart",
	""cl_grey"Silver",
	""cl_gold"Gold",
	""cl_brilliant"Brilliant"
};

new const bank_deposit_percents[4] = {
	0, 3, 5, 10
};

new const bank_type_prices[4] = {
	0, 200, 400, 1000
};

//==============================================================================
enum
{
	BC_STATUS_NONE,
	BC_STATUS_WAIT,
	BC_STATUS_ACTIVE
}

enum player_bank_card_info_struct
{
	pbc_is_loaded,
	pbc_pin_code_timer,

	pbc_id,
	pbc_account_id,
	pbc_balance,
	pbc_create_time,
	pbc_pin_code,
	pbc_status,
	pbc_timer,
	pbc_type,
	pbc_deposit_balance,
	pbc_deposit_percent,
}
new player_bank_card_info[MAX_PLAYERS][player_bank_card_info_struct];