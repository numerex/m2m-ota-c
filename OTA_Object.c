/*
 * OTA_Object.c
 *
 *  Created on: Aug 28, 2011
 *      Author: wlai
 */
#include <stdio.h>
#include <string.h>
#include <OTA_Def.h>



void delete_object(OTA_Object* p);
void print_object(OTA_Object* p);
BYTEARRAY* Object_getBytes(OTA_Object* p);
int get_object_size(OTA_Object* p);

/* Byte */
extern	void set_Byte_value (OTA_Object* p, BYTE data);
extern BYTEARRAY* Byte_to_BYTES(OTA_Object* p);
extern void Object_Byte_print(OBJ_BYTE* p);
extern void populate_Obj_Byte_socket (OTA_Object* p, int socketfd, int flags);
extern void populate_Obj_Byte_bytes (OTA_Object* p, BYTE* data);
extern BYTE get_byte_value (OTA_Object* p);
extern void Object_Byte_print_unsigned(OBJ_BYTE* p);



/* Integer */
extern void populate_Obj_Int_socket (OTA_Object*p, int socketfd, int flags);
extern  void set_Int_value(OTA_Object* p, long data);
extern BYTEARRAY* Int_to_BYTES(OTA_Object* p);
extern void populate_Obj_Int(OTA_Object* p, BYTE* data);
extern void Object_Int_print(OBJ_INT* p);
extern long get_Int_value (OTA_Object* p);
extern void Object_Int_print_unsigned(OBJ_INT* p);



/* Float */
extern void Object_Float_print(OBJ_FLOAT* p);
extern BYTEARRAY* Float_to_BYTES(OTA_Object* p);
extern void set_Float_value (OTA_Object*p, float data);
extern void populate_Obj_Float_socket (OTA_Object* p, int socketfd, int flags);
extern void populate_Obj_Float_bytes (OTA_Object* p, BYTE* data);
extern float get_float_value(OTA_Object* p);


/* String */
extern void set_String_value(OTA_Object* p, char* string);
extern void Object_String_print(OBJ_STRING* p);
extern void populate_Obj_String_socket (OTA_Object*p, int socketfd, int flags);
extern void populate_Obj_String(OTA_Object* p, BYTE* data);
extern BYTEARRAY* String_to_BYTES(OTA_Object* p);
extern char* get_String_value (OTA_Object* p);


/* mime */
extern void Object_Mime_print(OBJ_MIME* p) ;
extern BYTEARRAY* Mime_to_BYTES(OTA_Object* p);
extern	void set_Mime_value(OTA_Object* p, _CONTENT_TYPE content_type,  _SUB_CONTENT_TYPE subcontent_type, BYTE* location, int size);
extern void populate_Obj_Mime_socket (OTA_Object*p, int socketfd, int flags);
extern void populate_Obj_Mime(OTA_Object* p, BYTE* data);
extern _CONTENT_TYPE getMimeContentType (OTA_Object* p );
extern _SUB_CONTENT_TYPE getMimeSubContentType(OTA_Object* p );
extern BYTE* get_mime_content (struct Object* p);
extern int mime_content_size (OTA_Object* p);



/* timestamp */
extern void populate_Obj_Timestamp_socket (OTA_Object*p, int socketfd, int flags);
extern void set_Time_value(OTA_Object* p, ULong value);
extern void populate_Obj_Time(OTA_Object* p, BYTE* data);
extern BYTEARRAY* Timestamp_to_BYTES(OTA_Object* p);
extern void Object_Timestamp_print(OBJ_TIMESTAMP* p);
extern time_t get_Time_value (OTA_Object* p);


/* Integer array */
extern void set_Ints_value_1(OTA_Object* p, BYTE* data, int size);
extern void set_Ints_value_2(OTA_Object* p, UInt16* data, int size);
extern void set_Ints_value_4(OTA_Object* p, UInt32* data, int size );
extern void set_Ints_value_8(OTA_Object* p, ULong*  data, int size);
extern void populate_Obj_Ints_socket (OTA_Object* p, int socketfd, int flags);
extern void populate_Obj_Ints (OTA_Object* p, BYTE* data);
extern BYTEARRAY* Ints_to_BYTES (OTA_Object* p);
extern void Object_Ints_print(OBJ_INT_ARRAY* p);
extern int get_Int_array_size (OTA_Object* p);
extern void Object_Ints_print_unsigned(OBJ_INT_ARRAY* p);

