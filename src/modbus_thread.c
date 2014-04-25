#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <modbus-tcp.h>
#include <string.h>

#include "modbus_thread.h"

pthread_mutex_t modbus_lock = PTHREAD_MUTEX_INITIALIZER;

/* Only access with modbus_lock */
struct s_modbus_data modbus_data;

/* Another copy to allow for TCP blocking */
static struct s_modbus_data priv_modbus_data;

#define GET_REGS(first, last) { \
    retval = modbus_read_registers(ctx, first, last - first + 1,	    \
			priv_modbus_data.reg_##first##_##last);  	    \
    if (retval < 0) {							    \
	memset(priv_modbus_data.reg_##first##_##last, MODBUS_ERROR_VAL,	    \
			sizeof(priv_modbus_data.reg_##first##_##last));}}

void *poll_modbus(void *ctx_p) {
    int retval;
    modbus_t *ctx = (modbus_t *) ctx_p;

    /* Bring data into buffer */
    while (1) {
	/* TCP requests - may block */

	GET_REGS(826, 826);
	GET_REGS(352, 353);
	GET_REGS(902, 902);

	if (retval < 0) {
	    printf("modbus: Error on poll\n");
	    /* Attempt reconnect */
	    modbus_close(ctx);
	    if (modbus_connect(ctx) == -1) {
		/* Kill the process */
		exit(1);
	    }

	}

	pthread_mutex_lock(&modbus_lock);
	modbus_data = priv_modbus_data;
	pthread_mutex_unlock(&modbus_lock);

	usleep(MODBUS_POLL_INTERVAL_US);
    }
}
