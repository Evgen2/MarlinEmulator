/* mbed.cpp */
#include "stdafx.h"
#include "mbed.h"
#include "mbed_assert.h"

/** Waits for a number of seconds, with microsecond resolution (within
 *  the accuracy of single precision floating point).
 *
 *  @param s number of seconds to wait
 */
void wait(float s)
{  int w;
   w = s* 1000;
   Sleep(w);
}

/** Waits a number of milliseconds.
 *
 *  @param ms the whole number of milliseconds to wait
 */
void wait_ms(int ms)
{   Sleep(ms);
}

void mbed_assert_internal(const char *expr, const char *file, int line)
{
   printf("mbed assertation failed: %s, file: %s, line %d \n", expr, file, line);
}

/******************************************************************/
void pin_function(PinName pin, int function) {
    MBED_ASSERT(pin != (PinName)NC);

    uint32_t pin_number = (uint32_t)pin - (uint32_t)P0_0;
    int index = pin_number >> 4;
    int offset = (pin_number & 0xF) << 1;
    
    PINCONARRAY->PINSEL[index] &= ~(0x3 << offset);
    PINCONARRAY->PINSEL[index] |= function << offset;
}

void pin_mode(PinName pin, PinMode mode) {
    MBED_ASSERT(pin != (PinName)NC);
    
    uint32_t pin_number = (uint32_t)pin - (uint32_t)P0_0;
    int index = pin_number >> 5;
    int offset = pin_number & 0x1F;
    uint32_t drain = ((uint32_t) mode & (uint32_t) OpenDrain) >> 2;
    
    PINCONARRAY->PINMODE_OD[index] &= ~(drain << offset);
    PINCONARRAY->PINMODE_OD[index] |= drain << offset;
    
    if (!drain) {
        index = pin_number >> 4;
        offset = (pin_number & 0xF) << 1;
        
        PINCONARRAY->PINMODE[index] &= ~(0x3 << offset);
        PINCONARRAY->PINMODE[index] |= (uint32_t)mode << offset;
    }
}

#if 0
uint32_t gpio_set(PinName pin) {
    MBED_ASSERT(pin != (PinName)NC);
    pin_function(pin, 0);
    return (1 << ((int)pin & 0x1F));
}
#endif //0
void gpio_init(gpio_t *obj, PinName pin) {
	int n;
	LPC_GPIO_TypeDef *port_reg;
    obj->pin = pin;
    if (pin == (PinName)NC)
        return;

    obj->mask = gpio_set(pin);
	n = pin/32;
	switch(n)
	{  case 0:
		    port_reg = &LPC_gpio0; //(LPC_GPIO_TypeDef *)((int)pin & ~0x1F);
			break;
	case 1:
		    port_reg = &LPC_gpio1;
		break;
	case 2:
		    port_reg = &LPC_gpio2;
		break;
	case 3:
		    port_reg = &LPC_gpio3;
		break;
	}
//    LPC_GPIO_TypeDef *port_reg = (LPC_GPIO_TypeDef *)((int)pin & ~0x1F);
    obj->reg_set = &port_reg->FIOSET;
    obj->reg_clr = &port_reg->FIOCLR;
    obj->reg_in  = &port_reg->FIOPIN;
    obj->reg_dir = &port_reg->FIODIR;
}

#if 0
void gpio_mode(gpio_t *obj, PinMode mode) {
    pin_mode(obj->pin, mode);
}

void gpio_dir(gpio_t *obj, PinDirection direction) {
    MBED_ASSERT(obj->pin != (PinName)NC);
    switch (direction) {
        case PIN_INPUT :
            *obj->reg_dir &= ~obj->mask;
            break;
        case PIN_OUTPUT:
            *obj->reg_dir |= obj->mask;
            break;
    }
}

#endif //0

static inline void _gpio_init_in(gpio_t* gpio, PinName pin, PinMode mode)
{
    gpio_init(gpio, pin);
    if (pin != NC) {
        gpio_dir(gpio, PIN_INPUT);
        gpio_mode(gpio, mode);
    }
}

static inline void _gpio_init_out(gpio_t* gpio, PinName pin, PinMode mode, int value)
{
    gpio_init(gpio, pin);
    if (pin != NC) {
        gpio_write(gpio, value);
        gpio_dir(gpio, PIN_OUTPUT);
        gpio_mode(gpio, mode);
    }
}

void gpio_init_in_ex(gpio_t* gpio, PinName pin, PinMode mode) {
    _gpio_init_in(gpio, pin, mode);
}

void gpio_init_in(gpio_t* gpio, PinName pin) {
    gpio_init_in_ex(gpio, pin, PullDefault);
}


void gpio_init_out_ex(gpio_t* gpio, PinName pin, int value) {
    _gpio_init_out(gpio, pin, PullNone, value);
}

void gpio_init_out(gpio_t* gpio, PinName pin) {
    gpio_init_out_ex(gpio, pin, 0);
}

/******************************************************************/

void Ticker::handler(void)
{  printf("todo " __FUNCTION__ "\n");
}
void Ticker::detach(void)
{  printf("todo " __FUNCTION__ "\n");
}

