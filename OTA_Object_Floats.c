/*
 * OTA_Object_Float_Array.c
 *
 *  Created on: Sep 6, 2011
 *      Author: wlai
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <OTA_Def.h>




int get_Float_array_size (OTA_Object* p)
{
	if( p == NULL)
		return 0;

	if(p->payload.obj_floats.element_size == 0)
		return 0;

	return (int) ((p->payload.obj_floats.data_size) / (p->payload.obj_floats.element_size)) ;

}

double* get_Float_array(OTA_Object* p)
{

	int arraysize;
	double* d;
	int floatsize;
	float f;
	double d1;

	int i;	

	if (p == NULL)
		return 0;

	arraysize = p->payload.obj_floats.array_size(p);
	d = malloc (sizeof(double) * arraysize);
	floatsize = p->payload.obj_floats.element_size;
	
	for(i=0; i<arraysize; i++)
	{
	switch (floatsize)
	{
	case 4:
		memcpy(&f, &(p->payload.obj_floats.obj_data[i*floatsize]), floatsize);
		d[i] = f;
		break;

	case 8:
		memcpy(&d1, &(p->payload.obj_floats.obj_data[i*floatsize]), floatsize);
		d[i] = d1;
		break;
	}
	}
	return d;

}




void set_Floats_value(OTA_Object*p, BYTE* data, int size, int elementsize)
{
	if(p == NULL)
			return;

	p->payload.obj_floats.data_size = size * elementsize;
	p->payload.obj_floats.element_size = elementsize;
	p->payload.obj_floats.obj_data = data;

}




void set_Floats_value_4(OTA_Object* p, float* data, int size )
{
	BYTE* temp;

	
	if(data == NULL || size == 0)
		return;

	temp = malloc(size*4);
	memcpy(temp, data, size*4);
	set_Floats_value( p,  temp, size, 4);
}



void set_Floats_value_8(OTA_Object* p, double*  data, int size)
{

	BYTE* temp;
	
	if(data == NULL || size == 0)
		return;

	temp = malloc(size*8);
	memcpy(temp,  data, 8*size);

	set_Floats_value(p, (BYTE*) temp, size, 8);
	return;
}



void Object_Floats_print(OBJ_FLOAT_ARRAY* p)
{

	int i=0;
	
	float f1;
	double f2;

	printf("\t\t<FLOATS SIZE = %d/>\n", p->data_size);
	printf("\t\t<ELEMENT SIZE = %d/>\n", p->element_size);
	

	if(p->data_size == 0 || p->element_size == 0)
		return;



	for(i=0; i< (p->data_size)/(p->element_size); i++)
	{
		switch (p->element_size)
		{
		case 4:
			memcpy(&f1, &(p->obj_data[i*p->element_size]), 4);
			printf("\t\t\t<FLOAT[%d] = %f/>\n", i, f1);
			break;
		case 8:
			memcpy(&f2, &(p->obj_data[i*p->element_size]), 8);
			printf("\t\t\t<FLOAT[%d] = %lf/>\n", i, f2);
			break;
		}
	}

}

void populate_Obj_Floats_socket (OTA_Object* p, int socketfd, int flags)
{


	BYTE elementsize =  0;
	UInt16 datasize = 0;


	UInt32 d32 = 0;
	ULong  d64 = 0;
	BYTE* buff ;
	int i=0;


	// reading byte size
	int n  = recv(socketfd, &datasize, 2, flags);
	reverseBytes((BYTE*)&datasize, 2);
	p->payload.obj_floats.data_size = datasize;

	n = recv(socketfd, &elementsize, 1, flags);
	p->payload.obj_floats.element_size = elementsize;


	if(datasize == 0 || elementsize == 0)
	{
		p->payload.obj_floats.obj_data = NULL;
		return;
	}

	// total size
	buff = malloc(datasize);


	for(i=0; i< datasize/elementsize; i++)
	{
	switch (elementsize)
	    {
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
	p->payload.obj_floats.obj_data = buff;
	return;
}

void populate_Obj_Floats (OTA_Object* p, BYTE* data)
{
	int offset = 0;
	int i;
	UInt32 d32 = 0;
	ULong  d64 = 0;
	UInt16 datasize;
	BYTE   elementsize;

	memcpy(&datasize, data, 2);
	offset += 2;
	reverseBytes((BYTE*)&datasize, 2);

	 p->payload.obj_floats.data_size = datasize;
	 p->payload.obj_floats.element_size = data[offset++];
	elementsize = p->payload.obj_floats.element_size;

	if(datasize == 0 || elementsize == 0)
	{
		p->payload.obj_floats.obj_data = NULL;
		return;
	}
	p->payload.obj_floats.obj_data = malloc (datasize);


	for (i=0;i < p->payload.obj_floats.data_size/p->payload.obj_floats.element_size;i++)
	{
			switch (elementsize)
			{
			case 4:
				memcpy(&d32, &(data[i*elementsize + offset]), 4);
				reverseBytes((BYTE*)&d32, 4);
				memcpy(&(p->payload.obj_floats.obj_data[i*elementsize]), &d32, 4);
				break;
			case 8:
				memcpy(&d64, &(data[i*elementsize + offset]), 8);
				reverseBytes((BYTE*)&d64, 8);
				memcpy(&(p->payload.obj_floats.obj_data[i*elementsize]), &d64, 8);
				break;
			}
		}
    return;

}

BYTEARRAY* Floats_to_BYTES (OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);

	int offset = 0;
	UInt16 d16 = 0;
	UInt32 d32 = 0;
	ULong d64  = 0;
	UInt16 datasize;
	BYTE   elementsize;
	int i=0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;



	datasize = p->payload.obj_floats.data_size;
	elementsize = p->payload.obj_floats.element_size;

	if(datasize ==0 || elementsize == 0)
		return array;


	reverseBytes((BYTE*)&datasize, 2);
	memcpy(&(array->data[offset]), &datasize, 2);
	offset += 2;
	array->data[offset++] = elementsize;


	
	for(i=0; i<p->payload.obj_floats.data_size/elementsize; i++)
	{
		switch (elementsize)
		{
				case 4:
					memcpy(&d32, &(p->payload.obj_floats.obj_data[i*elementsize]), 4);
					reverseBytes((BYTE*) &d32, 4);
					memcpy(&(array->data[i*elementsize + offset]), &d32, 4);
					break;

				case 8:
					memcpy(&d64, &(p->payload.obj_floats.obj_data[i*elementsize]), 8);
					reverseBytes((BYTE*) &d64, 8);
					memcpy(&(array->data[i*elementsize + offset]), &d64, 8);
					break;
		}
	}

	return array;
}
