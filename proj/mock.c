#include <lcom/lcf.h>

typedef enum {PERIODIC, ALARM, UPDATE, RTC_NO_INT} rtc_int_t;


/////////////////////////////
//RELATED TO SUBSCRIPTIONS AND SETTING OF THE MODE
void rtc_alarm_subscribe(uint8_t *bit_no, int *notification)
{
    uint8_t reg_b;
    sys_outb(RTC_ADDR_REG, REGISTER_B);
    util_sys_inb(RTC_DATA_REG, &reg_b);
    printf("%x state\n", reg_b);
    reg_b = reg_b | BIT(AIE);
    printf("%x state2\n", reg_b);

    sys_outb(RTC_ADDR_REG, REGISTER_B);
    sys_outb(RTC_DATA_REG, &reg_b);
}

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


void set_seconds_alarm()
{
    printf("Setting seconds alarm\n");
    uint8_t byte1 = 0xc0;
    uint8_t byte2 = 0xc0;
    uint8_t byte3 = 0xc0;

    //disable_updates();

    sys_outb(RTC_ADDR_REG, SECONDS_ALARM);
    sys_outb(RTC_DATA_REG, byte1);

    sys_outb(RTC_ADDR_REG, MINUTES_ALARM);
    sys_outb(RTC_DATA_REG, byte2);

    sys_outb(RTC_ADDR_REG, HOURS_ALARM);
    sys_outb(RTC_DATA_REG, byte3);

    //enable_updates();
}

//////////////////////////////////////
//RELATED TO THE INTERRUPT HANDLER
uint8_t clear_interruptions()
{
    sys_outb(RTC_ADDR_REG, REGISTER_C);
    uint8_t reg_c, byte;
    util_sys_inb(RTC_DATA_REG, &reg_c);
    byte = reg_c;

    //Disable register updates
    reg_c = reg_c & ~!BIT(PF) & ~!BIT(AF) & ~!BIT(UF);
    sys_outb(RTC_ADDR_REG, REGISTER_C);
    sys_outb(RTC_DATA_REG, reg_c);
    return byte;
}


rtc_int_t rtc_handler()
{
    static uint8_t previous = 0;
    uint8_t byte = clear_interruptions();

    //if (byte != previous)
    //{
        printf("%x\t", byte);
        previous = byte;
    //}
    if (byte & BIT(AF))
    {
        printf("%xTHERE WAS AN ALARM", byte);

        printf("alarm");
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
/////////////////////////

int main()
{
    int ipc_status, r;
    uint32_t rtc_irq_set;
    message m;
    uint8_t rtc_hook_id;
    int rtc_notification _id;
    rtc_irq_set = BIT(rtc_hook_id);

    rtc_alarm_subscribe(&rtc_hook_id, &rtc_notification_id);        //Sets the alarm bit
    set_seconds_alarm();                                            //Sets the alarm registers to 11XXXXXX
    call_back |= rtc_subscribe(&rtc_hook_id, &rtc_notification_id); // Subscribes the rtc interruptions

    do
    {
        /* Get a request message. */
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
        {
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status))
        { /* received notification */
            switch (_ENDPOINT_P(msg.m_source))
            {
            case HARDWARE: /* hardware interrupt notification */

                if (msg.m_notify.interrupts & rtc_irq_set)
                {
                    rtc_handler()
                }
            default:
                continue; /* no other notifications expect do nothing */
            }
        }
        else
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    } while (1);
}