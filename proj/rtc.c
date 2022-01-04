#include <lcom/lcf.h>
#include "rtc.h"
#include "utils.h"
#include "leaderboard.h"

int rtc_subscribe(uint8_t *bit_no, int *notification)
{
    int call_back = 0;
    int hook_id = RTC_IRQ;
    *bit_no = hook_id; // Return argument (bit used to check against mask interrupt subscription)
    call_back = sys_irqsetpolicy(RTC_IRQ, IRQ_REENABLE, &hook_id);
    sys_irqenable(&hook_id);
    *notification = hook_id; // Return argument ("must be used in other calls to specify the interrupt notification to operate on")
    return call_back;
}

void rtc_unsubscribe(int *notification)
{
    sys_irqrmpolicy(notification);
    sys_irqdisable(notification);
}

void enable_updates()
{
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    uint8_t reg_b;
    util_sys_inb(RTC_DATA_REG, &reg_b);
    reg_b = reg_b & ~BIT(SET);
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    sys_outb(RTC_DATA_REG, reg_b);
}

void disable_updates()
{
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    uint8_t reg_b;
    util_sys_inb(RTC_DATA_REG, &reg_b);

    reg_b = reg_b & ~BIT(UIE);
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    sys_outb(RTC_DATA_REG, reg_b);
}

rtc_int_t rtc_handler()
{
    static uint8_t previous = 0;
    uint8_t byte = clear_interruptions();

    if (byte != previous)
    {
        previous = byte;
    }
    if (byte & BIT(AF))
    {
        return ALARM;
    }
    else if (byte & BIT(UF))
    {
        return UPDATE;
    }
    else if (byte & BIT(PF))
    {
        return PERIODIC;
    }
    return RTC_NO_INT;
}

struct date get_date()
{
    bool got_date;
    struct date new_date;
    do
    {
        disable_updates();
        got_date = read_date(&new_date);
        enable_updates();

    } while (!got_date); //In case there is an update
    return new_date;
}

bool read_date(struct date *new_date)
{

    sys_outb(RTC_ADDR_REG, REGISTER_A);
    uint8_t reg_a;
    util_sys_inb(RTC_DATA_REG, &reg_a);
    if (reg_a & BIT(UIP))
    {
        //Couldn't read the date because it's updating
        return false;
    }

    uint8_t year, month, day;

    sys_outb(RTC_ADDR_REG, YEAR_REG);
    util_sys_inb(RTC_DATA_REG, &year);

    (*new_date).year = bcd_to_int(year);

    sys_outb(RTC_ADDR_REG, MONTH_REG);
    util_sys_inb(RTC_DATA_REG, &month);

    (*new_date).month = bcd_to_int(month);

    sys_outb(RTC_ADDR_REG, DAY_REG);
    util_sys_inb(RTC_DATA_REG, &day);

    (*new_date).day = bcd_to_int(day);

    return true;
}

void set_seconds_alarm()
{
    uint8_t byte = 0xC0;
    sys_outb(RTC_ADDR_REG, SECONDS_ALARM);
    sys_outb(RTC_DATA_REG, byte);

    sys_outb(RTC_ADDR_REG, MINUTES_ALARM);
    sys_outb(RTC_DATA_REG, byte);

    sys_outb(RTC_ADDR_REG, HOURS_ALARM);
    sys_outb(RTC_DATA_REG, byte);
}

void set_timed_alarm(uint8_t seconds)
{
    uint8_t byte = 0xC0;
    uint8_t data;
    sys_outb(RTC_ADDR_REG, SECONDS_REG);
    util_sys_inb(RTC_DATA_REG, &data);
    int now = bcd_to_int(data);

    sys_outb(RTC_ADDR_REG, SECONDS_ALARM);
    sys_outb(RTC_DATA_REG, int_to_bcd((now + seconds) % 60));

    sys_outb(RTC_ADDR_REG, MINUTES_ALARM);
    sys_outb(RTC_DATA_REG, byte);

    sys_outb(RTC_ADDR_REG, HOURS_ALARM);
    sys_outb(RTC_DATA_REG, byte);
}

uint8_t clear_interruptions()
{
    sys_outb(RTC_ADDR_REG, REGISTER_C);
    uint8_t reg_c, byte;
    util_sys_inb(RTC_DATA_REG, &reg_c);
    byte = reg_c;

    //Disable register updates
    //reg_c = reg_c & (~BIT(AIE));
    //sys_outb(RTC_ADDR_REG, REGISTER_C);
    //sys_outb(RTC_DATA_REG, reg_c);
    return byte;
}

int bcd_to_int(uint8_t number)
{
    return (number & 0x0F) + (number >> 4) * 10;
}

uint8_t int_to_bcd(int number)
{
    return (uint8_t)(number % 10) | ((uint8_t)(number / 10)) << 4;
}

void disable_periodic_int()
{
    uint8_t reg_b;

    sys_outb(RTC_ADDR_REG, REGISTER_B);
    util_sys_inb(RTC_DATA_REG, &reg_b);

    reg_b = reg_b & (~BIT(UIE));
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    sys_outb(RTC_DATA_REG, reg_b);
}

void enable_alarm_int()
{
    uint8_t reg_b;
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    util_sys_inb(RTC_DATA_REG, &reg_b);
    reg_b = reg_b | BIT(AIE);

    sys_outb(RTC_ADDR_REG, REGISTER_B);
    sys_outb(RTC_DATA_REG, reg_b);
}

void disable_alarm_int()
{
    uint8_t reg_b;

    sys_outb(RTC_ADDR_REG, REGISTER_B);
    util_sys_inb(RTC_DATA_REG, &reg_b);
    reg_b = reg_b & (~BIT(AIE));
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    sys_outb(RTC_DATA_REG, &reg_b);
}
