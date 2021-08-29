#ifndef DUMB_DICT_H

#define DUMB_DICT_H

typedef struct item // size = 16 byte
{
	char* key;
	enum Flag { STRING, INTEGER, _DOUBLE_ } type_val;
	union
	{
		  char* str; 
		   int  num;     
		double  f_num;
	};
}ITEM;

typedef struct dict  // size = 16 byte
{	
	char*  name;
	 int   actual_size;
	 int   max_size;
	ITEM** items;
}DICT;

DICT* DictCreate  (char* name);
void  DictExpand  (DICT* d);
void  DictDestroy (DICT* d);

void  DictItemPut (DICT* d, char* key, char* val);
int   DictItemDel (DICT* d, char* key);
ITEM* DictItemGet (DICT* d, char* key);

char** DictKeysGet (DICT* d);
void** DictValsGet (DICT* d, int type);


#endif