// need to free
extern long* get_Int_Array (OTA_Object* p);



/* Byte array */
extern void set_Bytes_value(OTA_Object*p, BYTE* data, int size);
extern void Object_Bytes_print(OBJ_BYTE_ARRAY* p);
extern void populate_Obj_Bytes_socket (OTA_Object* p, int socketfd, int flags);
extern void populate_Obj_Bytes (OTA_Object* p, BYTE* data);
extern BYTEARRAY* Bytes_to_BYTES (OTA_Object* p);
extern int get_bytes_array_size (OTA_Object* p);
extern BYTE* get_bytes_array (OTA_Object* p );

/* Float array */
extern void set_Floats_value_4(OTA_Object* p, float* data, int size );
extern void set_Floats_value_8(OTA_Object* p, double*  data, int size);
extern void Object_Floats_print(OBJ_FLOAT_ARRAY* p);
extern void populate_Obj_Floats_socket (OTA_Object* p, int socketfd, int flags);
extern void populate_Obj_Floats (OTA_Object* p, BYTE* data);
extern BYTEARRAY* Floats_to_BYTES (OTA_Object* p);
extern int get_Float_array_size (OTA_Object* p);


// need to free
extern double* get_Float_array(OTA_Object* p);

int checkForEmptyObject(OTA_Object* p); 


