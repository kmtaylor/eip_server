#include <string.h>
#include <stdlib.h>

#include "opener_api.h"
#include "modbus_thread.h"

#define PAYLOAD_SIZE			14

#define INPUT_ASSEMBLY_NUM                100
#define OUTPUT_ASSEMBLY_NUM               150
#define CONFIG_ASSEMBLY_NUM               1

EIP_UINT8 eip_input_data[PAYLOAD_SIZE];
EIP_UINT8 eip_output_data[PAYLOAD_SIZE];
EIP_UINT8 eip_config_data[4];

EIP_STATUS IApp_Init(void) {
    createAssemblyObject(INPUT_ASSEMBLY_NUM, eip_input_data,
	sizeof(eip_input_data));

    createAssemblyObject(OUTPUT_ASSEMBLY_NUM, eip_output_data,
	sizeof(eip_output_data));

    createAssemblyObject(CONFIG_ASSEMBLY_NUM, eip_config_data,
	sizeof(eip_config_data));

    configureExclusiveOwnerConnectionPoint(0, OUTPUT_ASSEMBLY_NUM,
	INPUT_ASSEMBLY_NUM, CONFIG_ASSEMBLY_NUM);

    return EIP_OK;
}

void IApp_HandleApplication(void) {
    /* check if application needs to trigger an connection */
}

void IApp_IOConnectionEvent(unsigned int pa_unOutputAssembly,
    unsigned int pa_unInputAssembly, EIOConnectionEvent pa_eIOConnectionEvent) {
    /* maintain a correct output state according to the connection state*/

    (void) pa_unOutputAssembly; /* suppress compiler warning */
    (void) pa_unInputAssembly; /* suppress compiler warning */
    (void) pa_eIOConnectionEvent; /* suppress compiler warning */
}

EIP_STATUS IApp_AfterAssemblyDataReceived(S_CIP_Instance *pa_pstInstance) {
    EIP_STATUS nRetVal = EIP_OK;

    /*handle the data received e.g., update outputs of the device */
    switch (pa_pstInstance->nInstanceNr) {
	case OUTPUT_ASSEMBLY_NUM:
	    /* Data for the output assembly has been received. */
	    break;
	case CONFIG_ASSEMBLY_NUM:
	/* Add here code to handle configuration data and check if it is ok
	    * The demo application does not handle config data.
	    * However in order to pass the test we accept any data given.
	    * EIP_ERROR */
	    break;
    }
    return nRetVal;
}

#define MAP_8BIT(index, target, first, last) \
    eip_input_data[index] =						    \
	(uint8_t) modbus_data.reg_##first##_##last[target - first]

#define MAP_16BIT(index, target, first, last) \
    eip_input_data[index] =						    \
	(uint8_t) modbus_data.reg_##first##_##last[target - first];	    \
    eip_input_data[index + 1] =						    \
	(uint8_t) (modbus_data.reg_##first##_##last[target - first] >> 8)

#define MAP_32BIT(index, target, first, last) \
    tmp_32 = modbus_data.reg_##first##_##last[target - first + 1] |	    \
	     modbus_data.reg_##first##_##last[target - first] << 16;	    \
    memcpy(&eip_input_data[index], &tmp_32, sizeof(tmp_32))

#define SE_64BIT(index, target, first, last) \
    tmp_32 = modbus_data.reg_##first##_##last[target - first + 1] |	    \
	     modbus_data.reg_##first##_##last[target - first] << 16;	    \
    tmp_64 = (int64_t) tmp_32;						    \
    memcpy(&eip_input_data[index], &tmp_64, sizeof(tmp_64))

EIP_BOOL8 IApp_BeforeAssemblyDataSend(S_CIP_Instance *pa_pstInstance) {
    int i;
    static int no_print;
    int32_t tmp_32;
    int64_t tmp_64;
    pthread_mutex_lock(&modbus_lock);

    /* Fill buffer with modbus data */
    SE_64BIT(0, 352, 352, 353);
    MAP_16BIT(8, 902, 902, 902);
    MAP_8BIT(12, 826, 826, 826);

    pthread_mutex_unlock(&modbus_lock);

    if (!no_print) {
	printf("Transferring: ");
	for (i = 0; i < 14; i++) {
	    printf("%x ", eip_input_data[i]);
	}
	printf("\n");
	no_print = 1;
    }

    return true;
}

EIP_STATUS IApp_ResetDevice(void) {
    return EIP_OK;
}

EIP_STATUS IApp_ResetDeviceToInitialConfiguration(void) {
    IApp_ResetDevice();
    return EIP_OK;
}

void *IApp_CipCalloc(unsigned pa_nNumberOfElements,
					unsigned pa_nSizeOfElement) {
    return calloc(pa_nNumberOfElements, pa_nSizeOfElement);
}

void IApp_CipFree(void *pa_poData) {
    free(pa_poData);
}

void IApp_RunIdleChanged(EIP_UINT32 pa_nRunIdleValue) {
    (void) pa_nRunIdleValue;
}
