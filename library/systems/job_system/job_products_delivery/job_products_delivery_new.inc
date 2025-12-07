
/*
Используемые PVar`ы:

	JobProductsDelivery_RentVehicleID
	JobProductsDelivery_ProductsCount
	JobProductsDelivery_OrderIndex

*/

//==============================================================================
#define GetProdDeliveryData(%0,%1) 		prod_delivery_order_info[%0][%1]
#define SetProdDeliveryData(%0,%1,%2)	prod_delivery_order_info[%0][%1] = %2
#define AddProdDeliveryData(%0,%1,%2) 	prod_delivery_order_info[%0][%1] += %2

//==============================================================================
#define MAX_PRODUCTS_DELIVERY_ORDERS  	1000
#define JOB_PD_MIN_PRICE 				1
#define JOB_PD_MAX_PRICE				5
#define JOB_PD_CHANGE_PRICE_TIME 		600
#define JOB_PD_ACTOR_ID 				14
#define JOB_PD_CAR_RENT_PRICE 			10
#define JOB_PD_MAX_TRUCK_PRODUCTS 		1000

#define JOB_PD_LOAD_POS 				-23.04, -272.87, 5.43

//==============================================================================
new job_products_delivery_price,
	job_products_change_price_time,
	Text3D:job_products_delivery_text;

//==============================================================================
enum prod_delivery_order_info_struc
{
	pd_order_business_index,
	pd_order_count,
	pd_order_price
}

new prod_delivery_order_info[MAX_PRODUCTS_DELIVERY_ORDERS][prod_delivery_order_info_struc];