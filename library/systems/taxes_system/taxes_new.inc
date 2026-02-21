
/*
Используемые PVar`ы:
	Taxes_AddNewListitem
	Taxes_AddName
	Taxes_AddRequriedLevel
	Taxes_AddSum
	Taxes_AddTimeType
	Taxes_AddType
	Taxes_AddIsOffline
	Taxes_TaxIndex
*/

//==============================================================================
#define Taxes:%0(%1)					Txs_%0(%1)

//==============================================================================
#define GetTaxData(%0,%1)  				tax_info[%0][%1]
#define SetTaxData(%0,%1,%2)  			tax_info[%0][%1] = %2
#define AddTaxData(%0,%1,%2) 			tax_info[%0][%1] += %2

//==============================================================================
#define MAX_TAXES  						20
#define TAX_PAYMENT_TIME 				30*86400

//==============================================================================
enum
{
	TAX_TYPE_DEFAULT,
	TAX_TYPE_PAY_PERCENT,
	TAX_TYPE_VEHICLE,
	TAX_TYPES_COUNT
}
new const tax_type_names[TAX_TYPES_COUNT][32] = {
	"Обычный налог",
	"Процент от дохода",
	"Налог за личный транспорт"
};

enum
{
	TAX_TIME_TYPE_PAYDAY,
	TAX_TIME_TYPE_DAY,
	TAX_TIME_TYPES_COUNT
}
new const tax_type_time_names[TAX_TIME_TYPES_COUNT][32] = {
	"Раз в PayDay",
	"Раз в день"
};

//==============================================================================
enum tax_info_struct
{
	t_id,
	t_name[64],
	t_requried_level,
	t_sum,
	t_time_type,
	t_type,
	t_is_offline
}
new tax_info[MAX_TAXES][tax_info_struct];
new const tax_info_default[tax_info_struct] = {
	0,
	"-",
	0,
	0,
	0,
	0,
	0
};