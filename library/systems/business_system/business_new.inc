
/*
Используемые PVar`ы:
	Business_Index
	Business_MenuPositionIndex
	Business_PerkIndex

*/

#define Business:%0(%1) 					Bsnss_%0(%1)

//==============================================================================
#define GetBusinessData(%0,%1) 				business_info[%0][%1]
#define SetBusinessData(%0,%1,%2) 			business_info[%0][%1] = %2
#define AddBusinessData(%0,%1,%2) 			business_info[%0][%1] += %2

#define GetBusinessInteriorData(%0,%1) 		business_interior_info[%0][%1]

#define GetBusinessMenuData(%0,%1,%2)		business_menu_info[%0][%1][%2]

#define GetBusinessPerkData(%0,%1)  		business_perks_info[%0][%1]

//==============================================================================
#define MAX_BUSINESS 						1000
#define BUSINESS_ENTER_PICKUP_MODEL_ID		19132
#define MAX_BUSINESS_MENU_POSITIONS 		10
#define PLAYER_PAYMENT_METHOD_LISTITEM 		777
#define BUSINESS_MAX_PRODUCTS 				2500
#define BUSINESS_DAY_TAX 					1000
#define BUSINESS_SELL_PERCENT 				0.7

#define BUSINESS_INFO_PICKUP_POS 			1157.83, -1751.56, 13.58
#define BUSINESS_INFO_PICKUP_VW 			0
#define BUSINESS_INFO_PICKUP_INT 			0

#define BUSINESS_MIN_PRODUCT_PRICE 			0
#define BUSINESS_MAX_PRODUCT_PRICE 			100

#define BUSINESS_PERKS_COUNT  				2

#define BUSINESS_ROBBERY_DEF_PERCENT 		15
#define BUSINESS_ROBBERY_SEC_PERCENT		5

#define BUSINESS_ROBBERY_MIN_SUM 			1000
#define BUSINESS_ROBBERY_MAX_SUM 			5000

//==============================================================================
enum
{
	BUSINESS_TYPE_NONE,
	BUSINESS_TYPE_PIZZA,
	BUSINESS_TYPES_COUNT
}

enum
{
	BUSINESS_PERK_SECURITY,
	BUSINESS_PERK_CASHBOX
}

//==============================================================================
new const business_type_names[BUSINESS_TYPES_COUNT][32] = {
	"None",
	"Пиццерия"
};
//==============================================================================
enum business_menu_info_struct
{
	bm_name[128],
	bm_item_id,
	bm_products_price
}
new const business_menu_info[BUSINESS_TYPES_COUNT][MAX_BUSINESS_MENU_POSITIONS][business_menu_info_struct] = {
	{
		{"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}, {"-", 0, 0}
	},
	{ /* Пиццерия */
		{"Пицца \"Маргарита\"", ITEM_PIZZA_ID, 2},
		{"Пицца \"Пепперони\"", ITEM_PIZZA_ID, 2},
		{"Пицца \"Гавайская\"", ITEM_PIZZA_ID, 2},
		{"Пицца \"Четыре сыра\"", ITEM_PIZZA_ID, 2},
		{"Пицца \"Карбонара\"", ITEM_PIZZA_ID, 2},
		{"Пицца \"Мясная\"", ITEM_PIZZA_ID, 2},
		{"Пицца \"Дьябло\"", ITEM_PIZZA_ID, 2},
		{"Безалкогольный газированный напиток \"Sprunk\"", ITEM_SPRUNK_ID, 1},
		{"Безалкогольный газированный напиток \"eCola\"", ITEM_ECOLA_ID, 1},
		{"Кофе", ITEM_COFFEE_ID, 1}
	}
};

//==============================================================================
enum business_interior_info_struct
{
	Float:bi_exit_pos_x,
	Float:bi_exit_pos_y,
	Float:bi_exit_pos_z,
	Float:bi_exit_pos_angle,
	bi_int,
	Float:bi_cashbox_pos_x,
	Float:bi_cashbox_pos_y,
	Float:bi_cashbox_pos_z,
	bi_cashbox_pickup_model_id,
	bi_cashbox_text[128],

	Float:bi_perks_x[BUSINESS_PERKS_COUNT],
	Float:bi_perks_y[BUSINESS_PERKS_COUNT],
	Float:bi_perks_z[BUSINESS_PERKS_COUNT],
	Float:bi_perks_angle[BUSINESS_PERKS_COUNT],
	bi_perks_skins[BUSINESS_PERKS_COUNT]
}
new const business_interior_info[BUSINESS_TYPES_COUNT][business_interior_info_struct] = {
	{0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0.0, 0, "None", {0.0}, {0.0}, {0.0}, {0.0}},

	{ /* Пиццерия */
		372.34, -133.42, 1001.49, 0.0,
		5,
		373.75, -118.80, 1001.49,
		1582,
		""cl_lime"[Меню пиццерии]\n"cl_grey"'Left ALT'",

		{374.7228, 373.7692},
		{-133.5174, -117.2784},
		{1001.4922, 1001.4922},
		{352.9149, 180.0},
		{164, 155}
	}
};

//==============================================================================
enum business_perks_info_struct
{
	bp_name[64],
	bp_description[256],
	bp_price,
	bp_anim_lib[64],
	bp_anim_name[64]
}
new const business_perks_info[BUSINESS_PERKS_COUNT][business_perks_info_struct] = {
	{"Охранник", "Снижает шанс ограбления до 5 процентов", 5000, "DEALER", "DEALER_IDLE"},
	{"Кассир", "Повышает шанс чаевых до 10 процентов", 10000, "", ""}
};

//==============================================================================
enum business_info_struct
{
	b_id,
	b_type,
	b_name[64],
	b_owner_id,
	Float:b_pos_x,
	Float:b_pos_y,
	Float:b_pos_z,
	b_vw,
	b_int,
	b_products_prices[MAX_BUSINESS_MENU_POSITIONS],
	b_products,
	b_balance,
	b_price,
	b_perks[BUSINESS_PERKS_COUNT],

	b_perks_actors_id[BUSINESS_PERKS_COUNT],
	Text3D:b_enter_text,
	b_enter_pickup,
	Text3D:b_exit_text,
	b_exit_pickup,
	Text3D:b_cashbox_text,
	b_cashbox_pickup
}
new business_info[MAX_BUSINESS][business_info_struct];