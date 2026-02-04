
#define Donate:%0(%1) 					Dnt_%0(%1)

//==============================================================================
#define DONATE_SHOP_CONVERT 			4

#define DONATE_SHOP_VEHICLES_COUNT 		38
#define DONATE_SHOP_UPGRADES_COUNT 		4
#define DONATE_SHOP_CASHBOXES_COUNT 	4
#define DONATE_SHOP_SKINS_COUNT 		8
#define DONATE_SHOP_CASES_COUNT 		1

//==============================================================================
enum
{
	RARITY_TYPE_COMMON,
	RARITY_TYPE_RARE,
	RARITY_TYPE_EPIC,
	RARITY_TYPE_LEGENDARY
}

enum
{
	CASE_PRIZE_TYPE_MONEY,
	CASE_PRIZE_TYPE_DONATE,
	CASE_PRIZE_TYPE_VEHICLE,
	CASE_PRIZE_TYPE_SKIN
}

enum
{
	CASE_INDEX_START,
}

//==============================================================================
enum
{
	DONATE_UPGRADE_LEVEL,
	DONATE_UPGRADE_LICENSES,
	DONATE_UPGRADE_WANTED,
	DONATE_UPGRADE_NAME,
}

enum donate_upgrades_info_struct
{
	du_name[32],
	du_price,
	du_discount
}
new const donate_upgrades_info[DONATE_SHOP_UPGRADES_COUNT][donate_upgrades_info_struct] = {
	{"Повышение уровня", 75, 0},
	{"Пакет лицензий", 700, 0},
	{"Снятие розыска", 200, 0},
	{"Смена ник-нейма", 100, 0}
};

