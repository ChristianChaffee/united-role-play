
/*
Используемые PVar`ы:
	Business_Index
	Business_MenuPositionIndex
	Business_PerkIndex

	Business_BuyIndex
	Business_SellPrice
	Business_BuyPlayerid
	Business_BuyTime
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

#define BUSINESS_TIPS_DEF_PERCENT 			5
#define BUSINESS_TIPS_CASH_PERCENT 			10

#define BUSINESS_TIPS_MIN_SUM 				1
#define BUSINESS_TIPS_MAX_SUM 				5

//==============================================================================
enum
{
	BUSINESS_TYPE_NONE,
	BUSINESS_TYPE_PIZZA,
	BUSINESS_TYPE_BURGER,
	BUSINESS_TYPE_FASTFOOD,
	BUSINESS_TYPE_SHOP,
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
	"Пиццерия",
	"Бургерная",
	"Фастфуд-ресторан",
	"Магазин 24/7"
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
	},

	{ /* Бургерная */
		{"Чизбургер", ITEM_CHEESBURGER_ID, 2},
		{"Гамбургер", ITEM_HAMBURGER_ID, 2},
		{"Бургер с беконом", ITEM_HAMBURGER_ID, 2},
		{"Чили-бургер", ITEM_HAMBURGER_ID, 2},
		{"Дабл Чизбургер", ITEM_HAMBURGER_ID, 2},
		{"Веганский бургер", ITEM_HAMBURGER_ID, 2},
		{"Фишбургер", ITEM_HAMBURGER_ID, 2},
		{"Безалкогольный газированный напиток \"Sprunk\"", ITEM_SPRUNK_ID, 1},
		{"Безалкогольный газированный напиток \"eCola\"", ITEM_ECOLA_ID, 1},
		{"Кофе", ITEM_COFFEE_ID, 1}
	},

	{ /* Фастфуд-ресторан */
		{"Чизбургер", ITEM_CHEESBURGER_ID, 2},
		{"Гамбургер", ITEM_HAMBURGER_ID, 2},
		{"Картофель фри", ITEM_FRIED_POTATO_ID, 2},
		{"Пицца \"Пепперони\"", ITEM_PIZZA_ID, 2},
		{"Куриные наггетсы", ITEM_NUGGETS_ID, 2},
		{"Шаурма", ITEM_KEBAB_ID, 2},
		{"Хот-дог", ITEM_HOT_DOG_ID, 2},
		{"Безалкогольный газированный напиток \"Sprunk\"", ITEM_SPRUNK_ID, 1},
		{"Безалкогольный газированный напиток \"eCola\"", ITEM_ECOLA_ID, 1},
		{"Кофе", ITEM_COFFEE_ID, 1}
	},

	{ /* Магазин 24/7 */
		{"Аптечка", ITEM_MED_KIT_ID, 2},
		{"Телефон", ITEM_PHONE_ID, 2},
		{"SIM-карта", ITEM_SIM_CARD_ID, 2},
		{"Маска", ITEM_MASK_ID, 2},
		{"Фотоаппарат (50 снимков)", ITEM_CAMERA_ID, 10},
		{"Букет цветов", ITEM_FLOWERS_ID, 10},
		{"Безалкогольный газированный напиток \"Sprunk\"", ITEM_SPRUNK_ID, 2},
		{"Безалкогольный газированный напиток \"eCola\"", ITEM_ECOLA_ID, 2},
		{"-", 0, 0},
		{"-", 0, 0}
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
	},

	{ /* Бургерная */
		362.8493, -75.1439, 1001.5078, 315.6623,
		10,
		376.4931, -67.4347, 1001.5078,
		2703,
		""cl_lime"[Меню бургерной]\n"cl_grey"'Left ALT'",

		{379.4852, 376.4754},
		{-67.4879, -65.8490},
		{1001.5078, 1001.5078},
		{86.6623, 180.0},
		{164, 205}
	},

	{ /* Фастфуд-ресторан */
		364.9359, -11.8011, 1001.8516, 0.0,
		9,
		368.1276, -6.0168, 1001.8516,
		2769,
		""cl_lime"[Меню фастфуд-ресторана]\n"cl_grey"'Left ALT'",

		{363.8204, 368.0962},
		{-7.4535, -4.4925},
		{1001.8516, 1001.8516},
		{270.0, 180.0},
		{164, 167}
	},

	{ /* Магазин 24/7 */
		6.0862, -31.7568, 1003.5494, 0.0,
		10,
		3.2734, -29.0138, 1003.5494,
		1274,
		""cl_lime"[Список товаров 24/7]\n"cl_grey"'Left ALT'",

		{8.4494, 3.1430},
		{-31.5018, -30.7013},
		{1003.5494, 1003.5494},
		{0.0, 0.0},
		{164, 210}
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