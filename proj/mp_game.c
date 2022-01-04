#include <lcom/lcf.h>

#include "mp_game.h"
#include "consts.h"
#include "screen.h"
#include "keyboard.h"
#include "vc.h"
#include "num_and_letters.h"
#include "leaderboard.h"
#include "highscore.h"
#include "rtc.h"
#include "uart.h"


void mp_draw_game_graphics()
{
    if (!game_end)
    {
        mp_draw_buttons();

        mp_draw_player();

        mp_draw_counter();

        mp_draw_round();

        mp_draw_mouse();
    }
    else
    {
        mp_draw_winner_graphics();
    }
}

void mp_draw_winner_graphics()
{
    switch (ws)
    {
    case WINNER1:
        draw_xpm(game.s[6]);
        break;
    case WINNER2:
        draw_xpm(game.s[7]);
        break;
    case TIE:
        draw_xpm(game.s[18]);
        break;
    }
}

void mp_draw_buttons()
{
    switch (ps)
    {
    case DEFAULT:

        draw_xpm(game.s[9]);

        break;
    case GREEN:

        draw_xpm(game.s[10]);

        break;
    case YELLOW:

        draw_xpm(game.s[11]);

        break;
    case PURPLE:

        draw_xpm(game.s[12]);

        break;
    case BLUE:

        draw_xpm(game.s[13]);

        break;
    case RED:

        draw_xpm(game.s[14]);

        break;
    case VS:

        draw_xpm(game.s[8]);

        break;
    case WHITE:

        draw_xpm(game.s[15]);

        break;
    }
}

void mp_draw_player()
{
    if (is_player_1)
    {
        mp_print_player1();
    }
    else
    {
        mp_print_player2();
    }
}

void mp_draw_counter()
{
    static xpm_image_t number;
    static int previous_time = -1;
    int time_left;
    if (time_left_for_click > 0)
    {
        time_left = time_left_for_click;
    }
    else
    {
        time_left = 0;
    }
    if (previous_time != time_left)
    {
        number = *get_prop_from_char('0' + time_left);
        previous_time = time_left;
    }

    draw_xpm_x_y(number, 700, 0);
}

void mp_draw_round()
{
    static int round = -1;
    static struct text_box round_text;

    if (round != buttons_size)
    {

        round = buttons_size;
        int numbers_size = 0;
        char chars[3] = {'0', '0', '0'};
        int temp_round = round;
        while (temp_round > 0)
        {
            chars[numbers_size] = (temp_round % 10) + '0';
            temp_round /= 10;
            numbers_size++;
        }
        if (numbers_size == 0)
        {
            numbers_size = 1;
        }
        else if (numbers_size == 2)
        {
            int temp = chars[0];
            chars[0] = chars[1];
            chars[1] = temp;
        }
        else if (numbers_size == 3)
        {
            int temp = chars[0];
            chars[0] = chars[2];
            chars[2] = temp;
        }

        round_text = create_static_text_box(0, 50, chars, numbers_size);
    }

    draw_text_box(&round_text);
}

void mp_game_update_status_mouse(struct packet mouse_packet)
{

    if (game_end)
    {
        //We have ended the game, since no need to handle the mouse packet
        return;
    }
    if (game_started)
    {
        button_click_t click;
        if (!mouse_packet.x_ov)
        {
            if (mp_mouse.x_pos < -mouse_packet.delta_x)
            {
                mp_mouse.x_pos = 1;
            }
            else if (mp_mouse.x_pos + mouse_packet.delta_x >= get_mode_width() - MOUSE_WIDTH)
            {
                mp_mouse.x_pos = get_mode_width() - MOUSE_WIDTH;
            }
            else
            {
                mp_mouse.x_pos += mouse_packet.delta_x;
            }
        }
        if (!mouse_packet.y_ov)
        {
            if (mp_mouse.y_pos < mouse_packet.delta_y)
            {
                mp_mouse.y_pos = 1;
            }
            else if (mp_mouse.y_pos - mouse_packet.delta_y >= get_mode_height() - MOUSE_HEIGHT)
            {
                mp_mouse.y_pos = get_mode_height() - MOUSE_HEIGHT;
            }
            else
            {
                mp_mouse.y_pos -= mouse_packet.delta_y;
            }
        }

        if (mouse_packet.lb)
        {
            if (mp_mouse.left_hold)
            {
                mp_mouse.left_click = false;
            }
            else
            {
                mp_mouse.left_click = true;
                mp_mouse.left_hold = true;
            }
        }
        else
        {
            mp_mouse.left_hold = false;
            mp_mouse.left_click = false;
        }

        if (mp_mouse.left_click)
        {
            click = mp_get_button_click();
        }
        else
        {
            click = NO_BUTTON;
        }
        if (is_host == is_player_1)
        {
            mp_buttons_handler(click);
        }
    }
    else
    {
        return;
    }
}