//==============================================================================
enum donate_vehicle_info_struct
{
	dv_model_id,
	dv_price,
	dv_discount,
	dv_rarity,
	dv_image_url[32]
}
new const donate_vehicle_info[DONATE_SHOP_VEHICLES_COUNT][donate_vehicle_info_struct] = {
	{406, 5000, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/406.jpg"}, // Dumper
	{407, 3000, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/407.jpg"}, // Firetruck
	{408, 2800, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/408.jpg"}, // Trashmaster
	{409, 1200, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/409.jpg"}, // Stretch
	{416, 4500, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/416.jpg"}, // Ambulance
	{420, 600, 0, RARITY_TYPE_COMMON, "assets/donate/vehicles/420.jpg"}, // Taxi
	{423, 4000, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/423.jpg"}, // Mr Whoopee
	{424, 1600, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/424.jpg"}, // BF Injection
	{427, 4700, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/427.jpg"}, // Enforcer
	{428, 2200, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/428.jpg"}, // Securicar
	{431, 400, 0, RARITY_TYPE_COMMON, "assets/donate/vehicles/431.jpg"}, // Bus
	{433, 1100, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/433.jpg"}, // Barracks
	{434, 2500, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/434.jpg"}, // Hotknife
	{437, 400, 0, RARITY_TYPE_COMMON, "assets/donate/vehicles/437.jpg"}, // Coach
	{438, 700, 0, RARITY_TYPE_COMMON, "assets/donate/vehicles/438.jpg"}, // Cabbie
	{442, 1600, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/442.jpg"}, // Romero
	{443, 2100, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/443.jpg"}, // Packer
	{444, 6100, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/444.jpg"}, // Monster
	{448, 1000, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/448.jpg"}, // Pizzaboy
	{455, 1100, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/455.jpg"}, // Flatbed
	{456, 700, 0, RARITY_TYPE_COMMON, "assets/donate/vehicles/456.jpg"}, // Yankee
	{457, 2500, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/457.jpg"}, // Caddy
	{470, 3000, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/470.jpg"}, // Patriot
	{485, 1300, 0, RARITY_TYPE_RARE, "assets/donate/vehicles/485.jpg"}, // Baggage
	{486, 4000, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/486.jpg"}, // Dozer
	{490, 3400, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/490.jpg"}, // FBI Rancher
	{494, 3200, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/494.jpg"}, // Hotring Racer
	{504, 3000, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/504.jpg"}, // Bloodring Banger
	{524, 3900, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/524.jpg"}, // Cement Truck
	{525, 3000, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/525.jpg"}, // Towtruck
	{528, 5200, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/528.jpg"}, // FBI Truck
	{531, 4000, 30, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/531.jpg"}, // Tractor
	{532, 6100, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/532.jpg"}, // Combine Harvester
	{544, 3000, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/544.jpg"}, // Firetruck LA
	{552, 3000, 0, RARITY_TYPE_EPIC, "assets/donate/vehicles/552.jpg"}, // Utility Van
	{556, 6100, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/556.jpg"}, // Monster "A"
	{557, 6100, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/557.jpg"}, // Monster "B"
	{573, 4500, 0, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/573.jpg"} // Dune
};

//==============================================================================
enum donate_cashbox_info_struct
{
	dc_name[64],
	dc_price,
	dc_give_money
}
new const donate_cashbox_info[DONATE_SHOP_CASHBOXES_COUNT][donate_cashbox_info_struct] = {
	{"Стартовый набор", 100, 5000},
	{"Средний набор", 150, 10000},
	{"Премиум набор", 400, 30000},
	{"Элитный набор", 1000, 85000}
};

//==============================================================================
enum donate_skin_info_struct
{
	ds_name[64],
	ds_price,
	ds_discount,
	ds_model,
	ds_rarity,
	ds_image_url[32]
}
new const donate_skin_info[DONATE_SHOP_SKINS_COUNT][donate_skin_info_struct] = {
	{"Дон Карлеоне", 2000, 0, 313, RARITY_TYPE_LEGENDARY, "assets/donate/skins/313.png"},
	{"Сэм Винчестер", 300, 0, 314, RARITY_TYPE_COMMON, "assets/donate/skins/314.png"},
	{"Дин Винчестер", 300, 0, 315, RARITY_TYPE_COMMON, "assets/donate/skins/315.png"},
	{"Винсент Кассель", 100, 0, 316, RARITY_TYPE_COMMON, "assets/donate/skins/316.png"},
	{"Злобный француз", 450, 0, 317, RARITY_TYPE_RARE, "assets/donate/skins/317.png"},
	{"Пеннивайз", 3500, 15, 318, RARITY_TYPE_LEGENDARY, "assets/donate/skins/318.png"},
	{"Дональд Трамп", 1500, 0, 319, RARITY_TYPE_EPIC, "assets/donate/skins/319.png"},
	{"Трэвис Скотт", 1250, 0, 320, RARITY_TYPE_EPIC, "assets/donate/skins/320.png"}
};

//==============================================================================
enum donate_case_info_struct
{
	dci_name[64],
	dci_price,
	dci_discount,
	dci_image_url[32]
}
new const donate_case_info[DONATE_SHOP_CASES_COUNT][donate_case_info_struct] = {
	{"Кейс \"Стартовый\"", 150, 10, "assets/donate/case.png"}
};

enum donate_case_prize_info_struct
{
	dcp_type,
	dcp_count,
	dcp_rarity,
	dcp_image_url[32],
	Float:dcp_chance,
	dcp_case_index,
}

new const donate_case_prize_info[][donate_case_prize_info_struct] = {
	// --- Кейс "Стартовый"
	{CASE_PRIZE_TYPE_MONEY, 3500, RARITY_TYPE_COMMON, "assets/donate/money.png", 15.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_MONEY, 4500, RARITY_TYPE_COMMON, "assets/donate/money.png", 15.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_MONEY, 5500, RARITY_TYPE_RARE, "assets/donate/money.png", 10.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_MONEY, 6500, RARITY_TYPE_RARE, "assets/donate/money.png", 10.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_MONEY, 10000, RARITY_TYPE_LEGENDARY, "assets/donate/money.png", 5.0, CASE_INDEX_START},

	{CASE_PRIZE_TYPE_DONATE, 400, RARITY_TYPE_RARE, "assets/donate/coin.png", 9.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_DONATE, 600, RARITY_TYPE_RARE, "assets/donate/coin.png", 9.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_DONATE, 800, RARITY_TYPE_EPIC, "assets/donate/coin.png", 6.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_DONATE, 1500, RARITY_TYPE_LEGENDARY, "assets/donate/coin.png", 4.0, CASE_INDEX_START},

	{CASE_PRIZE_TYPE_SKIN, 316, RARITY_TYPE_COMMON, "assets/donate/skins/316.png", 4.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_SKIN, 314, RARITY_TYPE_RARE, "assets/donate/skins/314.png", 3.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_SKIN, 315, RARITY_TYPE_RARE, "assets/donate/skins/315.png", 3.0, CASE_INDEX_START},

	{CASE_PRIZE_TYPE_VEHICLE, 401, RARITY_TYPE_EPIC, "assets/donate/vehicles/401.jpg", 2.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_VEHICLE, 410, RARITY_TYPE_EPIC, "assets/donate/vehicles/410.jpg", 2.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_VEHICLE, 422, RARITY_TYPE_EPIC, "assets/donate/vehicles/422.jpg", 2.0, CASE_INDEX_START},
	{CASE_PRIZE_TYPE_VEHICLE, 426, RARITY_TYPE_LEGENDARY, "assets/donate/vehicles/426.jpg", 1.0, CASE_INDEX_START}
	//--------------------------------------------------------------------------
};

new const DONATE_SHOP_CASE_PRIZES_COUNT = sizeof(donate_case_prize_info);