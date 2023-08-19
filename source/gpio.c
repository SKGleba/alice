#include "include/types.h"
#include "include/utils.h"
#include "include/perv.h"

#include "include/gpio.h"

void gpio_set_port_mode(int bus, int port, int mode) {
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    gpio_regs[0] = (mode << port) | (gpio_regs[0] & ~(1 << port));
    dmb();
}

int gpio_port_read(int bus, int port) {
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    return (gpio_regs[1] >> port) & 1;
}

void gpio_port_set(int bus, int port) {
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    gpio_regs[2] |= 1 << port;
    gpio_regs[0xD];
    dsb();
}

void gpio_port_clear(int bus, int port) {
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    gpio_regs[3] |= 1 << port;
    gpio_regs[0xD];
    dsb();
}

void gpio_set_intr_mode(int bus, int port, int mode) {
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    unsigned int reg = 5 + port / 16;
    unsigned int off = 2 * (port % 16);

    gpio_regs[reg] |= (gpio_regs[reg] & ~(3 << off)) | (mode << off);

    dmb();
}

int gpio_query_intr(int bus, int port) {
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    return (1 << port) & ((gpio_regs[0x0E] & ~gpio_regs[0x07]) |
        (gpio_regs[0x0F] & ~gpio_regs[0x08]) |
        (gpio_regs[0x10] & ~gpio_regs[0x09]) |
        (gpio_regs[0x11] & ~gpio_regs[0x0A]) |
        (gpio_regs[0x12] & ~gpio_regs[0x0B]));
}

int gpio_acquire_intr(int bus, int port) {
    unsigned int ret;
    unsigned int mask = 1 << port;
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    ret = mask & ((gpio_regs[0x0E] & ~gpio_regs[0x07]) |
        (gpio_regs[0x0F] & ~gpio_regs[0x08]) |
        (gpio_regs[0x10] & ~gpio_regs[0x09]) |
        (gpio_regs[0x11] & ~gpio_regs[0x0A]) |
        (gpio_regs[0x12] & ~gpio_regs[0x0B]));
    gpio_regs[0x0E] = mask;
    gpio_regs[0x0F] = mask;
    gpio_regs[0x10] = mask;
    gpio_regs[0x11] = mask;
    gpio_regs[0x12] = mask;
    dsb();
    return ret;
}

void gpio_enable_port(int bus, int port) {
    unsigned int mask = 1 << port;
    volatile unsigned int* gpio_regs = GPIO_REGS(bus);
    gpio_regs[0x07] = ~(mask | ~gpio_regs[0x07]);
}

void gpio_init(bool init_leds) {
    pervasive_control_gate(PERV_CTRL_GATE_DEV_GPIO, 0b1, true, false);
    pervasive_control_reset(PERV_CTRL_RESET_DEV_GPIO, 0b1, false, false);
    if (init_leds) {
        gpio_set_port_mode(0, GPIO_PORT_PS_LED, GPIO_PORT_MODE_OUTPUT);
        gpio_set_port_mode(0, GPIO_PORT_GAMECARD_LED, GPIO_PORT_MODE_OUTPUT);
        for (int i = 16; i < 24; i++) // devkit leds
            gpio_set_port_mode(0, i, GPIO_PORT_MODE_OUTPUT);
    }
}