button_click_t mp_get_button_click()
{

    if (mp_get_red_click())
    {
        return RED_B;
    }
    else if (mp_get_green_click())
    {
        return GREEN_B;
    }
    else if (mp_get_purple_click())
    {
        return PURPLE_B;
    }
    else if (mp_get_blue_click())
    {
        return BLUE_B;
    }
    else if (mp_get_yellow_click())
    {
        return YELLOW_B;
    }
    else if (mp_get_vs_click())
    {
        return VS_B;
    }
    else
    {
        return NO_BUTTON;
    }
}

void mp_buttons_handler(button_click_t click)
{
    if (is_host == is_player_1) //We can only click when it is our turn
    {
        mp_click_button(click);
    }
    else
    {
        mp_remote_click_button(click);
    }
    if (click != NO_BUTTON)
    {
        mp_light_button(click);
        time_left_for_screen = TIMER_BUTTON_MP;
    }
}

void mp_click_button(button_click_t click)
{
    if (click == NO_BUTTON)
    {
        //Didn't click any button
        mp_mouse.left_click = false; // DEBUG;
        return;
    }

    if (guessed == buttons_size || VS_was_pressed)
    {
        if (guessed == buttons_size)
        {
            player_score = buttons_size;
        }
        //We are going to add another button
        if (guessed == max_buttons_size)
        {
            ws = TIE;
            mp_end_game();
            mp_send_winner();
        }
        else
        {
            if (is_vs_round)
            {
                if (VS_was_pressed)
                {
                    mp_add_button(click);
                    mp_send_button(click);
                }
                else
                {
                    if (click == VS_B)
                    {

                        VS_was_pressed = true;
                        VS_screen = true;
                        mp_add_button(click);
                        mp_send_button(click);
                        time_left_for_click = VS_INTERVAL;
                    }
                    else
                    {

                        mp_add_button(click);
                        mp_send_button(click);
                        mp_change_player();
                    }
                }
            }
            else
            {
                mp_add_button(click);
                mp_send_button(click);
                mp_change_player();
            }
        }
    }
    else
    {
        //We are checking if it clicked the right button
        if (buttons[guessed] == click)
        {
            mp_send_button(click);
            guessed++;
            if (guessed == buttons_size)
            {
                time_left_for_click = -1;
            }
            else
            {
                time_left_for_click = INTERVAL;
            }
        }
        else
        {
            mp_get_winner();
            mp_send_winner();
            mp_end_game();
        }
    }
}

void mp_remote_click_button(button_click_t click)
{

    if (guessed == buttons_size || VS_was_pressed)
    {
        if (guessed == buttons_size)
        {
            player_score = buttons_size;
        }
        //We are going to add another button
        if (guessed == max_buttons_size)
        {
            ws = TIE;
            mp_end_game();
        }
        else
        {
            if (is_vs_round)
            {
                if (VS_was_pressed)
                {
                    mp_add_button(click);
                }
                else
                {
                    if (click == VS_B)
                    {

                        VS_was_pressed = true;
                        VS_screen = true;
                        mp_add_button(click);
                    }
                    else
                    {

                        mp_add_button(click);
                        // We will receive the uart interruption after
                    }
                }
            }
            else
            {
                mp_add_button(click);
                // We will receive the uart interruption after
            }
        }
    }
    else
    {
        //We are checking if it clicked the right button
        if (buttons[guessed] == click)
        {
            // TODO Press the button on the screens
            //continue
            guessed++;
        }
        else
        {
            // We will receive a LOSE uart interruption
        }
    }
}

