#include <lcom/lcf.h>

#include "uart.h"
#include "utils.h"

int uart_subscribe(uint8_t *bit_no, int *notification)
{
    int call_back = 0;
    int hook_id = UART_IRQ;
    *bit_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
    call_back = sys_irqsetpolicy(UART_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
    *notification = hook_id; // Return argument ("must be used in other calls to specify the interrupt notification to operate on")
    sys_irqenable(notification);
    return call_back;
}

int uart_unsubscribe(int *notification)
{
    sys_irqrmpolicy(notification);
    sys_irqdisable(notification);
    return 0;
}

uint8_t uart_handler(uint8_t *data)
{
    uint8_t orig = get_int_origin();
    printf("Interrupt origin %x\n", orig);
    if (orig == IIR_MOD_STAT)
    {
    }
    else if (orig == IIR_TRANS_EMPTY)
    {
    }
    else if (orig == IIR_REC_DATA)
    {
        printf("Got data from serial connection\n");
        serial_read_byte(data);
    }
    else if (orig == IIR_LINE_STAT)
    {
    }
    else if (orig == IIR_CHAR_TIMEOUT)
    {
    }
    return orig;
}

bool check_thr_emtpy()
{
    uint8_t lsr;
    //printf("Trying to sys_inb\n");
    util_sys_inb(COM1 + LSR, &lsr);
    //printf("Success sys_inb\n");
    //printf("Byte %x", lsr);
    return lsr & LSR_THR;
}

int check_buffer()
{
    bool error = false;
    bool empty = true;
    uint8_t lsr;
    util_sys_inb(COM1 + LSR, &lsr);

    if (lsr & LSR_FIFO_ERROR)
    {
        error = true;
    }
    if (lsr & LSR_REC_DATA)
    {
        empty = false;
    }

    if (error)
    {
        return 1;
    }
    if (empty)
    {
        return 2;
    }
    return 0;
}

bool check_data_dlab()
{
    uint8_t lcr;
    util_sys_inb(COM1 + LCR, &lcr);
    return lcr & !LCR_DLAB;
}

void set_data_dlab()
{
    uint8_t lcr;
    util_sys_inb(COM1 + LCR, &lcr);
    lcr = lcr & ~LCR_DLAB;
    sys_outb(COM1 + LCR, lcr);
}

bool serial_write_message(uint8_t *bytes, uint8_t size)
{
    bool success = true;
    for (int i = 0; i < size; i++)
    {
        printf("write message %x", bytes[i]);
        success = success & serial_write_byte(bytes[i]);
    }
    return success;
}

bool serial_write_byte(uint8_t byte)
{
    if (!check_data_dlab())
    {
        set_data_dlab();
    }
    if (check_thr_emtpy())
    {
        printf("Sending data, %x\n", byte);
        sys_outb(COM1 + SER_DATA, byte);
        return true;
    }
    printf("THR not empty not sending");
    return false;
}

bool seria_write_ack()
{
    return serial_write_byte(0);
}

bool serial_read_byte(uint8_t *byte)
{
    util_sys_inb(COM1 + SER_DATA, byte);
    printf("The received byte %x", *byte);
    return check_thr_emtpy();
}
void enable_received_data_int()
{
    uint8_t ier;
    util_sys_inb(COM1 + IER, &ier);
    ier = ier | IER_DATA_INT;
    sys_outb(COM1 + IER, ier);
}

void disable_received_data_int()
{
    uint8_t ier;
    util_sys_inb(COM1 + IER, &ier);
    ier = ier & ~!IER_DATA_INT;
    sys_outb(COM1 + IER, ier);
}

void enable_trans_empty_int()
{
    uint8_t ier;
    util_sys_inb(COM1 + IER, &ier);
    ier = ier | IER_TRANS_EMPTY_INT;
    sys_outb(COM1 + IER, ier);
}

void disable_trans_empty_int()
{
    uint8_t ier;
    util_sys_inb(COM1 + IER, &ier);
    ier = ier & ~!IER_TRANS_EMPTY_INT;
    sys_outb(COM1 + IER, ier);
}

uint8_t get_int_origin()
{
    uint8_t iir;
    util_sys_inb(COM1 + IIR, &iir);
    return (iir >> 1) & 0x7;
}
