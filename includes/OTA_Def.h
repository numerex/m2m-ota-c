/*
 * OTA_Def.h
 *
 *  Created on: Aug 25, 2011
 *      Author: wlai
 */


#ifndef _OTA_DEF_H
#define _OTA_DEF_H

// --------------- ABOUT EMBEDDED IMPLEMETATIONS--------------------
// -----------------------------------------------------------------
//We will not have the luxury of <sys/socket.h> in a bear-metal
//embedded platform.  We'll have to manually implement send/recv/etc 
//functions that are compatible with our implementation.  Also, we'll
//have to reimplement malloc free to do pseudo-dynamic memory allocation
//to keep the code compatible.  The "pseudo-dynamic allcation" will
//simply return a pointer to the begining of a static chunk of memory.
//NOTE that in this implementation we are not RTOS/thread safe and DO NOT
//build "collections" of "objects" that will pull from the underlying
//memory buffer
//NOTE2 C compilers typically like to see all local variable definitions
//at the begining of the function to properly setup the stack.  I'll update
//this code to be compliant. 
//------------------------------------------------------------------

//------------------------------------------------------------------
#ifdef EMBEDDED
#include "OTA_Message_Embedded.h"
//#ifdef SELF_MALLOC
//#pragma import(__use_no_heap)
//#endif
#endif
//------------------------------------------------------------------

/* define the message types */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifndef EMBEDDED
#include <sys/socket.h>
#endif

#define  	MOBILE_ORIGINATED_EVENT  			0xAA
#define  	MOBILE_ORIGINATED_ACK  				0xBB
#define  	MOBILE_TERMINATED_EVENT				0xCC
#define  	MOBILE_TERMINATED_ACK 				0xDD
#define 	MOBILE_ORIGINATED_MAILBOX_REQ			0xEE
#define		MOBILE_ORIGINATED_MAILBOX_ACK			0xFF


#ifndef		_MAJOR_VERSION
#define 	_MAJOR_VERSION    		0x01
#endif

#ifndef		_MINOR_VERSION
#define 	_MINOR_VERSION    		0x02    // Adding mailbox request
#endif

#define 	__LITTLE_ENDIAN___   	1

#define 	OBJ_BYTE_SIZE			3
#define     HEADER_SIZE             13


typedef unsigned char 				BYTE;
typedef unsigned short				UInt16;
typedef unsigned int				UInt32;
typedef unsigned long long			ULong;


struct OBJ_DATA
{
		int size;
		BYTE* data;
		void (*print) (struct OBJ_DATA*);
		void (*Delete) (struct OBJ_DATA*);
		void (*append) (struct OBJ_DATA*, struct OBJ_DATA*);
		void (*put) (struct OBJ_DATA*, struct OBJ_DATA*, int offset);
};

typedef struct OBJ_DATA	 BYTEARRAY;

BYTEARRAY* new_BYTEARRAY( int size);

enum _MESSAGE_TYPE {
		_MOBILE_ORIGINATED_EVENT = 0xAA,
		_MOBILE_ORIGINATED_ACK = 0xBB,
		_MOBILE_TERMINATED_EVENT = 0xCC,
		_MOBILE_TERMINATED_ACK = 0xDD,
		_MOBILE_ORIGINATED_MAILBOX_REQ = 0xEE,
		_MOBILE_ORIGINATED_MAILBOX_ACK = 0xFF
	};

typedef enum _MESSAGE_TYPE MESSAGE_TYPE;

struct _OTA_Msg_Header {
    BYTE type;
    BYTE version;
    BYTE event_code;
    UInt16 seq_id;
    ULong timestamp;

    void (*print) (struct _OTA_Msg_Header *);
    BYTEARRAY* (*getBytes)      (struct _OTA_Msg_Header* );
	void (*set_Seq_ID) 			(struct _OTA_Msg_Header*, UInt16);
	UInt16 (*get_Seq_ID) 		(struct _OTA_Msg_Header* );
	void (*set_Event_Code) 		(struct _OTA_Msg_Header* , BYTE);
	BYTE (*get_Event_Code) 		(struct _OTA_Msg_Header* );
	int  (*send)				(struct _OTA_Msg_Header*, int, int);
	ULong (*getTime)			(struct _OTA_Msg_Header*);
	void (*setTime)				(struct _OTA_Msg_Header*, ULong);
    int (*size)					();
    void (*set_type)			(struct _OTA_Msg_Header*, MESSAGE_TYPE);
    MESSAGE_TYPE (*get_type)   (struct _OTA_Msg_Header *);
};