void mp_light_button(button_click_t click)
{
    switch (click)
    {
    case GREEN_B:
        ps = GREEN;
        break;
    case YELLOW_B:
        ps = YELLOW;
        break;
    case PURPLE_B:
        ps = PURPLE;
        break;
    case BLUE_B:
        ps = BLUE;
        break;
    case RED_B:
        ps = RED;
        break;
    case VS_B:
        if (VS_was_pressed && VS_screen)
        {
            ps = VS;
        }
        else
        {
            ps = WHITE;
        }
        break;
    case NO_BUTTON:
        ps = DEFAULT;
        break;
    }
}

void mp_game_update_status_key(struct key_press key)
{
    if (game_end)
    {
        if (key.key == ENTER)
        {
            if (ws == TIE)
            {
                game.mode = LEADERBOARD;
            }
            else
            {
                mp_checkNewHiScore(player_score);
            }
        }
    }
}

void mp_draw_mouse()
{
    draw_xpm_x_y(*get_mouse(), mp_mouse.x_pos, mp_mouse.y_pos);
}

void mp_game_update_status_timer()
{
    mp_mouse.left_click = false;
    if (time_left_for_screen == 0)
    {
        ps = DEFAULT;
    }
    else
    {
        time_left_for_screen--;
    }
    // After a frame, it's no longer holding the button
}

void mp_game_update_status_rtc()
{
    if (is_host == is_player_1 && !game_end)
    {
        //CAUSE LIKE... if we set the timer.... and the serial port delays, then it will lose the game
        // even though we aren't the one playing
        //IT DOES... it just is so fast you barely see it
        // I am going to fix that too dw
        // I think this was is not too bad
        // Unless I create a flag... which... imo is much better?
        //Indeed... when presses the white and is_vs_round and !vs was pressed
        //It sets a flag
        // If the flag is set, then we display that screen, otherwise we dont'
        // Flag
        //No, right now we have a flag to know if it was ever pressed
        //
        //Player logic
        if (time_left_for_click == 0 && !VS_was_pressed)
        {
            // Player that is playing loses
            mp_get_winner();
            mp_send_winner();
            mp_end_game();
            return;
        }
        else if (time_left_for_click == 0 && is_vs_round && VS_was_pressed)
        {
            mp_change_player();
        }
        else if (time_left_for_click > 0)
        {
            time_left_for_click--;
        }
        else
        {
            // time_left_for_click is -1
            //We have infinite time
        }
    }

    if (game_end)
    {
        set_seconds_alarm(); //Reset the previous alarms

        if (ws == TIE)
        {
            game.mode = LEADERBOARD;
        }
        else
        {
            mp_checkNewHiScore(player_score);
        }
    }
}

void mp_game_update_status_uart(uint8_t data, uint8_t type)
{
    printf("Got byte %x", data);
    switch (type)
    {
    case IIR_REC_DATA:
        if (!game_started)
        {
            start_game(data, false);
            return;
        }
        play_game(data);
        //We are receiving buttons or REady or IS_VS or IS_NOT_VS

        break;
    case IIR_TRANS_EMPTY:
        if (!game_started)
        {
            start_game(CONNECTED, true);
            return;
        }
        if (uart_has_data_to_send)
        {
            uart_ready();
        }
        break;
    default:
        break;
    }
}

