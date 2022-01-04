#include <lcom/lcf.h>

#include "simon.h"

//Screens
#include "menu.h"
#include "game.h"
#include "mp_game.h"
#include "host.h"
#include "number_of_rounds.h"
#include "how_to_play.h"
#include "leaderboard.h"
#include "highscore.h"

//Props and game state
#include "screen.h"
#include "num_and_letters.h"

//Devices
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "vc.h"
#include "rtc.h"
#include "uart.h"

void run1()
{
    bool exit = false;
    int ipc_status, r;
    uint32_t uart_irq_set, keyboard_irq_set, mouse_irq_set;
    message msg;
    uint8_t uart_hook_id, kbc_hook_id, mouse_hook_id;
    int uart_notification_id, kbc_notification_id, mouse_notification_id;
    int call_back = 0; //Error handling
    call_back |= uart_subscribe(&uart_hook_id, &uart_notification_id);
    call_back |= keyboard_subscribe(&kbc_hook_id, &kbc_notification_id);
    call_back |= mouse_subscribe(&mouse_hook_id, &mouse_notification_id);
    uart_irq_set = BIT(uart_hook_id);
    keyboard_irq_set = BIT(kbc_hook_id);
    mouse_irq_set = BIT(mouse_hook_id);

    enable_received_data_int();
    enable_trans_empty_int();

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
                if (msg.m_notify.interrupts & uart_irq_set)
                {
                    //uart_handler();
                }
                if (msg.m_notify.interrupts & keyboard_irq_set)
                { /* subscribed interrupt */
                    got_key_press = keyboard_handler(&keyboard_packet);
                }
                else
                {
                    got_key_press = false;
                    keyboard_packet.key = 0;
                }
                if (msg.m_notify.interrupts & mouse_irq_set)
                {
                    struct packet apacket;
                    mouse_handler(&apacket);
                    if (apacket.lb)
                    {
                        printf("mouse interrupt\n");
                        int answer = serial_write_byte('a');
                        printf("Wrote byte %x\n", answer);
                    }
                }
                exit = event_handler();
            default:
                continue; /* no other notifications expect do nothing */
            }
        }
        else
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    } while (!scancode_is_esc_break() && !exit); // Check for exit breakcode since the kbc_ih_output isn't altered in the keyboard_handler and can still be used here

    keyboard_unsubscribe(&kbc_notification_id);
    mouse_unsubscribe(&mouse_notification_id);
    //timer_unsubscribe_int();
    //rtc_unsubscribe(&rtc_notification_id);
    //vg_exit();
    int value = check_buffer();
    printf("Buffer, %x", value);
    bool is_emtpy = check_thr_emtpy();
    printf("THR is emtpy %x", is_emtpy);
    uart_unsubscribe(&uart_notification_id);
}
void run()
{

    bool exit = false;
    int ipc_status, r;
    uint32_t keyboard_irq_set, timer_irq_set, mouse_irq_set, rtc_irq_set, uart_irq_set;
    message msg;
    uint8_t kbd_hook_id, timer_hook_id, mouse_hook_id, rtc_hook_id, uart_hook_id;
    int kbd_notification_hook_id, mouse_notification_hook_id, rtc_notification_id, uart_notification_id;
    int call_back = 0; //Error handling

    printf("Starting program\n");

    if (get_mode_info(0x114) == 1)
    {
        return;
    }
    if (set_vbe_mode(0x114) == 1)
    {
        return;
    }

    srand(time(NULL));

    //Props load
    game = loadscreens();
    loadnums();
    loadlet();
    loadlilnums();
    loadlilet();

    enable_alarm_int();
    disable_updates();
    disable_periodic_int();
    call_back |= keyboard_subscribe(&kbd_hook_id, &kbd_notification_hook_id);
    call_back |= timer_subscribe_int(&timer_hook_id);
    call_back |= !mouse_data_reporting_enable();
    call_back |= mouse_subscribe(&mouse_hook_id, &mouse_notification_hook_id);
    call_back |= rtc_subscribe(&rtc_hook_id, &rtc_notification_id);
    call_back |= uart_subscribe(&uart_hook_id, &uart_notification_id);
    set_seconds_alarm();
    keyboard_irq_set = BIT(kbd_hook_id);
    timer_irq_set = BIT(timer_hook_id);
    mouse_irq_set = BIT(mouse_hook_id);
    rtc_irq_set = BIT(rtc_hook_id);
    uart_irq_set = BIT(uart_hook_id);

    uint8_t data;
    if (check_buffer() == 0)
    {
        serial_read_byte(&data);
    }

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

                if (msg.m_notify.interrupts & keyboard_irq_set)
                { /* subscribed interrupt */
                    got_key_press = keyboard_handler(&keyboard_packet);
                }
                else
                {
                    got_key_press = false;
                    keyboard_packet.key = 0;
                }
                if (msg.m_notify.interrupts & mouse_irq_set)
                {
                    got_mouse_packet = mouse_handler(&mouse_packet);
                }
                else
                {
                    got_mouse_packet = false;
                }
                if (msg.m_notify.interrupts & timer_irq_set)
                {
                    timer_interrupt = true;
                }
                else
                {
                    timer_interrupt = false;
                }
                if (msg.m_notify.interrupts & rtc_irq_set)
                {
                    if (rtc_handler() == ALARM)
                    {
                        rtc_interrupt = true;
                    }
                }
                else
                {
                    rtc_interrupt = false;
                }
                if (msg.m_notify.interrupts & uart_irq_set)
                {
                    printf("UART INTERRUPT\n");
                    uart_type_interrupt = uart_handler(&data_packet);
                    uart_interrupt = true;
                }
                else
                {
                    uart_interrupt = false;
                }
                exit = event_handler();
            default:
                continue; /* no other notifications expect do nothing */
            }
        }
        else
        { /* received a standard message, not a notification */
            /* no standard messages expected: do nothing */
        }
    } while (!scancode_is_esc_break() && !exit); // Check for exit breakcode since the kbc_ih_output isn't altered in the keyboard_handler and can still be used here

    keyboard_unsubscribe(&kbd_notification_hook_id);
    mouse_unsubscribe(&mouse_notification_hook_id);
    timer_unsubscribe_int();
    rtc_unsubscribe(&rtc_notification_id);
    vg_exit();
}

