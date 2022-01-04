#include <lcom/lcf.h>

#include "game.h"
#include "consts.h"
#include "screen.h"
#include "keyboard.h"
#include "vc.h"
#include "num_and_letters.h"
#include "leaderboard.h"
#include "highscore.h"
#include "rtc.h"


void draw_game_graphics()
{
    if (!game_end)
    {
        draw_buttons();

        draw_player();

        draw_counter();

        draw_round();

        draw_mouse();
    }
    else
    {
        draw_winner_graphics();
    }
}

void draw_winner_graphics()
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

void draw_buttons()
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

void draw_player()
{
    if (is_player_1)
    {
        print_player1();
    }
    else
    {
        print_player2();
    }
}

void draw_counter()
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

void draw_round()
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

void game_update_status_mouse(struct packet mouse_packet)
{

    if (game_end)
    {
        //We have ended the game, since no need to handle the mouse packet
        return;
    }
    button_click_t click;
    if (!mouse_packet.x_ov)
    {
        if (mouse.x_pos < -mouse_packet.delta_x)
        {
            mouse.x_pos = 1;
        }
        else if (mouse.x_pos + mouse_packet.delta_x >= get_mode_width() - MOUSE_WIDTH)
        {
            mouse.x_pos = get_mode_width() - MOUSE_WIDTH;
        }
        else
        {
            mouse.x_pos += mouse_packet.delta_x;
        }
    }
    if (!mouse_packet.y_ov)
    {
        if (mouse.y_pos < mouse_packet.delta_y)
        {
            mouse.y_pos = 1;
        }
        else if (mouse.y_pos - mouse_packet.delta_y >= get_mode_height() - MOUSE_HEIGHT)
        {
            mouse.y_pos = get_mode_height() - MOUSE_HEIGHT;
        }
        else
        {
            mouse.y_pos -= mouse_packet.delta_y;
        }
    }

    if (mouse_packet.lb)
    {
        if (mouse.left_hold)
        {
            mouse.left_click = false;
        }
        else
        {
            mouse.left_click = true;
            mouse.left_hold = true;
        }
    }
    else
    {
        mouse.left_hold = false;
        mouse.left_click = false;
    }

    if (mouse.left_click)
    {
        click = get_button_click();
    }
    else
    {
        click = NO_BUTTON;
    }
    //get if I clicked some button

    click_button(click);
    light_button(click);
}

button_click_t get_button_click()
{

    if (get_red_click())
    {
        return RED_B;
    }
    else if (get_green_click())
    {
        return GREEN_B;
    }
    else if (get_purple_click())
    {
        return PURPLE_B;
    }
    else if (get_blue_click())
    {
        return BLUE_B;
    }
    else if (get_yellow_click())
    {
        return YELLOW_B;
    }
    else if (get_vs_click())
    {
        return VS_B;
    }
    else
    {
        return NO_BUTTON;
    }
}

void click_button(button_click_t click)
{
    if (click == NO_BUTTON)
    {
        //Didn't click any button
        mouse.left_click = false; // DEBUG;
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
            end_game();
            //TODO
        }
        else
        {
            if (is_vs_round)
            {
                if (click == VS_B && !VS_was_pressed)
                {
                    VS_was_pressed = true;

                    buttons[buttons_size] = click;
                    buttons_size++;
                    time_left_for_click = VS_INTERVAL;
                }
                else
                {
                    buttons[buttons_size] = click;
                    buttons_size++;
                    guessed = 0;
                    if (!VS_was_pressed)
                    {
                        is_player_1 = !is_player_1;
                        is_VS_round(); //Update to see if the next round is a vs round
                        time_left_for_click = INTERVAL;
                    }
                }
            }
            else
            {
                buttons[buttons_size] = click;
                buttons_size++;
                guessed = 0;
                if (!VS_was_pressed)
                {
                    is_player_1 = !is_player_1;
                    time_left_for_click = INTERVAL;
                    is_VS_round();
                }
            }
        }
    }
    else
    {
        //We are checking if it clicked the right button
        if (buttons[guessed] == click)
        {
            //continue
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
            get_winner();
            end_game();
        }
    }
}