void play_game(uint8_t data)
{
    //    uint8_t data = YELLOW + click;
    if (data >= 3 && data <= 8)
    {
        //We received a button
        button_click_t click = (button_click_t)(data - GREEN_REMOTE); //I THINK THE PROBLEM IS HERE
        mp_buttons_handler(click);
    }
    else if (data == IS_VS) //We are the client and a new round is starting
    {
        is_vs_round = true;
        VS_was_pressed = false;
        guessed = 0;
        is_player_1 = !is_player_1;
        time_left_for_click = INTERVAL;
    }
    else if (data == IS_NOT_VS) //We are the client and a new round is starting
    {
        is_vs_round = false;
        VS_was_pressed = false;
        guessed = 0;
        is_player_1 = !is_player_1;
        time_left_for_click = INTERVAL;
    }
    else if (data == READY) // We are the host
    {
        mp_is_VS_round();
        guessed = 0;
        is_player_1 = !is_player_1;
        time_left_for_click = INTERVAL;
    }
    else if (data == LOSE) //The other player lost and is already displaying the lose screen, now it is up to us to do so
    // Each player already has the score (The most memorized was done by us)
    {
        if (is_host)
        { //We received the lose message from the client who is the player2
            ws = WINNER1;
        }
        else
        { //We received the lose message frmo the host who is the player1
            ws = WINNER2;
        }
        mp_end_game();
    }
    else if (data == TIE)
    {
        ws = TIE;
        mp_end_game();
    }
}

void mp_reset_game(int num_rounds)
{
    mp_mouse.x_pos = get_mode_width() / 2;
    mp_mouse.y_pos = get_mode_height() / 2;
    mp_mouse.left_hold = false;
    mp_mouse.left_click = false;
    max_buttons_size = num_rounds;
    buttons = malloc(num_rounds * sizeof(Play_Screens));
    buttons_size = 0;
    player_score = 0;
    guessed = 0;
    is_player_1 = true;
    game_end = false;
    ps = DEFAULT;
    time_left_for_click = -1;
    time_left_for_screen = 0;
    mp_is_VS_round();
    VS_was_pressed = false;
}

void host_mp_game(int num_rounds)
{
    printf("hosting mp_game\n");
    /*
    if (check_buffer() == 0)
    {
        serial_read_byte(&data);
    }
    */
    mp_reset_game(num_rounds);
    is_host = true;
    is_player_1 = true;
    game_started = false;
    uart_has_data_to_send = false;
}

void connect_mp_game()
{
    printf("Connect mp game\n");
    is_player_1 = false;
    is_host = false;
    serial_write_byte(CONNECTED);
    uart_has_data_to_send = false;
    game_started = false;
}

void start_game(uint8_t data, bool empty_thr)
{
    static int received_no_bytes = 0;
    static int num_round_sent_bytes = 0;
    static uint8_t received_byte;
    if (is_host)
    {
        if (data == CONNECTED && num_round_sent_bytes < 2) // Message from client
        {
            if (check_thr_emtpy())
            {
                serial_write_byte(((uint8_t)max_buttons_size) >> (8 * num_round_sent_bytes));
                num_round_sent_bytes++;
            }
        }
        if (data == ACK) // Client after receiving the max_buttons_size
        {
            game_started = true;
        }
    }
    else
    {
        if (empty_thr) //Trying to transmit the data
        {
            if (received_no_bytes == 2)
            {
                serial_write_byte(ACK);
                game_started = true;
            }
        }
        else //Receiving data
        {
            if (received_no_bytes == 0)
            {
                received_byte = data;
            }
            else //received_no_bytes == 1
            {

                int num_of_rounds = (data << 8) | received_byte;
                mp_reset_game(num_of_rounds);
                if (check_thr_emtpy())
                {
                    serial_write_byte(ACK);
                    game_started = true;
                }
            }
            received_no_bytes++;
        }
    }
}

void mp_end_game()
{
    game_end = true;
    free(buttons);
    set_timed_alarm(5);
    //Say that the game has ended to the other person
}

void mp_send_winner()
{
    if (ws == TIE)
    {
        if (check_thr_emtpy())
        {
            serial_write_byte(TIE_REMOTE);
        }
        else
        {
            uart_set_queue(TIE_REMOTE);
        }
    }
    else
    {
        if (check_thr_emtpy())
        {
            serial_write_byte(LOSE);
        }
        else
        {
            uart_set_queue(LOSE);
        }
    }
}

