/*
 * OTA_Object_Int.c
 *
 *  Created on: Aug 25, 2011
 *      Author: wlai
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <OTA_Def.h>



long get_Int_value (OTA_Object* p)
{
	long ret;
	
	if (p == NULL)
		return 0;

	ret = p->payload.obj_int.obj_data;
	return ret;

}
void set_Int_value(OTA_Object* p, long data)
{
	int datasize = 8;
	unsigned long udata = (unsigned long) data;

	if(p == NULL)
		return;

	if(p->objtype == _OBJTYPE_INT)
	{

		if (data >= CHAR_MIN && data <= CHAR_MAX)
			datasize = 1;
		else if (data >= SHRT_MIN && data <= SHRT_MAX)
			datasize = 2;
		else if (data >= INT_MIN && data <= INT_MAX)
			datasize = 4;
	}
    /* for unsigned int case */
	else
	{
		if(udata <= UCHAR_MAX)
			datasize = 1;
		else if (udata <= USHRT_MAX)
			datasize = 2;
		else if (udata <= UINT_MAX)
			datasize = 4;

	}

	p->payload.obj_int.data_size = datasize;
	p->payload.obj_int.obj_data = data;


}


void Object_Int_print(OBJ_INT* p)
{

	printf("\t\t<INT SIZE = %d/>\n", p->data_size);
	printf("\t\t<VALUE = %d/>\n", (int)( p->obj_data));

}

void Object_Int_print_unsigned(OBJ_INT* p)
{

	unsigned int temp = p->obj_data;
	printf("\t\t<INT SIZE = %d/>\n", p->data_size);
	printf("\t\t<VALUE = %ld/>\n", temp);

}


void populate_Obj_Int_socket (OTA_Object* p, int socketfd, int flags)
{

	UInt16 d16 = 0;
	UInt32 d32 = 0;
	ULong  d64 = 0;
	BYTE size = 0;
	BYTE  byte = 0;

	// reading byte size
	int n  = recv(socketfd, &size, 1, flags);

	switch (size)
	    {
	    case 1:
	    	n = recv(socketfd, &byte, 1, flags);
			p->payload.obj_int.obj_data = byte;
			break;
		case 2:
			n = recv(socketfd, &d16, 2, flags);
			reverseBytes((BYTE*)&d16, 2);
			p->payload.obj_int.obj_data = d16;
			break;
		case 4:
			n = recv(socketfd, &d32, 2, flags);
			reverseBytes((BYTE*)&d32, 4);
			p->payload.obj_int.obj_data = d32;
			break;
		case 8:
			n = recv(socketfd, &d64, 8, flags);
			reverseBytes((BYTE*)&d64, 8);
			p->payload.obj_int.obj_data = d64;
			break;
	    }
	return;
}

void populate_Obj_Int (OTA_Object* p, BYTE* data)
{
	int offset = 0;
	UInt16 d16 = 0;
	UInt32 d32 = 0;
	ULong  d64 = 0;

	p->payload.obj_int.data_size = data[offset++];
    switch (p->payload.obj_int.data_size)
    {
    case 1:
		p->payload.obj_int.obj_data = (char) data[offset++];
		break;
	case 2:
		memcpy(&d16, &(data[offset]), 2);
		reverseBytes((BYTE*)&d16, 2);
		p->payload.obj_int.obj_data = (short) d16;
		break;
	case 4:
		memcpy(&d32, &(data[offset]), 4);
		reverseBytes((BYTE*)&d32, 4);
		p->payload.obj_int.obj_data = (int)d32;
		break;
	case 8:
		memcpy(&d64, &(data[offset]), 8);
		reverseBytes((BYTE*)&d64, 8);
		p->payload.obj_int.obj_data = (long) d64;
		break;
    }

    return;

}

BYTEARRAY* Int_to_BYTES (OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	UInt16 d16 = 0;
	UInt32 d32 = 0;
	ULong d64  = 0;
	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;
	


    array->data[offset++] = p->payload.obj_int.data_size;
	switch (p->payload.obj_int.data_size) {
	case 1:
		array->data[offset] = p->payload.obj_int.obj_data;
		break;

	case 2:
		d16 = p->payload.obj_int.obj_data;
		reverseBytes((BYTE*) &d16, 2);
		memcpy(&(array->data[offset]), &d16, p->payload.obj_int.data_size);
		break;
	case 4:
		d32 = p->payload.obj_int.obj_data;
		reverseBytes((BYTE*)&d32, 4);
		memcpy(&(array->data[offset]), &d32, p->payload.obj_int.data_size);
		break;
	case 8:
		d64 = p->payload.obj_int.obj_data;
		reverseBytes((BYTE*)&d64, 8);
		memcpy(&(array->data[offset]), &d64, p->payload.obj_int.data_size);
		break;
	}

	return array;


}
