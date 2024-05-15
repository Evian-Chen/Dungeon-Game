#include "Shop.h"

void Shop::listAllItem()
{
	for (auto& item : items)
	{
		std::cout << item.first << '\t' << item.second << '\n';
	}
}

void Shop::buyItem(Role& role, string item)
{
	int avalibleMoney = role.money;
	if (avalibleMoney >= items[item])
	{
		role.updatePack(item);
	}
}