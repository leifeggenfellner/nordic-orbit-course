/* mpu_sensor.h */
#ifndef MPU_SENSOR_H
#define MPU_SENSOR_H

#include <zephyr.h>
#include <logging/log.h>
#include <nrfx_twim.h>
#include "mpu_6050_registers.h"

#define LOG_MODULE_NAME_MPU mpu_sensor
#define MPU_TWI_BUFFER_SIZE 14
#define MPU_TWI_TIMEOUT 10000
#define MPU_ADDRESS 0x68

volatile static bool twi_xfer_done = false;
uint8_t twi_tx_buffer[MPU_TWI_BUFFER_SIZE];

uint8_t app_mpu_tx(const nrfx_twim_t *  p_instance, uint8_t address, uint8_t *p_data, uint8_t length, bool no_stop);
uint8_t app_mpu_rx(const nrfx_twim_t *   p_instance, uint8_t address, uint8_t *p_data, uint8_t length);
uint8_t wait_for_xfer_done(void)
uint8_t app_mpu_write_single_register(uint8_t reg, uint8_t data)
uint8_t app_mpu_write_registers(uint8_t reg, uint8_t * p_data, uint8_t length)
uint8_t app_mpu_read_registers(uint8_t reg, uint8_t * p_data, uint8_t length);
void my_twim_handler(nrfx_twim_evt_t const *p_event, void *p_context);
uint8_t twi_init(void);
uint8_t mpu_sensor_init(void);

#endif /* MPU_SENSOR_H */