void light_button(button_click_t click)
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
        if (VS_was_pressed && time_left_for_click == VS_INTERVAL)
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

void game_update_status_key(struct key_press key)
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
                checkNewHiScore(player_score);
            }
        }
    }
}

void draw_mouse()
{
    draw_xpm_x_y(*get_mouse(), mouse.x_pos, mouse.y_pos);
}

void game_update_status_timer()
{
    mouse.left_click = false;
    // After a frame, it's no longer holding the button
}

void game_update_status_rtc()
{
    //Player logic
    if (time_left_for_click == 0 && !VS_was_pressed)
    {
        // Player that is playing loses
        get_winner();
        end_game();
    }
    else if (time_left_for_click == 0 && is_vs_round && VS_was_pressed)
    {
        is_player_1 = !is_player_1;
        time_left_for_click = INTERVAL;
        is_VS_round();
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

    if (game_end)
    {
        set_seconds_alarm(); //Reset the previous alarms

        if (ws == TIE)
        {
            game.mode = LEADERBOARD;
        }
        else
        {
            checkNewHiScore(player_score);
        }
    }
}

void reset_game(int num_rounds)
{
    mouse.x_pos = get_mode_width() / 2;
    mouse.y_pos = get_mode_height() / 2;
    mouse.left_hold = false;
    mouse.left_click = false;
    max_buttons_size = num_rounds;
    printf("IN GAME.C %X", max_buttons_size);
    buttons = malloc(num_rounds * sizeof(Play_Screens));
    buttons_size = 0;
    player_score = 0;
    guessed = 0;
    is_player_1 = true;
    game_end = false;
    ps = DEFAULT;
    time_left_for_click = -1;
    is_VS_round();
    VS_was_pressed = false;
}

void end_game()
{
    game_end = true;
    free(buttons);
    set_timed_alarm(5);
}

bool get_red_click()
{
    return ((mouse.x_pos - RED_X) * (mouse.x_pos - RED_X) + (mouse.y_pos - RED_Y) * (mouse.y_pos - RED_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool get_green_click()
{
    return ((mouse.x_pos - GREEN_X) * (mouse.x_pos - GREEN_X) + (mouse.y_pos - GREEN_Y) * (mouse.y_pos - GREEN_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool get_purple_click()
{
    return ((mouse.x_pos - PURPLE_X) * (mouse.x_pos - PURPLE_X) + (mouse.y_pos - PURPLE_Y) * (mouse.y_pos - PURPLE_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool get_blue_click()
{
    return ((mouse.x_pos - BLUE_X) * (mouse.x_pos - BLUE_X) + (mouse.y_pos - BLUE_Y) * (mouse.y_pos - BLUE_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool get_yellow_click()
{
    return ((mouse.x_pos - YELLOW_X) * (mouse.x_pos - YELLOW_X) + (mouse.y_pos - YELLOW_Y) * (mouse.y_pos - YELLOW_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

bool get_vs_click()
{
    return ((mouse.x_pos - VS_X) * (mouse.x_pos - VS_X) + (mouse.y_pos - VS_Y) * (mouse.y_pos - VS_Y)) <= BUTTON_WIDTH * BUTTON_WIDTH;
}

void print_player1()
{
    draw_xpm_x_y(*get_prop_from_char('1'), 0, 0);
}

void print_player2()
{
    draw_xpm_x_y(*get_prop_from_char('2'), 0, 0);
}

void is_VS_round()
{
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

void get_winner()
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

int get_max_buttons_size(){
    return max_buttons_size;
}

void checkNewHiScore()
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

void debugging(int i)
{
    draw_xpm_x_y(*get_prop_from_char('0'), 200, 200 + i * 40);
}