OTA_Object* new_OTA_Object( OBJ_TYPE type,  BYTE objid)
{

	OTA_Object* p = malloc (sizeof(OTA_Object));

	p->objtype = (BYTE) type;
	p->objectid = objid;

	memset(&(p->payload), 0, sizeof(p->payload));

	p->print = print_object;
	p->Delete = delete_object;
	p->getBytes = Object_getBytes;
	p->size = get_object_size;


	switch (p->objtype)
	{

	case _OBJTYPE_BYTE:
	case _OBJTYPE_UBYTE:
		p->payload.obj_byte.set = set_Byte_value;
		p->payload.obj_byte.toBytes = Byte_to_BYTES;
		p->payload.obj_byte.print = Object_Byte_print;
		p->payload.obj_byte.fromBytes = populate_Obj_Byte_bytes;
		p->payload.obj_byte.read = populate_Obj_Byte_socket;
		p->payload.obj_byte.byteValue = get_byte_value;

		if(p->objtype == _OBJTYPE_UBYTE)
			p->payload.obj_byte.print = Object_Byte_print_unsigned;

		break;

	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
		p->payload.obj_int.set = set_Int_value;
		p->payload.obj_int.read = populate_Obj_Int_socket;
		p->payload.obj_int.fromBytes = populate_Obj_Int;
		p->payload.obj_int.toBytes = Int_to_BYTES;
		p->payload.obj_int.print = Object_Int_print;
		p->payload.obj_int.intValue = get_Int_value;

		if(p->objtype == _OBJTYPE_UINT)
			p->payload.obj_int.print = Object_Int_print_unsigned;



		p->payload.obj_int.data_size = 0;
		break;

	case _OBJTYPE_FLOAT:
		p->payload.obj_float.set = set_Float_value;
		p->payload.obj_float.read = populate_Obj_Float_socket;
		p->payload.obj_float.fromBytes = populate_Obj_Float_bytes;
		p->payload.obj_float.toBytes = Float_to_BYTES;
		p->payload.obj_float.print = Object_Float_print;
		p->payload.obj_float.floatValue = get_float_value;
		break;

	case _OBJTYPE_STRING:
		p->payload.obj_string.set = set_String_value;;
		p->payload.obj_string.read = populate_Obj_String_socket;
		p->payload.obj_string.fromBytes = populate_Obj_String;
		p->payload.obj_string.toBytes = String_to_BYTES;
		p->payload.obj_string.print = Object_String_print;
		p->payload.obj_string.stringValue = get_String_value;

		p->payload.obj_string.data_size = 0;
		p->payload.obj_string.obj_data = NULL;

		break;

	case _OBJTYPE_TIMESTAMP:
		p->payload.obj_timestamp.set = set_Time_value;
		p->payload.obj_timestamp.read = populate_Obj_Timestamp_socket;
		p->payload.obj_timestamp.fromBytes = populate_Obj_Time;
		p->payload.obj_timestamp.toBytes = Timestamp_to_BYTES;
		p->payload.obj_timestamp.print = Object_Timestamp_print;
		p->payload.obj_timestamp.timeValue = get_Time_value;
		p->payload.obj_timestamp.set(p, 0);

		break;

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		p->payload.obj_bytes.set = set_Bytes_value;;
		p->payload.obj_bytes.read = populate_Obj_Bytes_socket;
		p->payload.obj_bytes.fromBytes = populate_Obj_Bytes;
		p->payload.obj_bytes.toBytes = Bytes_to_BYTES;
		p->payload.obj_bytes.print = Object_Bytes_print;
		p->payload.obj_bytes.array_size = get_bytes_array_size;
		p->payload.obj_bytes.array = get_bytes_array;
		break;

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
		p->payload.obj_ints.set_1 = set_Ints_value_1;
		p->payload.obj_ints.set_2 = set_Ints_value_2;
		p->payload.obj_ints.set_4 = set_Ints_value_4;
		p->payload.obj_ints.set_8 = set_Ints_value_8;
		p->payload.obj_ints.read = populate_Obj_Ints_socket;
		p->payload.obj_ints.fromBytes = populate_Obj_Ints;
		p->payload.obj_ints.toBytes = Ints_to_BYTES;
		p->payload.obj_ints.print = Object_Ints_print;
		p->payload.obj_ints.array_size = get_Int_array_size;
		// needs to free the array.
		p->payload.obj_ints.array = get_Int_Array;

		if(p->objtype == _OBJTYPE_ARRAY_UINT)
			p->payload.obj_ints.print = Object_Ints_print_unsigned;


		p->payload.obj_ints.data_size = 0;
                p->payload.obj_ints.element_size = 0;
 		p->payload.obj_ints.obj_data = NULL;

		break;

	case _OBJTYPE_ARRAY_FLOAT:
		p->payload.obj_floats.set_4 = set_Floats_value_4;
		p->payload.obj_floats.set_8 = set_Floats_value_8;
		p->payload.obj_floats.read = populate_Obj_Floats_socket;
		p->payload.obj_floats.fromBytes = populate_Obj_Floats;
		p->payload.obj_floats.toBytes = Floats_to_BYTES;
		p->payload.obj_floats.print = Object_Floats_print;
		p->payload.obj_floats.array_size = get_Float_array_size;
		// needs to free
		p->payload.obj_floats.array = get_Float_array;

		break;

	case _OBJTYPE_MIME:
		p->payload.obj_mime.set = set_Mime_value;
		p->payload.obj_mime.read = populate_Obj_Mime_socket;
		p->payload.obj_mime.fromBytes = populate_Obj_Mime;
		p->payload.obj_mime.toBytes = Mime_to_BYTES;
		p->payload.obj_mime.print = Object_Mime_print;
		p->payload.obj_mime.get_content_type = getMimeContentType;
		p->payload.obj_mime.get_subcontent_type = getMimeSubContentType;
		p->payload.obj_mime.get_content = get_mime_content;
		p->payload.obj_mime.size = mime_content_size;
		p->payload.obj_mime.data_size = 0;
		p->payload.obj_mime.mime_type = 0;
		p->payload.obj_mime.obj_data = NULL;
		
		break;

	}
	p->isEmpty = checkForEmptyObject; 
	p->next = NULL;

	return p;
}


void delete_object(OTA_Object* p)
{
	switch(p->objtype)
	{

	case _OBJTYPE_STRING:
		if(p->payload.obj_string.obj_data != NULL)
			free(p->payload.obj_string.obj_data);
		break;


	case _OBJTYPE_MIME:
		if(p->payload.obj_mime.obj_data != NULL)
			free(p->payload.obj_mime.obj_data);
		break;


	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		if(p->payload.obj_bytes.obj_data != NULL)
			free(p->payload.obj_bytes.obj_data);
		break;

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
		if(p->payload.obj_ints.obj_data != NULL)
			free(p->payload.obj_ints.obj_data);
		break;

	case _OBJTYPE_ARRAY_FLOAT:
		if(p->payload.obj_floats.obj_data != NULL)
			free(p->payload.obj_floats.obj_data);
		break;


	default:
		break;
	}

	free(p);
	p = NULL;

}

