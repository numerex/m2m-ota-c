/*
 * Obj_Data.c
 *
 *  Created on: Aug 25, 2011
 *      Author: wlai
 */


#include <stdio.h>
#include <OTA_Def.h>
#ifndef EMBEDDED
#include <time.h>
#endif
/*
#include <OTA_Object_Def.h>
#include <OTA_Msg_Def.h>
*/
void Obj_Data_print(BYTEARRAY* p);
void delete_Object_Data(BYTEARRAY* p);
void append_array(BYTEARRAY* p, BYTEARRAY* d);
void put_in_array(BYTEARRAY* p, BYTEARRAY* d, int offset);

BYTEARRAY* new_BYTEARRAY( int size)
{

	BYTEARRAY* p = malloc (sizeof(BYTEARRAY));
	p->data = malloc (size);
	p->size = size;
	p->print = Obj_Data_print;
	p->Delete = delete_Object_Data;
	p->append = append_array;
	p->put = put_in_array;
	return p;
}

void put_in_array(BYTEARRAY* p, BYTEARRAY* d, int offset)
{

	if(p == NULL || d == NULL)
		return;

	if(p->size - offset < d->size){
#ifdef SXL_DEBUG_PRINTF
		printf("going to puke!");
#endif
	//	__nop();
		asm("nop");
		return;
	}
	memcpy(&(p->data[offset]), d->data, d->size);
}



void append_array(BYTEARRAY* p, BYTEARRAY* d)
{
	int new_size;
	BYTE* buff;

	if(p == NULL || d == NULL)
		return;
	 new_size = p->size + d->size;

	 buff = malloc( new_size);
	 memcpy(buff, p->data, p->size);
     memcpy(&(buff[p->size]), d->data, d->size);
	 p->size = new_size;
	 free(p->data);
	 p->data = buff;

}


void Obj_Data_print(BYTEARRAY* p)
{
	int i=0;

    printf("<BYTES>\n");
	printf("\t<Size=%d/>\n", p->size);
	printf("\t<Hex> ");

	for(i=0; i<p->size; i++)
	{
		printf("%02X", p->data[i]);
		if(i != (p->size -1))
			printf(", ");
	}

	printf(" </Hex>\n");

	printf("</BYTES>\n");

}



void delete_Object_Data(BYTEARRAY* p)
{
	if(p == NULL)
		return;
	if(p->data != NULL)
		free(p->data);
	free(p);
	p = NULL;
}





