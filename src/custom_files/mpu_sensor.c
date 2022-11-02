#include "mpu_sensor.h"

const nrfx_twim_t m_twim_instance = NRFX_TWIM_INSTANCE(0);

LOG_MODULE_REGISTER(LOG_MODULE_NAME_MPU);

uint8_t app_mpu_tx(const nrfx_twim_t *p_instance, uint8_t address, uint8_t *p_data, uint8_t length, bool no_stop)
{
    int err;

    nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_TX(address, p_data, length);
    err = nrfx_twim_xfer(p_instance, &xfer, 0);
    if (err != NRFX_SUCCESS)
    {
        return err;
    }

    return 0;
}

uint8_t app_mpu_rx(const nrfx_twim_t *p_instance, uint8_t address, uint8_t *p_data, uint8_t length)
{
    int err;
    nrfx_twim_xfer_desc_t xfer = NRFX_TWIM_XFER_DESC_RX(address, p_data, length);

    err = nrfx_twim_xfer(p_instance, &xfer, 0);
    if (err != NRFX_SUCCESS)
    {
        return err;
    }
    return 0;
}

uint8_t wait_for_xfer_done(void)
{
    int timeout = MPU_TWI_TIMEOUT;
    while ((!twi_xfer_done) && --timeout)
    {
        // Wait...
    }
    if (timeout == 0)
    {
        return NRFX_ERROR_TIMEOUT;
    }
    return 0;
}

uint8_t app_mpu_write_single_register(uint8_t reg, uint8_t data)
{
    int err;

    uint8_t packet[2] = {reg, data};

    twi_xfer_done = false; // reset for new xfer
    err = app_mpu_tx(&m_twim_instance, MPU_ADDRESS, packet, 2, false);
    if (err)
    {
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT)
    {
        return err;
    }

    return 0;
}

uint8_t app_mpu_write_registers(uint8_t reg, uint8_t *p_data, uint8_t length)
{
    int err;

    twi_tx_buffer[0] = reg;
    memcpy((twi_tx_buffer + 1), p_data, length);

    nrfx_twim_xfer_desc_t xfer = {0};
    xfer.address = MPU_ADDRESS;
    xfer.type = NRFX_TWIM_XFER_TX;
    xfer.primary_length = length + 1;
    xfer.p_primary_buf = twi_tx_buffer;

    twi_xfer_done = false; // reset for new xfer
    err = nrfx_twim_xfer(&m_twim_instance, &xfer, 0);
    if (err != NRFX_SUCCESS)
    {
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT)
    {
        return err;
    }

    return 0;
}

uint8_t app_mpu_read_registers(uint8_t reg, uint8_t *p_data, uint8_t length)
{
    int err;

    twi_xfer_done = false; // reset for new xfer
    err = app_mpu_tx(&m_twim_instance, MPU_ADDRESS, &reg, 1, false);
    if (err)
    {
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT)
    {
        return err;
    }

    twi_xfer_done = false; // reset for new xfer
    err = app_mpu_rx(&m_twim_instance, MPU_ADDRESS, p_data, length);
    if (err)
    {
        LOG_ERR("app_mpu_rx returned %08x", err);
        return err;
    }
    err = wait_for_xfer_done();
    if (err == NRFX_ERROR_TIMEOUT)
    {
        return err;
    }

    return 0;
}

void my_twim_handler(nrfx_twim_evt_t const *p_event, void *p_context)
{
    LOG_INF("TWIM callback");
}

uint8_t twi_init(void)
{
    // Setup peripheral interrupt.
    IRQ_CONNECT(DT_IRQN(DT_NODELABEL(i2c0)), DT_IRQ(DT_NODELABEL(i2c0), priority), nrfx_isr, nrfx_twim_0_irq_handler, 0);
    irq_enable(DT_IRQN(DT_NODELABEL(i2c0)));

    int err = 0;

    const nrfx_twim_config_t twim_config = {
        .scl = 4,
        .sda = 3,
        .frequency = NRF_TWIM_FREQ_400K,
        .interrupt_priority = NRFX_TWIM_DEFAULT_CONFIG_IRQ_PRIORITY,
        .hold_bus_uninit = false,
    };

    err = nrfx_twim_init(&m_twim_instance, &twim_config, my_twim_handler, NULL);

    if (err != NRFX_SUCCESS)
    {
        LOG_ERR("twim_init failed. (err %x)", err);
        return err;
    }

    nrfx_twim_enable(&m_twim_instance);

    return 0;
}

uint8_t mpu_sensor_init(void)
{
    int err;

    LOG_INF("Initializing MPU Sensor");

    err = twi_init();

    if (err)
    {
        return err;
    }

    return err;
}