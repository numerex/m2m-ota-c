/*
 * main.c
 *
 *  Created on: Aug 25, 2011
 *      Author: wlai
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <OTA_Def.h>

#define u8   BYTE
#define u32  int
#define DEBUG_PROTOCOL  log

static int goid = 10;
static int seqid = 55;
static int eventcode = 108;



static int sockfd;


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void log(char* message)
{

	printf("\n---------- %s -------------\n", message);

}

void testObjectByte()
{
	log ("OBJECT BYTE");

	log ("Creating a new object");
	OTA_Object* o = new_OTA_Object(_OBJTYPE_BYTE, goid++);

	log ("Print the object");
	o->print(o);


	log("Set value of the object");
	o->payload.obj_byte.set(o, -100);
	o->print(o);

	log("Get Bytes of the object");
	BYTEARRAY* b = o->getBytes(o);
	b->print(b);

	log("Deleting the byte array");
	b->Delete(b);


	log("Get value of the object ");
	BYTE data = o->payload.obj_byte.byteValue(o);
	printf("DATA = %x\n", data);

	log("Delete the object ");
	o->Delete(o);

	log ("END OF OBJECT TYPE");
}


void testObjectInt()
{
	log("Creating new INT Object ");
	OTA_Object* o = new_OTA_Object(_OBJTYPE_INT, goid++);

	o->print(o);
        BYTEARRAY* arr = o->getBytes(o);
        arr->print(arr);


	int d = -129;
	log("Set value of the object");

	o->payload.obj_int.set(o, d);
	o->print(o);

	BYTEARRAY* q = o->getBytes(o);
	q->print(q);
	q->Delete(q);


	log("Setting value, automatically using smallest size int.");
	d = 10;
	o->payload.obj_int.set(o, d);
	o->print(o);

	log("Get bytes of the object");
	BYTEARRAY* p = o->getBytes(o);

	p->print(p);
	p->Delete(p);

	log("Deleting the object");
	o->Delete(o);

	log("End of INT Object");

}


void testObjectFloat(float f, int oid)
{

	log("Creating floating point object");
	OTA_Object* p = new_OTA_Object(_OBJTYPE_FLOAT, oid);

	p->print(p);

	log("Assing float value");
	//float f= 0.123;
	//float f = -35.084789;

	p->payload.obj_float.set(p, f);
	p->print(p);

	log("Get value of the float object");
	float b = p->payload.obj_float.floatValue(p);

	printf(" value = %f\n", b);


	log("get bytes array of the object");
	BYTEARRAY* arr = p->getBytes(p);
	arr->print(arr);

	log("Delete the byte array and object");
	arr->Delete(arr);
	p->Delete(p);


	log("End of FLOAT Object");

}

void testObjectMime()
{

	log("Creating empty MIME object");

	OTA_Object* obj_mime = new_OTA_Object(_OBJTYPE_MIME, goid++);
	obj_mime->print(obj_mime);

	    //BYTE data2[] = {0x01, 0x02, 0x03, 0x04, 0x05};
		BYTE data2[10000];
		int i=0; 
		for(i=0; i<10000; i++)
			data2[i] = i;

	    obj_mime->payload.obj_mime.set(obj_mime, IMAGE, JPG, data2, 10000);
	    obj_mime->print(obj_mime);

	    log("Get Object Byte array");
	    BYTEARRAY* dp = obj_mime->getBytes(obj_mime);
	    dp->print(dp);
	    dp->Delete(dp);


	    log("Get the content type, subcontent type and content");
	    _CONTENT_TYPE c = obj_mime->payload.obj_mime.get_content_type(obj_mime);
	    printf(" content type = %d\n", c);

	    _SUB_CONTENT_TYPE s = obj_mime->payload.obj_mime.get_subcontent_type(obj_mime);
	    printf(" sub content type = %d\n", s);


	    BYTE* content = obj_mime->payload.obj_mime.get_content(obj_mime);
	    int size = obj_mime->payload.obj_mime.size(obj_mime);

	    printf("size = %d\n", size);
	    //int i;
	    for(i=0; i<size; i++)
	    	printf("content[%d] = %x\n", i, content[i]);

	    dp = obj_mime->getBytes(obj_mime);
	    dp->print(dp);

	    dp->Delete(dp);
	    obj_mime->Delete(obj_mime);


}

void testObjectString()
{
	log("Creating Object String");
	OTA_Object* strobj = new_OTA_Object(_OBJTYPE_STRING, goid++);
	      strobj->print(strobj);

	      log("Setting value of the object");
	      char* msg = "HAPPY LABOR'S DAY!";
	      strobj->payload.obj_string.set(strobj, msg);

	      strobj->print(strobj);


	      log("Get string value from the object");
	      char* s = strobj->payload.obj_string.stringValue(strobj);

	      printf(" value = %s\n", s);

	      log("output to bytes\n");
	      BYTEARRAY* dp = strobj->getBytes(strobj);
	      dp->print(dp);

	      dp->Delete(dp);
	      strobj->Delete(strobj);

	      log("End of String Object");
}


void testObjectIntArray()
{

	log("Creating a new UINT ARRAY object");
	OTA_Object* p = new_OTA_Object(_OBJTYPE_ARRAY_UINT, goid++);

	p->print(p);
	BYTEARRAY* d = p->getBytes(p);
	    log("get bytes");
	    d->print(d);


	log("Assing a short int array");
	short b [] = {-3, -4, -5, -6, -7};

	p->payload.obj_ints.set_2(p, &b, 5);
	p->print(p);


    d = p->getBytes(p);
    log("get bytes");
    d->print(d);

    p->Delete(p);

    log("Construct int array object from bytes");
    p = new_OTA_Object_Bytes(d->data);
    p->print(p);


    log("Getting the int array");

    // obj_ints.array() returns long* from malloc.
    long* data = p->payload.obj_ints.array(p);
    int i;
    for(i=0; i<p->payload.obj_ints.array_size(p); i++)
    {
    	printf("data[%d] = %d\n", i, data[i]);

    }
    free(data);



    p->Delete(p);
    d->Delete(d);

	log("End of Int Array Object");


}

void testObjectFloatArray()
{

	log("Create a new Float array");
	OTA_Object* p = new_OTA_Object (_OBJTYPE_ARRAY_FLOAT, goid++);

	p->print(p);

	float data[] = {0.1, 0.2, 0.3, 0.4, 0.66};
	log("Setting data");


	p->payload.obj_floats.set_4(p, &data, 5 );
	p->print(p);


	log("Getting the floats out of object");

	int i;
	// obj_floats.array()  returns  double* from malloc
	double* fa = p->payload.obj_floats.array(p);
	for(i=0;  i<p->payload.obj_floats.array_size(p); i++)
	{

		printf("data[%d] = %f\n", i, fa[i]);

	}

	free (fa);

	BYTEARRAY* ba = p->getBytes(p);
	ba->print(ba);
	ba->Delete(ba);



	log("End of FLOAT ARRAY");





}

void testObjectByteArray()
{

	log("Creating byte array");
	OTA_Object* p = new_OTA_Object(_OBJTYPE_ARRAY_UBYTE, goid++);
	p->print(p);

	BYTE data[] = { 0xFA, 0xFB, 0xFC, 0xFD };

	// This is clone of data.
	p->payload.obj_bytes.set(p, &data, 4);
	p->print(p);


	log("print the byte array");
	BYTEARRAY* ba = p->getBytes(p);
	ba->print(ba);
	p->print(p);

	log("construct object from byte array");

	OTA_Object* p2 = new_OTA_Object_Bytes(ba->data);
	p2->print(p2);
	p2->Delete(p2);


	log("get value out from object");
	log("number of bytes in Object Bytes array");

	int number = p->payload.obj_bytes.array_size(p);
	printf(" array size is %d\n", number);

	log("Retriving object data");

	// need to free BYTE* from array();
	BYTE* d = p->payload.obj_bytes.array(p);
	int i=0;
	for(i=0; i<number; i++)
		printf(" data[%d] = 0x%x \n", i, d[i]);

	free(d);

	log("delete object and byte array");
	p->Delete(p);
	ba->Delete(ba);


	log ("End of Object BYTE ARRAY");




}

void testObjectTimestamp()
{


	log ("Create a Object timestamp, using current time as default");

	OTA_Object* p = new_OTA_Object(_OBJTYPE_TIMESTAMP, goid++);
	p->print(p);


	log("Chaning the value of timestamp");
	ULong l = 1234;
    p->payload.obj_timestamp.set(p, l);
    p->print(p);

    log("Chaning the timestap to current time");
    p->payload.obj_timestamp.set(p, 0);
    p->print(p);


    log("get bytes of the object");
    BYTEARRAY* d = p->getBytes(p);

    d->print(d);
    d->Delete(d);

    log(" get timestamp value from the object");
    time_t time = p->payload.obj_timestamp.timeValue(p);

    printf(" timestamp is %d\n", time);
    p->Delete(p);


	log ("End of Object timestamp");






}



void testMessageMTA()
{

	   log("Create a empty MTA message, no data.");
	    OTA_Message* ota = new_OTA_Message(_MOBILE_TERMINATED_ACK);
	    ota->print(ota);

	    int setid = 10;
	    int eventcode = 15;
	    printf("\n--- setting seq id %d and event code %d\n", setid, eventcode);
	    ota->set_Seq_ID(ota, setid++);
	    ota->set_Event_Code(ota, eventcode++);
	    ota->print(ota);

	    log("get the bytes");
	    BYTEARRAY* b = ota->getBytes(ota);
	    b->print(b);
	    b->Delete(b);

	    log("send the message to socket");

	    	    int socketflag = 0;
	    	    ota->send(ota, sockfd, socketflag);

	     OTA_Message* p3 =  recvFromSocket(sockfd, socketflag);
	     log("Received something from socket");
	     p3->print(p3);
	     p3->Delete(p3);


	    printf("\n -- delete the message \n");
	    ota->Delete(ota);

	    log("End of MTA message");


}

void testMsgMTE()
{

	log("Creating a empty MTE message");

	    OTA_Message*  ote = new_OTA_Message(_MOBILE_TERMINATED_EVENT);
	    ote->set_Seq_ID(ote, seqid++);
	    ote->set_Event_Code(ote, eventcode++);
	    ote->print(ote);

	    BYTE id1 = 97;
	    BYTE id2 = 98;
	    BYTE id3 = 99;
	    printf("\n -- add object ids %d %d %d to the message  \n", id1, id2, id3);
	    ote->add_objid(ote,id1);

	    ote->add_objid(ote,id2);

	    ote->add_objid(ote,id3);


	    ote->print(ote);


	    log("get the object ids");

	    int num = ote->objcnt(ote);
	    printf("obj ids is %d\n", num);

	    OBJ_ID* o = ote->getOids(ote);
	    int i;
	    for(i=0; i<num; i++)
	    {
	    	printf("OID[%d] = %d\n", i, o->id);
	    	o = o->next;

	    }




	    log("get the bytes");
	    BYTEARRAY* ote_b = ote->getBytes(ote);
	    ote_b->print(ote_b);


	    log("Construct message from bytes");
	    OTA_Message* p2 =   createMessageFromBytes(ote_b->data);

	    p2->print(p2);



	    log("send the message to socket");

	    int socketflag = 0;
	    ote->send(ote, sockfd, socketflag);


	    OTA_Message* p3 =  recvFromSocket(sockfd, socketflag);
	    log("Received something from socket");

	    p3->print(p3);



	    log("delete the byte array");
	    ote_b->Delete(ote_b);


	    log("delete the message");
	    ote->Delete(ote);




	    log("End of  MTE message");


}


void testMsgMOE()
{

	log("Creating a MOE message, (Same as MOA)");

	    OTA_Message* moe = new_OTA_Message(_MOBILE_ORIGINATED_EVENT);

	    moe->set_Seq_ID(moe, seqid++);
	    moe->set_Event_Code(moe, eventcode++);
	    moe->print(moe);

	     printf("\n -----------  add objects to the message \n");

	     OTA_Object* obj_byte = new_OTA_Object(_OBJTYPE_BYTE, goid++);
	     obj_byte->payload.obj_byte.set(obj_byte, 100);

	     OTA_Object* obj_int = new_OTA_Object(_OBJTYPE_INT, goid++);
	     obj_int->payload.obj_int.set(obj_int, 88888);

	     OTA_Object* obj_mime = new_OTA_Object(_OBJTYPE_MIME, goid++);
	     BYTE data2[] = {0x01, 0x02, 0x03, 0x04, 0x05};
	   	 obj_mime->payload.obj_mime.set(obj_mime, IMAGE, JPG, data2, 5);


	     moe->add_obj(moe,obj_byte);
	     moe->add_obj(moe,obj_int);
	     moe->add_obj(moe,obj_mime);
	
		
	     OTA_Object* obj_int_empty = new_OTA_Object(_OBJTYPE_UINT, goid++);
	     moe->add_obj(moe,obj_int_empty);

		
             OTA_Object* obj_ints = new_OTA_Object(_OBJTYPE_ARRAY_INT, goid++);
		moe->add_obj(moe, obj_ints); 




	     moe->print(moe);

	     log("get the bytes of message");
	     BYTEARRAY* moe_b = moe->getBytes(moe);
	     moe_b->print(moe_b);

	     log("Get the objs out from the message");

	     int objcnt = moe->objcnt(moe);
	     printf("number of objects is %d\n", objcnt);

	     int i;
	     OTA_Object* obj = moe->getObjs(moe);

	     for( i=0; i<objcnt; i++)
	     {
	    	 obj->print(obj);
	    	 obj = obj->next;
	     }


         log("send the message to socket");
         int socketid=1;
         int socketflag = 0;
         moe->send(moe, sockfd, socketflag);


    BYTE data[] = {
	0xAA, 0x01, 0x00, 0x27, 0x11, 0x00, 0x00, 0x01, 0x35, 
	0xAC, 0x49, 0xB8, 0x98, 0x17, 
	0x00, 0x02, 0x00, 0x00, 
	0x24, 0x01, 0x01, 0x00, 0x01, 0x03, 0x42, 0x36, 0xB7, 
	0xE4, 0x02, 0x03, 0xC2, 0xDE, 0x18, 0x8C, 0x03, 0x0A, 
	0x02, 0x13, 0x2E, 0x04, 0x0A, 0x01, 0x00, 0x05, 0x0A, 
	0x01, 0x00, 0x06, 0x02, 0x00, 0x13, 0x32, 0x30, 0x31, 
	0x32, 0x2F, 0x30, 0x31, 0x2F, 0x32, 0x33, 0x20, 0x32, 
	0x32, 0x3A, 0x31, 0x36, 0x3A, 0x34, 0x37, 0x07, 0x0A, 0x01, 0x0A, 0x08, 0x03, 0x3F, 0x8E, 0x14, 0x7B, 0x09, 0x03, 0x40, 0x03, 0xD7, 0x0A, 0x0A, 0x0A, 0x01, 0x09, 0x0B, 0x0A, 0x01, 0x33, 0x0D, 0x02, 0x00, 0x03, 0x34, 0x31, 0x30, 0x0E, 0x0A, 0x02, 0x01, 0x36, 0x0F, 0x02, 0x00, 0x04, 0x39, 0x37, 0x31, 0x38, 0x11, 0x02, 0x00, 0x05, 0x30, 0x2E, 0x30, 0x2E, 0x31, 0x12, 0x0A, 0x01, 0x00, 0x13, 0x02, 0x00, 0x00, 0x14, 0x0A, 0x01, 0x04, 0x15, 0x0A, 0x01, 0xE9, 0x16, 0x0A, 0x01, 0xFF, 0x17, 0x0A, 0x01, 0x51, 0xE4 }; 


         
         log("Read OTA message from bytes");
         OTA_Message* p2 = createMessageFromBytes (data);
         p2->print(p2);

          BYTEARRAY* ote_b = p2->getBytes(p2);
          ote_b->print(ote_b);

}

void testCRC() {


    Msg_Header * h = new_OTA_MSG_Header(NULL, 0,0,0);
    h->print(h);



  BYTE data[] = {0xdd, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x47, 0xd5, 0xce, 0x20, 0x6d, 0x55};

DEBUG_PROTOCOL("Processing server message");
 
      MESSAGE_TYPE type;
      u8 crc;
      OTA_Message* OTAMessage;
      u8 objectCnt = 0;
      u32 n        = 0;
      u8 objectId  = 0;
      u32 i        = 0;
      u8 rxCrc     = 0;
     

	OTA_Message* p = createMessageFromBytes (data);
        p->print(p);

	//DEBUG_PROTOCOL("Create Message From Bytes is ok"); 
	//printf("Message type is %d\n", p2->type(p2));


      Msg_Header* header = (Msg_Header*) malloc (sizeof(Msg_Header)); 
      new_OTA_MSG_Header(header, 0, 0, 0); 
      //Msg_Header* header = createHeaderFromBytes(NULL, data);
      createHeaderFromBytes(header, data);
      if (header == NULL)
      {
         DEBUG_PROTOCOL("Failed to read header of message from server");
//         return NULL;
      }
      else
      {
	 DEBUG_PROTOCOL("PRINTING HEADER....\n"); 
         type = (MESSAGE_TYPE)header->type;
         if ( ( type != _MOBILE_TERMINATED_EVENT) && (type != _MOBILE_TERMINATED_ACK) &&
              ( type != _MOBILE_ORIGINATED_EVENT) && (type != _MOBILE_ORIGINATED_ACK) )
         {
            DEBUG_PROTOCOL("Not a recognized server message type");
   //         return NULL;
         }
         else
         {
            OTAMessage = new_OTA_Message(type);
            memcpy(&(OTAMessage->header), header, HEADER_SIZE);
            free(header);
           
            switch (OTAMessage->type(OTAMessage))
            {
               case _MOBILE_TERMINATED_EVENT:
                  // TODO:  Not implemented yet. . .
                  //n = recv(socketfd, &objectCnt, 1, flags);
                  //OTAMessage->obj_cnt = 0;
                  //for (i = 0; i < objectCnt; i++)
                  //{
                  //   //n = recv(socketfd, &oid,1,flags);
                  //   OTAMessage->add_objid(OTAMessage, objectId);
                  //}
                  break;
              
               case _MOBILE_TERMINATED_ACK:
                    break;
 
               case _MOBILE_ORIGINATED_EVENT:
               case _MOBILE_ORIGINATED_ACK:  
                  // Should not have to worry about these as we are the mobile.
                  break;
            }
           
	    OTAMessage->print(OTAMessage);
            crc = OTAMessage->updateCRC(OTAMessage);
            rxCrc = data[HEADER_SIZE];
 
           if (crc != rxCrc)
            {
               DEBUG_PROTOCOL("CRC check failed for server message");
           }
		else
			DEBUG_PROTOCOL("CRC IS FINE !!!!"); 

	}

	printf("My system is %d\n", getByteOrder());

	}	
}



void testMailboxCommand () 
{

        log (" create empty mailbox request....");
 	OTA_Message*  m = new_OTA_Message(_MOBILE_ORIGINATED_MAILBOX_REQ);
        m->print(m);
	
	log ("setting delivery protocol for mail...");
        //TCP=1, UDP=2, SMS=3
        m->setMTRoute(m, UDP); 
	m->print(m);

	log ("Print the bytes...");
        BYTEARRAY* q = m->getBytes(m);
        q->print(q);


	log ("Convert byte back to OTA Message .....");
        
	 OTA_Message* p2 = createMessageFromBytes (q->data);
         p2->print(p2);

        q->Delete(q);
}


void testMailboxAck () 
{
       log (" create empty mailbox ack...");
       
       OTA_Message*  m = new_OTA_Message(_MOBILE_ORIGINATED_MAILBOX_ACK);
       m->mb_mail_count = 100;
       
       m->print(m);
       BYTEARRAY* q = m->getBytes(m);
       q->print(q);

       log (" read message from bytes....");
       OTA_Message* p = createMessageFromBytes (q->data);
       p->print(p);
    
       q->Delete(q);
    
     
}






int main(int argc, char *argv[])
{

	//testCRC(); 

    BYTE data[] = {
	0xAA, 0x01, 0x00, 0x27, 0x11, 0x00, 0x00, 0x01, 0x35, 
	0xAC, 0x49, 0xB8, 0x98, 0x17, 0x00, 0x02, 0x00, 0x00, 
	0x24, 0x01, 0x01, 0x00, 0x01, 0x03, 0x42, 0x36, 0xB7, 
	0xE4, 0x02, 0x03, 0xC2, 0xDE, 0x18, 0x8C, 0x03, 0x0A, 
	0x02, 0x13, 0x2E, 0x04, 0x0A, 0x01, 0x00, 0x05, 0x0A, 
	0x01, 0x00, 0x06, 0x02, 0x00, 0x13, 0x32, 0x30, 0x31, 
	0x32, 0x2F, 0x30, 0x31, 0x2F, 0x32, 0x33, 0x20, 0x32, 
	0x32, 0x3A, 0x31, 0x36, 0x3A, 0x34, 0x37, 0x07, 0x0A, 0x01, 0x0A, 0x08, 0x03, 0x3F, 0x8E, 0x14, 0x7B, 0x09, 0x03, 0x40, 0x03, 0xD7, 0x0A, 0x0A, 0x0A, 0x01, 0x09, 0x0B, 0x0A, 0x01, 0x33, 0x0D, 0x02, 0x00, 0x03, 0x34, 0x31, 0x30, 0x0E, 0x0A, 0x02, 0x01, 0x36, 0x0F, 0x02, 0x00, 0x04, 0x39, 0x37, 0x31, 0x38, 0x11, 0x02, 0x00, 0x05, 0x30, 0x2E, 0x30, 0x2E, 0x31, 0x12, 0x0A, 0x01, 0x00, 0x13, 0x02, 0x00, 0x00, 0x14, 0x0A, 0x01, 0x04, 0x15, 0x0A, 0x01, 0xE9, 0x16, 0x0A, 0x01, 0xFF, 0x17, 0x0A, 0x01, 0x51, 0xE4 }; 

	BYTE data2[] = {0x01, 0x02, 0x03, 0x04, 0x05};

        //BYTE ackerror[] = {0xdd, 0x11, 0x00, 0x1d, 0xdc, 0x00, 0x00, 0x01, 0x36, 0xcb, 0x25, 0xf2, 0x4c, 0x77};
        BYTE ackerror[] = {0xdd, 0x11, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x47, 0xd5, 0xce, 0x20, 0x6d, 0x55};

	printf("ulong on machine is %d\n", sizeof(ULong));
	printf("unsigned long long on my machine is %d\n", sizeof(unsigned long long));




/*
	int  portno, n;
	struct sockaddr_in serv_addr;
	 struct hostent *server;

	    char buffer[256];
	    if (argc < 3) {
	       fprintf(stderr,"usage %s hostname port\n", argv[0]);
	       exit(0);
	    }
	    portno = atoi(argv[2]);
	    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	    if (sockfd < 0)
	        error("ERROR opening socket");
	    server = gethostbyname(argv[1]);
	    if (server == NULL) {
	        fprintf(stderr,"ERROR, no such host\n");
	        exit(0);
	    }
	    bzero((char *) &serv_addr, sizeof(serv_addr));
	    serv_addr.sin_family = AF_INET;
	    bcopy((char *)server->h_addr,
	         (char *)&serv_addr.sin_addr.s_addr,
	         server->h_length);
	    serv_addr.sin_port = htons(portno);
	    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	        error("ERROR connecting");
*/

/*

//   testObjectFloat();


 //  testObjectMime();


   testObjectString();


   testObjectIntArray();


   testObjectFloatArray();

   testObjectByteArray();


   testObjectTimestamp();
*/

//   testObjectFloat(-35.084789, 31);
//   testObjectFloat(-80.626045, 32);

//   testMsgMOE();

//   testMessageMTA();

   //testMsgMTE();




//	log("Read OTA message from bytes");
//         OTA_Message* p2 = createMessageFromBytes (&ackerror[0]);
//         p2->print(p2);
	
//	printf("p2->crc is %d\n", p2->updateCRC(p2));

/*
          log("Read OTA message from socket");
          OTA_Message* p3 =   recvFromSocket(sockfd, socketflag);
          if(p3 != NULL)
                  p3->print(p3);

              log("delete the message");
              moe->Delete(moe);
              moe_b->Delete(moe_b);




   testObjectByte();

   testObjectInt();

   testObjectMime();
*/

	 testMailboxCommand();
	testMailboxAck();
	  return 0;


}