void print_object(OTA_Object* p)
{

	printf("\t<OBJECT>\n");
	printf("\t\t<ID = %d/>\n", p->objectid);
	printf("\t\t<TYPE = %d (%s)/>\n", p->objtype, OBJTYPE_STR[p->objtype]);

	switch( p->objtype )
	{
	case _OBJTYPE_BYTE:
	case _OBJTYPE_UBYTE:
		p->payload.obj_byte.print((OBJ_BYTE*) &(p->payload.obj_byte));
		break;

	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
		p->payload.obj_int.print((OBJ_INT*) &(p->payload.obj_int));
		break;

	case _OBJTYPE_MIME:
		p->payload.obj_mime.print((OBJ_MIME*) &(p->payload.obj_mime));
		break;

	case _OBJTYPE_FLOAT:
		p->payload.obj_float.print((OBJ_FLOAT*) &(p->payload.obj_float));
		break;

	case _OBJTYPE_STRING:
		p->payload.obj_string.print((OBJ_STRING*) &(p->payload.obj_string));
		break;

	case _OBJTYPE_TIMESTAMP:
			p->payload.obj_timestamp.print((OBJ_TIMESTAMP*) &(p->payload.obj_timestamp));
			break;

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
			p->payload.obj_ints.print((OBJ_INT_ARRAY*) &(p->payload.obj_ints));
			break;

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
			p->payload.obj_bytes.print((OBJ_BYTE_ARRAY*) &(p->payload.obj_bytes));
			break;

	case _OBJTYPE_ARRAY_FLOAT:
			p->payload.obj_floats.print((OBJ_FLOAT_ARRAY*) &(p->payload.obj_floats));
			break;

	}

	printf("\t</OBJECT>\n");

}


/* obj size include objid, objtype, and data */
int get_object_size(OTA_Object* p)
{

	int size = 0;
	
	if(p==NULL)
		return 0;

	
	switch(p->objtype)
	{
	case _OBJTYPE_BYTE:
	case _OBJTYPE_UBYTE:
		size = 3;
		break;

	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
		size = 3 + p->payload.obj_int.data_size;
		break;

	case _OBJTYPE_MIME:
		size = 7 + p->payload.obj_mime.data_size;
		break;

	case _OBJTYPE_FLOAT:
		size = 6;
		break;

	case _OBJTYPE_STRING:
		size = p->payload.obj_string.data_size + 4; // 2 bytes size;
		break;

	case _OBJTYPE_TIMESTAMP:
		size = 10;
		break;

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		size = p->payload.obj_bytes.data_size + 4;
		break;

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
		size = p->payload.obj_ints.data_size + 5;
		break;

	case _OBJTYPE_ARRAY_FLOAT:
		size = p->payload.obj_floats.data_size + 5;
		break;

	}

	return size;

}



int getByteOrder()
{


		union {
                long l;
                char c[4];
        } test;
        int ret = 0;

		test.l = 1;
        
        if( !test.c[3] && !test.c[2] && !test.c[1] && test.c[0] )
        {
                ret =   __LITTLE_ENDIAN___;
        }
       return ret;
}


void reverseBytes(BYTE* p, int size)
{

	static int byteorder = -1;

	if(byteorder == -1)
		byteorder = getByteOrder();

    if(byteorder==  __LITTLE_ENDIAN___)
    {
	    int i=0;
	    BYTE temp = 0;
		for(i=0; i<size/2; i++)
		{
			temp = p[i];
			p[i] = p[size-1-i];
			p[size-1-i] = temp;
		}
    }
	return;
}



BYTEARRAY* Object_getBytes(OTA_Object* p)
{

	switch(p->objtype)
	{
	case _OBJTYPE_BYTE:
	case _OBJTYPE_UBYTE:
		    return p->payload.obj_byte.toBytes(p);

	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
			return p->payload.obj_int.toBytes(p);

	case _OBJTYPE_FLOAT:
		return p->payload.obj_float.toBytes(p);

	case _OBJTYPE_STRING:
			return p->payload.obj_string.toBytes(p);

	case _OBJTYPE_TIMESTAMP:
			return p->payload.obj_timestamp.toBytes(p);

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
			return p->payload.obj_ints.toBytes(p);

	case _OBJTYPE_MIME:
		return p->payload.obj_mime.toBytes(p);

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		return p->payload.obj_bytes.toBytes(p);

	case _OBJTYPE_ARRAY_FLOAT:
		return p->payload.obj_floats.toBytes(p);

	default:
		break;
	}


	return NULL;
}
void print_array(BYTEARRAY* p)
{
	
	int i=0;
	
	if(p == NULL)
		return;
	printf("<HEX> ");
	
	for(i=0; i<p->size; i++)
	{
		printf("0x%02x", p->data[i]);
		if(i != (p->size -1))
			printf(", ");
	}
	printf("</HEX>\n");

}
void delete_array(BYTEARRAY* p)
{
	if(p == NULL)
		return;
	free(p);
	p = NULL;

}