typedef  struct _OTA_Msg_Header    Msg_Header;


 enum  _OBJTYPE_DEF
	{
		_OBJTYPE_BYTE = 		0,
		_OBJTYPE_INT = 			1,
		_OBJTYPE_STRING = 		2,
		_OBJTYPE_FLOAT = 		3,
		_OBJTYPE_TIMESTAMP = 	4,
		_OBJTYPE_ARRAY_BYTE = 	5,
		_OBJTYPE_ARRAY_INT = 	6,
		_OBJTYPE_ARRAY_FLOAT = 	7,
		_OBJTYPE_MIME = 		8,
		_OBJTYPE_UBYTE =        9,
		_OBJTYPE_UINT =         10,
		_OBJTYPE_ARRAY_UBYTE =  11,
		_OBJTYPE_ARRAY_UINT  = 12
	};

 static char* OBJTYPE_STR[]= {
		 "BYTE",
		 "INT",
		 "STRING",
		 "FLOAT",
		 "TIMESTAMP",
		 "ARRAY OF BYTE",
		 "ARRAY OF INT",
		 "ARRAY OF FLOAT",
		 "MIME",
		 "UNSIGNED BYTE",
		 "UNSIGNED INT",
		 "ARRAY OF UNSIGNED BYTE",
		 "ARRAY OF UNSIGNED INT"
 };




