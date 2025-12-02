
/*
Используемые PVar`ы:
	Business_Index

*/

#define Business:%0(%1) 					Bsnss_%0(%1)

//==============================================================================
#define GetBusinessData(%0,%1) 				business_info[%0][%1]
#define SetBusinessData(%0,%1,%2) 			business_info[%0][%1] = %2
#define AddBusinessData(%0,%1,%2) 			business_info[%0][%1] += %2

#define GetBusinessInteriorData(%0,%1) 		business_interior_info[%0][%1]

#define GetBusinessMenuData(%0,%1,%2)		business_menu_info[%0][%1][%2]

//==============================================================================
#define MAX_BUSINESS 						1000
#define BUSINESS_ENTER_PICKUP_MODEL_ID		19132
#define MAX_BUSINESS_MENU_POSITIONS 		10
#define PLAYER_PAYMENT_METHOD_LISTITEM 		777

#define BUSINESS_INFO_PICKUP_POS 			1157.83, -1751.56, 13.58
#define BUSINESS_INFO_PICKUP_VW 			0
#define BUSINESS_INFO_PICKUP_INT 			0

//==============================================================================
enum
{
	BUSINESS_TYPE_NONE,
	BUSINESS_TYPE_PIZZA,
	BUSINESS_TYPES_COUNT
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
}
new const business_interior_info[BUSINESS_TYPES_COUNT][business_interior_info_struct] = {
	{0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0.0, 0, "None"},

	{ /* Пиццерия */
		372.34, -133.42, 1001.49, 0.0,
		5,
		373.75, -118.80, 1001.49,
		1582,
		""cl_lime"[Меню пиццерии]\n"cl_grey"'Left ALT'"
	}
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

	Text3D:b_enter_text,
	b_enter_pickup,
	Text3D:b_exit_text,
	b_exit_pickup,
	Text3D:b_cashbox_text,
	b_cashbox_pickup,
}
new business_info[MAX_BUSINESS][business_info_struct];