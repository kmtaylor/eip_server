#include <stdio.h>
#include <modbus-tcp.h>

#include "opener_api.h"
#include "modbus_thread.h"

int main(int argc, char *arg[]) {
    pthread_t modbus_thread;
    EIP_UINT16 nUniqueConnectionID;
    modbus_t *ctx;

    ctx = modbus_new_tcp(MODBUS_DEVICE_IP, MODBUS_TCP_DEFAULT_PORT);
    if (modbus_connect(ctx) == -1) {
	printf("Failed to initiate modbus_tcp connection to %s\n",
			MODBUS_DEVICE_IP);
	modbus_free(ctx);
	return -1;
    }

    setDeviceSerialNumber(123456789);
    nUniqueConnectionID = rand();
    CIP_Init(nUniqueConnectionID);

    if (EIP_OK != NetworkHandler_Init()) {
	printf("NetworkHandler_Init error\n");
	return 1;
    }

    pthread_create(&modbus_thread, NULL, poll_modbus, ctx);

    while (1) {
	if(EIP_OK != NetworkHandler_ProcessOnce()) break;
    }

    NetworkHandler_Finish();
    shutdownCIP();
    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}
