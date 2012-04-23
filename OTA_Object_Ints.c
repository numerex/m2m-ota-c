/*
 * OTA_Object_Int_Array.c
 *
 *  Created on: Sep 6, 2011
 *      Author: wlai
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


#include <OTA_Def.h>
int (*array_size) (struct Object*);
long* (*array) (struct Object*);


int get_Int_array_size (OTA_Object* p)
{
	if (p == NULL)
		return 0;

	if(p->payload.obj_ints.element_size == 0)
		return 0;

	return (int)(p->payload.obj_ints.data_size/p->payload.obj_ints.element_size);
}


long* get_Int_Array (OTA_Object* p)
{

	int array_size;
	int intsize;
	long* l;
	int i = 0;
	short i16;
	int i32;
	long i64;

	if (p == NULL)
			return 0;
	array_size = p->payload.obj_ints.array_size(p);
	intsize = p->payload.obj_ints.element_size;
	l = malloc (sizeof(long) * array_size);


	for (i=0; i<array_size; i++)
	{
		switch (intsize)
		{
		case 1:
				l[i] = (short) p->payload.obj_ints.obj_data[i];
				break;
		case 2:
				memcpy(&i16, &(p->payload.obj_ints.obj_data[i*intsize]), intsize);
				l[i] = i16;
				break;
		case 4:
				memcpy(&i32, &(p->payload.obj_ints.obj_data[i*intsize]), intsize);
				l[i] = i32;
				break;

		case 8:
			memcpy(&i64, &(p->payload.obj_ints.obj_data[i*intsize]), intsize);
				l[i] = i64;
				break;
		}
	}
	return l;
}

void set_Ints_value(OTA_Object*p, BYTE* data, int size, int elementsize)
{
	if(p == NULL)
			return;

	p->payload.obj_ints.data_size = size * elementsize;
	p->payload.obj_ints.element_size = elementsize;
	p->payload.obj_ints.obj_data = malloc(size*elementsize);

	memcpy(p->payload.obj_ints.obj_data, data, size*elementsize);

}

void set_Ints_value_1(OTA_Object* p, BYTE* data, int size)
{
	set_Ints_value(p, data, size, 1);
	return;
}
void set_Ints_value_2(OTA_Object* p, UInt16* data, int size)
{

	UInt16* temp = malloc(size*2);
	memcpy(temp, data, size*2);

	set_Ints_value( p, (BYTE*) temp, size, 2);
	return;
}


void set_Ints_value_4(OTA_Object* p, UInt32* data, int size )
{
	UInt32* temp = malloc(size*4);
	memcpy(temp, data, size*4);

	set_Ints_value(p, (BYTE*) data, size, 4);
	return;
}



void set_Ints_value_8(OTA_Object* p, ULong*  data, int size)
{
	ULong* temp = malloc(size*8);
	memcpy(temp, data, size*8);
	set_Ints_value(p, (BYTE*) data, size, 8);
	return;
}



void Object_Ints_print(OBJ_INT_ARRAY* p)
{

	
	short d1=0;
    int d2 = 0;
    long d3 = 0;
	int i=0;

	printf("\t\t<INT SIZE = %d/>\n", p->data_size);
	printf("\t\t<ELEMENT SIZE = %d/>\n", p->element_size);
	
	if(p->element_size == 0)
		return;
    

	for(i=0; i<p->data_size/p->element_size; i++)
	{
		switch (p->element_size)
		{
		case 1:
			printf("\t\t\t<INT[%d] = %d/>\n", i, (char) p->obj_data[i]);
			break;
		case 2:
			memcpy(&d1, &(p->obj_data[i*p->element_size]), p->element_size);
			printf("\t\t\t<INT[%d] = %d/>\n", i, d1);
			break;
		case 4:
			memcpy(&d2, &(p->obj_data[i*p->element_size]), p->element_size);
			printf("\t\t\t<INT[%d] = %d/>\n", i, d2);
			break;
		case 8:
			memcpy(&d3, &(p->obj_data[i*p->element_size]), p->element_size);
			printf("\t\t\t<INT[%d] = %ld/>\n", i, d3);
			break;
		}
	}

}



void Object_Ints_print_unsigned(OBJ_INT_ARRAY* p)
{


	short d1=0;
    int d2 = 0;
    long d3 = 0;
	int i=0;

	printf("\t\t<INT SIZE = %d/>\n", p->data_size);
	printf("\t\t<ELEMENT SIZE = %d/>\n", p->element_size);

	if(p->element_size == 0)
		return;


	for(i=0; i<p->data_size/p->element_size; i++)
	{
		switch (p->element_size)
		{
		case 1:
			printf("\t\t\t<INT[%d] = %d/>\n", i, (unsigned char) p->obj_data[i]);
			break;
		case 2:
			memcpy(&d1, &(p->obj_data[i*p->element_size]), p->element_size);
			printf("\t\t\t<INT[%d] = %d/>\n", i, (unsigned short) d1);
			break;
		case 4:
			memcpy(&d2, &(p->obj_data[i*p->element_size]), p->element_size);
			printf("\t\t\t<INT[%d] = %d/>\n", i, (unsigned int) d2);
			break;
		case 8:
			memcpy(&d3, &(p->obj_data[i*p->element_size]), p->element_size);
			printf("\t\t\t<INT[%d] = %ld/>\n", i, (unsigned long) d3);
			break;
		}
	}

}
void populate_Obj_Ints_socket (OTA_Object* p, int socketfd, int flags)
{


	BYTE elementsize =  0;
	UInt16 datasize = 0;
	UInt16 d16;
	BYTE d8;
	UInt32 d32 = 0;
	ULong  d64 = 0;
	BYTE* buff;
	int i=0;



	// reading byte size
	int n  = recv(socketfd, &datasize, 2, flags);
	reverseBytes((BYTE*)&datasize, 2);
	p->payload.obj_ints.data_size = datasize;

	n = recv(socketfd, &elementsize, 1, flags);
	p->payload.obj_ints.element_size = elementsize;

	if(datasize == 0 || elementsize== 0)
	{
		p->payload.obj_ints.obj_data = NULL;
		return;
	}

	// total size
	buff = malloc(datasize);

	
	for(i=0; i< datasize/elementsize; i++)
	{
	switch (elementsize)
	    {
	    case 1:
	    	n = recv(socketfd, &d8, 1, flags);
	    	buff[i] = d8;
			break;
		case 2:
			n = recv(socketfd, &d16, 2 , flags);
			reverseBytes((BYTE *)&d16, 2);
			memcpy(&(buff[i*elementsize]), &d16, 2);
			break;
		case 4:
			n = recv(socketfd, &d32, 4, flags);
			reverseBytes((BYTE*)&d32, 4);
			memcpy(&(buff[i*elementsize]), &d32, 4);
			break;
		case 8:
			n = recv(socketfd, &d64, 8, flags);
			reverseBytes((BYTE*)&d64, 8);
			memcpy(&(buff[i*elementsize]), &d64, 8);
			break;
	    }
	}
	p->payload.obj_ints.obj_data = buff;
	return;
}

void populate_Obj_Ints (OTA_Object* p, BYTE* data)
{
	int offset = 0;
	UInt16 d16 = 0;
	UInt32 d32 = 0;
	ULong  d64 = 0;
	UInt16 datasize;
	BYTE   elementsize;
		int i;

	memcpy(&datasize, data, 2);
	offset += 2;
	reverseBytes((BYTE*)&datasize, 2);

	p->payload.obj_ints.data_size = datasize;
	p->payload.obj_ints.element_size = data[offset++];
	elementsize = p->payload.obj_ints.element_size;

	if(datasize==0 || elementsize == 0)
	{
		p->payload.obj_ints.obj_data = NULL;
		return;
	}

	p->payload.obj_ints.obj_data = malloc (datasize);


	for (i=0;i< p->payload.obj_ints.data_size/p->payload.obj_ints.element_size;i++)
	{
			switch (p->payload.obj_ints.element_size)
			{
			case 1:
				p->payload.obj_ints.obj_data[i] = (BYTE) data[i+offset];
				break;
			case 2:
				memcpy(&d16, &(data[i*elementsize + offset]), 2);
				reverseBytes((BYTE*)&d16, 2);
				memcpy(&(p->payload.obj_ints.obj_data[i*elementsize]), &d16, 2);
				break;
			case 4:
				memcpy(&d32, &(data[i*elementsize + offset]), 4);
				reverseBytes((BYTE*)&d32, 4);
				memcpy(&(p->payload.obj_ints.obj_data[i*elementsize]), &d32, 4);
				break;
			case 8:
				memcpy(&d64, &(data[i*elementsize + offset]), 8);
				reverseBytes((BYTE*)&d64, 8);
				memcpy(&(p->payload.obj_ints.obj_data[i*elementsize]), &d64, 8);
				break;
			}
		}
    return;

}

BYTEARRAY* Ints_to_BYTES (OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	UInt16 d16 = 0;
	UInt32 d32 = 0;
	ULong d64  = 0;
	UInt16 datasize;
	BYTE   elementsize;
	int offset = 0;
	int i=0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;
	


	datasize = p->payload.obj_ints.data_size;
	elementsize = p->payload.obj_ints.element_size;
	if(datasize == 0 || elementsize == 0)
		return array;

	reverseBytes((BYTE*)&datasize, 2);
	memcpy(&(array->data[offset]), &datasize, 2);
	offset += 2;
	array->data[offset++] = elementsize;



	for(i=0; i<p->payload.obj_ints.data_size/elementsize; i++)
	{
		switch (p->payload.obj_ints.element_size)
		{
				case 1:
					array->data[offset++] =  p->payload.obj_ints.obj_data[i];
					break;

				case 2:
					memcpy(&d16, &(p->payload.obj_ints.obj_data[i*elementsize]), 2);
					reverseBytes((BYTE*) &d16, 2);
					memcpy(&(array->data[i*elementsize + offset]), &d16, 2);
					break;

				case 4:
					memcpy(&d32, &(p->payload.obj_ints.obj_data[i*elementsize]), 4);
					reverseBytes((BYTE*) &d32, 4);
					memcpy(&(array->data[i*elementsize+ offset]), &d32, 4);
					break;

				case 8:
					memcpy(&d64, &(p->payload.obj_ints.obj_data[i*elementsize]), 8);
					reverseBytes((BYTE*) &d64, 8);
					memcpy(&(array->data[i*elementsize + offset]), &d64, 8);
					break;
		}
	}

	return array;
}