bool event_handler()
{
    static Game_Modes previous = 0;
    if (game.mode != previous)
    {
        previous = game.mode;
        printf("Game screen: %x", previous);
    }
    bool exit = false;
    switch (game.mode)
    {
    case MENU:
        draw_menu_graphics();
        if (count != 0)
        {
            if (got_key_press)
            {
                menu_update_status(keyboard_packet);
            }
        }
        count++;
        break;
    case HOST:
        draw_host_graphics();
        if (got_key_press)
        {
            host_update_status(keyboard_packet);
        }
        break;
    case HOW_TO_PLAY:
        draw_how_to_play_graphics();
        if (got_key_press)
        {
            how_to_play_update_status(keyboard_packet);
        }
        break;
    case LEADERBOARD:
        draw_leaderboard_graphics();
        if (got_key_press)
        {
            leaderboard_update_status(keyboard_packet);
        }
        break;
    case NUM_ROUNDS:
        draw_num_of_rounds_graphics();
        if (got_key_press)
        {
            number_of_rounds_update_status(keyboard_packet);
        }
        break;
    case GAME:
        draw_game_graphics();
        if (got_key_press)
        {
            game_update_status_key(keyboard_packet);
        }
        if (got_mouse_packet)
        {
            game_update_status_mouse(mouse_packet);
        }
        if (timer_interrupt)
        {
            game_update_status_timer();
        }
        if (rtc_interrupt)
        {
            game_update_status_rtc();
        }
        break;
    case MP_GAME:

        mp_draw_game_graphics();
        if (got_key_press)

        {
            mp_game_update_status_key(keyboard_packet);
        }
        if (got_mouse_packet)
        {
            mp_game_update_status_mouse(mouse_packet);
        }
        if (timer_interrupt)
        {
            mp_game_update_status_timer();
        }
        if (rtc_interrupt)
        {
            mp_game_update_status_rtc();
        }
        if (uart_interrupt)
        {
            printf("mp uart");
            mp_game_update_status_uart(data_packet, uart_type_interrupt);
        }
        break;

    case HIGHSCORE:
        printf("high\n");
        draw_highscore_graph();
        if (got_key_press)
        {
            highscore_update_status(keyboard_packet);
        }
        break;

    default:
        break;
    }
    if (game.mode == EXIT)
    {
        exit = true;
    }
    copy_to_video();
    return exit;
}
