#ifndef PC_HOST
#include "OTA_Def.h"
#ifdef EMBEDDED
#include "OTA_Message_Embedded.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/watchdog.h"
#include "driverlib/timer.h"


time_t time(time_t* t){
	
	*t = ROM_HibernateRTCGet();
	return ROM_HibernateRTCGet();
	
};

size_t recv(int socketfd, void* data, size_t length, int flags){
	return 0;
};

size_t send(int socket, const void *buffer, size_t length, int flags){
	return 0;
};


#ifdef SELF_MALLOC

static unsigned char pseudo_malloc_buf[MAX_PSEUDO_ALLOC_SIZE];
static unsigned char* pseudo_malloc_ptr = pseudo_malloc_buf;
static unsigned char* max_alloc_boundary = pseudo_malloc_buf + MAX_PSEUDO_ALLOC_SIZE;

void* malloc(size_t size){

	void* ret = 0;
#define MALLOC_MEM_TEST
#ifdef MALLOC_MEM_TEST
	static unsigned char set = 0;

	if(!set){
		memset(pseudo_malloc_buf, 0xAA, sizeof(pseudo_malloc_buf));
		set = 1;
	}
#endif

	while(size % 4){
		size++;
	}
	
	if(((unsigned long)pseudo_malloc_ptr + size) < (unsigned long)max_alloc_boundary ){
		//pseudo calloc :) ?
		memset(pseudo_malloc_ptr, 0, size);
		ret = pseudo_malloc_ptr;
		__nop();
		(unsigned char*)pseudo_malloc_ptr += size;
		
	}
	else{
		__nop();
#ifdef MALLOC_MEM_TEST
		printf("Crashed and burned in malloc...");
		while(1){}
#endif
		ret = 0;
		
	}
	__nop();
	return ret;
}

void free(void* to_free){
	return;
}
#endif
#endif
#endif
