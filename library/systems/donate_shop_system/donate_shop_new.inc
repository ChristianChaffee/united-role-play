
#define Donate:%0(%1) 					Dnt_%0(%1)

//==============================================================================
#define DONATE_SHOP_CONVERT 			4
#define DONATE_SHOP_VEHICLES_COUNT 		38

//==============================================================================
enum
{
	RARITY_TYPE_COMMON,
	RARITY_TYPE_RARE,
	RARITY_TYPE_EPIC,
	RARITY_TYPE_LEGENDARY
}

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