typedef enum _OBJTYPE_DEF OBJ_TYPE;

	enum _CONTENT_TYPE
	{
		APPLICATION = 0xA0,
		AUDIO	 = 0xB0,
		VIDEO	 = 0xC0,
		IMAGE	= 0xD0,
		MESSAGE = 0xE0,
		TEXT	= 0xF0
	};

	typedef enum _CONTENT_TYPE _CONTENT_TYPE;


	enum SUB_CONTENT_TYPE
	{
		JPG = 0x01,
		GIF = 0x02,
		PNG = 0x03
	};
	typedef  enum SUB_CONTENT_TYPE _SUB_CONTENT_TYPE;


        enum  MB_COMMAND 
	{
		SET_MT_ROUTE = 1
	};
	typedef enum MB_COMMAND MB_COMMAND; 

        enum  MT_ROUTE_TYPE 
	{
		TCP=1,
		UDP=2,
		SMS=3
	};
	typedef enum MT_ROUTE_TYPE MT_ROUTE_TYPE;

    struct Object;


	struct OBJ_BYTE  {
		BYTE obj_data;
		void (*set) (struct Object*, BYTE);
		BYTEARRAY* (*toBytes) (struct Object*);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_BYTE*);
		BYTE (*byteValue) (struct Object*);
	};

	typedef struct OBJ_BYTE OBJ_BYTE;


	/* one byte of object size */
	struct OBJ_INT {
		BYTE data_size;
		ULong obj_data;
		void (*set) (struct Object*,  long);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_INT*);
		BYTEARRAY* (*toBytes) (struct Object*);
		long (*intValue) (struct Object*);
	};

	typedef struct OBJ_INT OBJ_INT;

	struct OBJ_FLOAT {
		float obj_data;
		void (*set) (struct Object*, float);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_FLOAT*);
		BYTEARRAY* (*toBytes) (struct Object*);
		float (*floatValue) (struct Object*);
	};

	typedef struct OBJ_FLOAT OBJ_FLOAT;

	/* two bytes of size plus data */
	struct OBJ_STRING {
		UInt16 data_size;
		BYTE* obj_data;
		void (*set) (struct Object*, char*);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_STRING*);
		BYTEARRAY* (*toBytes) (struct Object*);
		char* (*stringValue) ( struct Object*);
	};

	typedef struct OBJ_STRING OBJ_STRING;


	struct OBJ_TIMESTAMP {
		ULong time;
		 void (*set) (struct Object*, ULong);
		 void (*read) (struct Object*, int, int);
		 void (*fromBytes) (struct Object*, BYTE*);
		 void (*print) (struct OBJ_TIMESTAMP*);
		 BYTEARRAY* (*toBytes) (struct Object*);
		 time_t (*timeValue) (struct Object*);
	};

	typedef struct OBJ_TIMESTAMP OBJ_TIMESTAMP;


	struct OBJ_BYTE_ARRAY {
		UInt16 		data_size;
		BYTE*     	obj_data;
		void (*set) (struct Object*, BYTE*, int);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_BYTE_ARRAY*);
		BYTEARRAY* (*toBytes) (struct Object*);
		int (*array_size) (struct Object*);
		BYTE* (*array) (struct Object*);

	};
	typedef struct OBJ_BYTE_ARRAY OBJ_BYTE_ARRAY;

	struct  OBJ_INT_ARRAY{
		UInt16 	data_size;
		BYTE  	element_size;
		BYTE*  	obj_data;
		void (*set_1) (struct Object*, BYTE*, int);
		void (*set_2) (struct Object*, UInt16*, int);
		void (*set_4) (struct Object*, UInt32*, int);
		void (*set_8) (struct Object*, ULong*, int);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_INT_ARRAY*);
		BYTEARRAY* (*toBytes) (struct Object*);
		int (*array_size) (struct Object*);
		long* (*array) (struct Object*);


	};
	typedef struct  OBJ_INT_ARRAY OBJ_INT_ARRAY;




	struct OBJ_FLOAT_ARRAY {
		UInt16 	data_size;
		BYTE  	element_size;
		BYTE*  	obj_data;
		void (*set_4) (struct Object*, float*, int);
		void (*set_8) (struct Object*, double*, int);
		void (*read) (struct Object*, int, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_FLOAT_ARRAY*);
		BYTEARRAY* (*toBytes) (struct Object*);
		int (*array_size) (struct Object*);
		double* (*array) (struct Object*);

	};
	typedef struct OBJ_FLOAT_ARRAY OBJ_FLOAT_ARRAY;

	struct OBJ_MIME {
		UInt32 	data_size;
		BYTE 	mime_type;
		BYTE*  obj_data;
		void (*read) (struct Object*, int, int);
		void (*set) (struct Object*, _CONTENT_TYPE, _SUB_CONTENT_TYPE, BYTE*, int);
		void (*fromBytes) (struct Object*, BYTE*);
		void (*print) (struct OBJ_MIME*);
		BYTEARRAY* (*toBytes) (struct Object*);
		_CONTENT_TYPE (*get_content_type) (struct Object*);
		_SUB_CONTENT_TYPE (*get_subcontent_type) (struct Object*);
		BYTE* (*get_content) (struct Object*);
		int (*size) (struct Object*);


	};
	typedef struct OBJ_MIME OBJ_MIME;




	struct Object  {

		BYTE 	objectid;
		BYTE 	objtype;

		union {
			OBJ_BYTE  		obj_byte;
			OBJ_INT 		obj_int;
			OBJ_FLOAT		obj_float;
			OBJ_STRING      obj_string;
			OBJ_TIMESTAMP   obj_timestamp;
			OBJ_BYTE_ARRAY  obj_bytes;
			OBJ_INT_ARRAY   obj_ints;
			OBJ_FLOAT_ARRAY obj_floats;
			OBJ_MIME        obj_mime;

			} payload;

		struct Object* next;

        void (*print) (struct Object*);
        BYTEARRAY* (*getBytes) (struct Object *);
        void (*Delete) (struct Object *);
        int (*size) (struct Object *);
        int (*send) (struct Object*,  int, int);
		int (*isEmpty) (struct Object*); 

	};
	typedef struct Object OTA_Object;



	OTA_Object* new_OTA_Object(OBJ_TYPE type,  BYTE objid);
	OTA_Object* new_OTA_Object_Bytes(BYTE* bytes);
	OTA_Object* new_OTA_Object_Socket(int socketfd, int flags);




	struct OBJ_ID {
		BYTE id;
		struct OBJ_ID* next;
	};
	typedef struct OBJ_ID OBJ_ID;




	struct OTA_MSG {
		Msg_Header header;

		BYTE obj_cnt;
		union {
			OBJ_ID*		 oid;
			OTA_Object*	 obj;
		} objlist;

		BYTE mb_command_count; 
		BYTE mb_command;
		BYTE mb_route_type;
		BYTE mb_mail_count; 
		BYTE crc;

		void (*print) (struct OTA_MSG *);
		void (*set_Seq_ID) 			(struct OTA_MSG*, UInt16);
		UInt16 (*get_Seq_ID) 		(struct OTA_MSG* );
		void (*set_Event_Code) 		(struct OTA_MSG* , BYTE);
		BYTE (*get_Event_Code) 		(struct OTA_MSG* );
		void (*Delete) 				(struct OTA_MSG* );
		int  (*send)				(struct OTA_MSG* , int, int);
		BYTEARRAY* (*getBytes)		(struct OTA_MSG* );
		BYTE (*updateCRC)			(struct OTA_MSG* );
		void (*add_obj) 			(struct OTA_MSG*, OTA_Object*);
		void (*add_objid)           (struct OTA_MSG*, BYTE id);
		MESSAGE_TYPE (*type)        (struct OTA_MSG*);
		ULong (*getTime)            (struct OTA_MSG*);
		BYTE (*objcnt)				(struct OTA_MSG*);
		int (*size)					(struct OTA_MSG*);
		OBJ_ID* (*getOids)			(struct OTA_MSG*);
		OTA_Object* (*getObjs)		(struct OTA_MSG*);
		
		BYTE (*getMailCount) 		(struct OTA_MSG*);
		void (*setMTRoute) 		(struct OTA_MSG*, MT_ROUTE_TYPE);
	};

	typedef struct OTA_MSG  OTA_Message;


	OTA_Message*  new_OTA_Message(MESSAGE_TYPE type);
	OTA_Message*  createMessageFromBytes(BYTE* bytes);
	OTA_Message*  recvFromSocket(int socketfd, int flags);
    BYTE getCRC8(BYTE* p, int size);
    void reverseBytes(BYTE* p, int size);
    void DieWithError(char *errorMessage);



#endif
