#include "include/types.h"
#include "include/clib.h"
#include "include/gpio.h"
#include "include/spi.h"
#include "include/utils.h"
#include "include/paddr.h"
#include "include/ernie.h"

ernie_comms_t g_ernie_comms;

void ernie_write(uint8_t *data, uint8_t size) {
    gpio_port_clear(0, GPIO_PORT_ERNIE_OUT);
    spi_write_start(0);
    for (int i = 0; i < size; i+=2)
        spi_write(0, (data[i + 1] << 8) | data[i]);
    spi_write_end(0);
    gpio_port_set(0, GPIO_PORT_ERNIE_OUT);
}

bool ernie_read(uint8_t *data, uint8_t max_size) {
    while (!gpio_query_intr(0, GPIO_PORT_ERNIE_IN)){};
    gpio_acquire_intr(0, GPIO_PORT_ERNIE_IN);
    uint8_t data_read = 0;
    uint32_t tmp_rd = 0;
    bool is_oob = false;
    while (spi_read_available(0)) {
        if (data_read >= max_size) {
            is_oob = true;
            break;
        }
        tmp_rd = spi_read(0);
        data[data_read] = tmp_rd & 0xFF;
        if (data_read + 1 < max_size)
            data[data_read + 1] = (tmp_rd >> 8) & 0xFF;
        data_read += 2;
    }
    spi_read_end(0);
    gpio_port_clear(0, GPIO_PORT_ERNIE_OUT);
    return is_oob;
}

int ernie_exec(uint8_t* tx, uint8_t tx_size, uint8_t* rx, uint8_t rx_size) {
    uint8_t hash = 0, result = 0, length = 0;

    if (!tx || !rx)
        return -1;
    if (tx_size < ERNIE_TX_SIZE(0) || rx_size < ERNIE_RX_SIZE(0))
        return -2;
    
    length = tx[ERNIE_TX_LENGTH];
    if (length > (tx_size - ERNIE_TX_SIZE(0)))
        return -3;

    // hash
    tx[ERNIE_TX_LENGTH] = length + 1;
    for (uint8_t i = 0; i < ERNIE_TX_DATA(0) + length; i++)
        hash += tx[i];
    tx[ERNIE_TX_DATA(0) + length] = ~hash;

    do {
        memset(rx, -1, rx_size);
        ernie_write(tx, ERNIE_TX_SIZE(length));
        ernie_read(rx, rx_size);
        result = rx[ERNIE_RX_RESULT];
    } while (result == 0x80 || result == 0x81);

    return result;
}

int ernie_exec_cmd(uint16_t cmd, void *data_in, uint8_t data_in_size) {
    if (ERNIE_TX_SIZE(data_in_size) > sizeof(g_ernie_comms.tx))
        return -1;
    memset(&g_ernie_comms, 0, sizeof(g_ernie_comms));
    g_ernie_comms.tx[ERNIE_TX_CMD_LO] = cmd & 0xFF;
    g_ernie_comms.tx[ERNIE_TX_CMD_HI] = (cmd >> 8) & 0xFF;
    g_ernie_comms.tx[ERNIE_TX_LENGTH] = data_in_size;
    if (data_in)
        memcpy(&g_ernie_comms.tx[ERNIE_TX_DATA(0)], data_in, data_in_size);
    ernie_exec(g_ernie_comms.tx, sizeof(g_ernie_comms.tx), g_ernie_comms.rx, sizeof(g_ernie_comms.rx));
    return (int)*(uint32_t *)(g_ernie_comms.rx);
}

int ernie_exec_cmd_short(uint16_t cmd, uint32_t data_in, uint8_t data_in_size) {
    return ernie_exec_cmd(cmd, (data_in_size) ? &data_in : NULL, data_in_size);
}