OTA_Object* new_OTA_Object_Socket(int socketfd, int flags)
{

	BYTE objectid = 0;
	BYTE objecttype = 0;
	OTA_Object * obj ;
	BYTE data;

	int n = recv(socketfd, &objectid, 1, flags);
	n = recv(socketfd, &objecttype, 1, flags);
	obj = new_OTA_Object( objecttype, objectid);

	data = 0;
	switch (objecttype)
	{
	case _OBJTYPE_BYTE:
	case _OBJTYPE_UBYTE:
		obj->payload.obj_byte.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
		obj->payload.obj_int.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_MIME:
		obj->payload.obj_mime.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_FLOAT:
		obj->payload.obj_float.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_STRING:
		obj->payload.obj_string.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_TIMESTAMP:
		obj->payload.obj_timestamp.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
		obj->payload.obj_ints.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		obj->payload.obj_bytes.read(obj, socketfd, flags);
		break;

	case _OBJTYPE_ARRAY_FLOAT:
		obj->payload.obj_floats.read(obj, socketfd, flags);
		break;
	}

	return obj;


}


OTA_Object* new_OTA_Object_Bytes(BYTE* bytes)
{
    
	int offset = 0;
	BYTE objectid;
	BYTE objecttype;
	OTA_Object * obj;

	if (bytes == NULL)
    	return NULL;

	
	objectid = bytes[offset++];
	objecttype = bytes[offset++];

	obj = new_OTA_Object(objecttype, objectid);

	switch (objecttype)
	{
	case _OBJTYPE_BYTE:
	case _OBJTYPE_UBYTE:
		obj->payload.obj_byte.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
		obj->payload.obj_int.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_MIME:
		obj->payload.obj_mime.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_FLOAT:
		obj->payload.obj_float.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_STRING:
		obj->payload.obj_string.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_ARRAY_INT:
	case _OBJTYPE_ARRAY_UINT:
		obj->payload.obj_ints.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		obj->payload.obj_bytes.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_ARRAY_FLOAT:
		obj->payload.obj_floats.fromBytes(obj, &(bytes[offset]));
		break;

	case _OBJTYPE_TIMESTAMP:
		obj->payload.obj_timestamp.fromBytes(obj, &(bytes[offset]));
		break;

	}
		return obj;
}


int checkForEmptyObject(OTA_Object* p)
{
   if(p == 0)
	return 1;

   switch(p->objtype)
   {
	case _OBJTYPE_BYTE:
        case _OBJTYPE_UBYTE:
	case _OBJTYPE_FLOAT:
	case _OBJTYPE_TIMESTAMP:
		return 0;
	
	case _OBJTYPE_INT:
	case _OBJTYPE_UINT:
		if(p->payload.obj_int.data_size == 0)
			return 1;
		else
			break;
	case _OBJTYPE_STRING:
		if(p->payload.obj_string.data_size == 0 
			|| p->payload.obj_string.obj_data == NULL)
			return 1;
		else
			break;

	case _OBJTYPE_ARRAY_BYTE:
	case _OBJTYPE_ARRAY_UBYTE:
		if(p->payload.obj_bytes.data_size == 0 ||
			p->payload.obj_bytes.obj_data == NULL)
		return 1;
		else
			break;

	case _OBJTYPE_ARRAY_INT: 
	case _OBJTYPE_ARRAY_UINT:
		
		if(p->payload.obj_ints.data_size == 0 ||
			p->payload.obj_ints.element_size == 0 ||
			p->payload.obj_ints.obj_data == NULL)
		return 1;
		else
			break;
		
	case _OBJTYPE_ARRAY_FLOAT:
		if(p->payload.obj_floats.data_size == 0 ||
			p->payload.obj_floats.element_size == 0 ||
			p->payload.obj_floats.obj_data == NULL)
		return 1;
		else
			break;

        case _OBJTYPE_MIME:
		if(p->payload.obj_mime.data_size == 0 || 
			p->payload.obj_mime.obj_data == NULL)
		return 1;
		else
			break;

	default:
		return 1;

   }

	return 0; 
}


