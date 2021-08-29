#include "dict.h"
#include "interface.h"

#include <stdlib.h>
#include <string.h>

DICT* DictCreate(char* name)
{
	DICT* d = (DICT*)malloc(sizeof(DICT));
	if (!d) exit(EXIT_FAILURE);

	char* string = (char*)calloc(25, sizeof(char));
	if (!string) exit(EXIT_FAILURE);
	memcpy(string, name, 25);

	d->name = string;
	d->actual_size = 0;
	d->max_size = 10;

	d->items = (ITEM**)calloc(10, sizeof(ITEM*));
	if (!d->items) exit(EXIT_FAILURE);

	return d;
}

void DictExpand(DICT* d)
{
	ITEM** old_items = d->items;
	ITEM** new_items = (ITEM**)calloc(d->max_size << 1, sizeof(ITEM*));
	if (!new_items) exit(EXIT_FAILURE);

	for (int i = 0; i < d->max_size; i++)
		new_items[i] = old_items[i];

	d->max_size <<= 1;
	free(old_items);
	d->items = new_items;
}

int ConvertStrToInt(char* string)
{
	int N = 0, i = 0, c = 1;
	while ((int)string[i] == 32) i++;
	if (string[i] == 45)
	{
		c = -1;
		i++;
	}
	while ((int)string[i])
	{
		if ((int)string[i] < 48 || (int)string[i] > 57) return INT_MAX;
		N = N * 10 + (int)string[i++] - 48;
	}

	return N * c;
}

void DictItemPut(DICT* d, char* key, char* val)
{
	if (d->actual_size == d->max_size) DictExpand(d);

	// Value initialization
	int value_i = ConvertStrToInt(val);
	double value_d = 0;
	int flag = 0;
	if (value_i != INT_MAX && (strlen(val) < 10))
		flag = INTEGER;
	else
	{
		value_d = atof(val); // atof() : string -> double
		if (value_d != 0 && (strlen(val) < 17))
			flag = _DOUBLE_;
		else
			flag = STRING;
	}


	// Create or change item
	ITEM* item = NULL;
	for (int i = 0; i < d->actual_size; i++)
		if (!strcmp(d->items[i]->key, key))
		{
			item = d->items[i];
			break;
		}

	if (item == NULL)
	{
		item = (ITEM*)malloc(sizeof(ITEM));
		if (!item) exit(EXIT_FAILURE);

		item->key = (char*)calloc(strlen(key) + 1, sizeof(char));
		if (!item->key) exit(EXIT_FAILURE);
		memcpy(item->key, key, strlen(key));

		item->type_val = flag;
		if (flag == STRING)
		{
			item->str = (char*)calloc(strlen(val) + 1, sizeof(char));
			if (!item->str) exit(EXIT_FAILURE);
			memcpy(item->str, val, strlen(val));
		}
		if (flag == INTEGER)
			item->num = value_i;
		if (flag == _DOUBLE_)
			item->f_num = value_d;

		d->items[d->actual_size++] = item;
	}
	else
	{
		item->type_val = flag;
		if (flag == STRING)
			memcpy(item->str, val, strlen(val));
		if (flag == INTEGER)
			item->num = value_i;
		if (flag == _DOUBLE_)
			item->f_num = value_d;
	}

}

ITEM* DictItemGet(DICT* d, char* key)
{
	for (int i = 0; i < d->actual_size; i++)
		if (!strcmp(d->items[i]->key, key))
			return d->items[i];
	return NULL;
}

int DictItemDel(DICT* d, char* key)
{
	ITEM* item = DictItemGet(d, key);
	if (item == NULL) return 1;

	int pos = 0;
	while (d->items[pos] != item) pos++;

	int f = 1;
	if (pos == d->actual_size - 1) f = 0;

	if (item->type_val == STRING)
		free(item->str);
	free(item->key);
	free(item);
	d->items[pos] = NULL;

	if (f)
	{
		ITEM* last = d->items[d->actual_size - 1];
		d->items[pos] = last;
		d->items[d->actual_size - 1] = NULL;
	}
	d->actual_size--;
	return 0;
}

void DictDestroy(DICT* d)
{
	for (int i = 0; i < d->actual_size; i++)
		DictItemDel(d, d->items[i]->key);
	free(d->name);
	free(d->items);
	free(d);
}

char** DictKeysGet(DICT* d)
{
	char** arr = (char**)calloc(d->actual_size, sizeof(char*));
	if (!arr) exit(EXIT_FAILURE);
	for (int i = 0; i < d->actual_size; i++)
		arr[i] = d->items[i]->key;
	return arr;
}

void** DictValsGet(DICT* d, int type)
{
	int c = 0;
	if (type == STRING)
	{
		char** arr = (char**)calloc(d->actual_size + 1, sizeof(char*));
		if (!arr) exit(EXIT_FAILURE);
		for (int i = 0; i < d->actual_size; i++)
			if (d->items[i]->type_val == STRING)
				arr[c++] = d->items[i]->str;
		return arr;
	}
	else if (type == INTEGER)
	{
		int** arr = (int**)calloc(d->actual_size + 1, sizeof(int*));
		if (!arr) exit(EXIT_FAILURE);
		for (int i = 0; i < d->actual_size; i++)
			if (d->items[i]->type_val == INTEGER)
				arr[c++] = &d->items[i]->num;
		return arr;
	}
	else if (type == _DOUBLE_)
	{
		double** arr = (double**)calloc(d->actual_size + 1, sizeof(double*));
		if (!arr) exit(EXIT_FAILURE);
		for (int i = 0; i < d->actual_size; i++)
			if (d->items[i]->type_val == _DOUBLE_)
				arr[c++] = &d->items[i]->f_num;
		return arr;
	}
	return NULL;
}
