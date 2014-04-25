#define MODBUS_DEVICE_IP "192.168.250.150"
#define MODBUS_POLL_INTERVAL_US 200000
#define MODBUS_ERROR_VAL 0xFF

extern pthread_mutex_t modbus_lock;

#define DECLARE_REGS(first, last) \
    uint16_t reg_##first##_##last[last - first + 1]

struct s_modbus_data {
    DECLARE_REGS(826, 826);
    DECLARE_REGS(352, 353);
    DECLARE_REGS(902, 902);
};

/* Only access with modbus_lock */
extern struct s_modbus_data modbus_data;

void *poll_modbus(void *none);