void mp_send_button(button_click_t click)
{
    uint8_t data = GREEN_REMOTE + click;

    if (check_thr_emtpy())
    {
        serial_write_byte(data);
    }
    else
    {
        uart_set_queue(data);
    }
}

void mp_add_button(button_click_t click)
{
    buttons[buttons_size] = click;
    buttons_size++;
}

void mp_change_player()
{
    is_player_1 = !is_player_1;
    guessed = 0;
    //mp_is_VS_round(); //Update to see if the next round is a vs round
    // This is left for the host to computer
    if (is_host)
    {
        mp_is_VS_round();
        if (check_thr_emtpy())
        {
            if (is_vs_round)
            {
                serial_write_byte(IS_VS);
            }
            else
            {
                serial_write_byte(IS_NOT_VS);
            }
        }
        else
        {
            if (is_vs_round)
            {
                uart_set_queue(IS_VS);
            }
            else
            {
                uart_set_queue(IS_NOT_VS);
            }
        }
    }
    else
    {
        if (check_thr_emtpy())
        {
            serial_write_byte(READY);
        }
        else
        {
            uart_set_queue(READY);
        }
    }
}

bool mp_get_red_click()
{
    return ((mp_mouse.x_pos - RED_X) * (mp_mouse.x_pos - RED_X) + (mp_mouse.y_pos - RED_Y) * (mp_mouse.y_pos - RED_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool mp_get_green_click()
{
    return ((mp_mouse.x_pos - GREEN_X) * (mp_mouse.x_pos - GREEN_X) + (mp_mouse.y_pos - GREEN_Y) * (mp_mouse.y_pos - GREEN_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool mp_get_purple_click()
{
    return ((mp_mouse.x_pos - PURPLE_X) * (mp_mouse.x_pos - PURPLE_X) + (mp_mouse.y_pos - PURPLE_Y) * (mp_mouse.y_pos - PURPLE_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool mp_get_blue_click()
{
    return ((mp_mouse.x_pos - BLUE_X) * (mp_mouse.x_pos - BLUE_X) + (mp_mouse.y_pos - BLUE_Y) * (mp_mouse.y_pos - BLUE_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool mp_get_yellow_click()
{
    return ((mp_mouse.x_pos - YELLOW_X) * (mp_mouse.x_pos - YELLOW_X) + (mp_mouse.y_pos - YELLOW_Y) * (mp_mouse.y_pos - YELLOW_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool mp_get_vs_click()
{
    return ((mp_mouse.x_pos - VS_X) * (mp_mouse.x_pos - VS_X) + (mp_mouse.y_pos - VS_Y) * (mp_mouse.y_pos - VS_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

void mp_print_player1()
{
    draw_xpm_x_y(*get_prop_from_char('1'), 0, 0);
}

void mp_print_player2()
{
    draw_xpm_x_y(*get_prop_from_char('2'), 0, 0);
}

void mp_is_VS_round()
{
    is_vs_round = true;
    VS_was_pressed = false;
    return;
    int random = rand() % 10;
    VS_was_pressed = false;
    if (random % 2 == 0)
    {
        is_vs_round = true;
    }
    else
    {
        is_vs_round = false;
    }
}

void mp_get_winner()
{
    if (is_player_1)
    {
        ws = WINNER2;
    }
    else
    {
        ws = WINNER1;
    }
}

void mp_checkNewHiScore()
{
    if (player_score > getLowestHiScore())
    {
        game.mode = HIGHSCORE;
        reset_hiscore(player_score);
    }
    else
    {
        game.mode = LEADERBOARD;
    }
}

void mp_debugging(int i)
{
    draw_xpm_x_y(*get_prop_from_char('0'), 200, 200 + i * 40);
}

void uart_set_queue(uint8_t data)
{
    uart_has_data_to_send = true;
    uart_data = data;
}

void uart_ready()
{
    uart_has_data_to_send = false;
    serial_write_byte(uart_data);
}
