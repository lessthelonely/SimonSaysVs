#include <lcom/lcf.h>

#include "mp_game.h"
#include "consts.h"
#include "screen.h"
#include "keyboard.h"
#include "vc.h"
#include "uart.h"
#include "rtc.h"
#include "num_and_letters.h"
#include "leaderboard.h"
#include "highscore.h"

char s[3];


void mp_draw_game_graphics()
{
    if (!game_end)
    {
        draw_buttons();

        draw_player();

        draw_counter();

        draw_round();

        mp_draw_mouse();

        // debugging
        if (is_vs_round)
        {
            debugging(0);
        }
    }
    else
    {
        mp_draw_winner_graphics();
    }
}

void mp_draw_mouse()
{
    draw_xpm_x_y(*get_mouse(), mouse.x_pos, mouse.y_pos);
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

void mp_game_update_status_mouse(struct packet mouse_packet)
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

    //printf("Mouse x_pos %d\n", mouse.x_pos);
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
    mp_light_button(click);
}

void mp_click_button(button_click_t click)
{
    timer_counter=TIMER_BUTTON_MP;
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
            game_end = true;
            end_game();
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
                        mp_is_VS_round(); //Update to see if the next round is a vs round
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
                    mp_is_VS_round();
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
            game_end = true;
            end_game();
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

void mp_game_update_status_timer()
{
    mouse.left_click = false; //Because after the click, it is no longer doing the action of clicking
    timer_counter--;
    if(timer_counter==0){
      ps = DEFAULT;
       }
    //HOwever, it can still be holding because we didn't get any more mouse interrupts
    //button_click_t click;
}

void mp_game_update_status_rtc()
{

    debugging(2);
    //Player logic
    if (time_left_for_click == 0 && !VS_was_pressed)
    {
        //  PLAYER THAT IS PLAYING LOSES
        get_winner();
    }
    else if (time_left_for_click == 0 && is_vs_round && VS_was_pressed)
    {
        is_player_1 = !is_player_1;
        time_left_for_click = INTERVAL;
        mp_is_VS_round();
    }
    else if (time_left_for_click > 0)
    {
        time_left_for_click--;
    }
    else
    {
        // time_lefet_for_click is -1
        //We have infinite time
    }
}

void mp_game_update_status_uart(uint8_t data)
{
    printf("Got byte %x", data);
    if (!game_started)
    {
        start_game(data);
        return;
    }
}

void mp_reset_game(int num_rounds)
{
    mouse.x_pos = get_mode_width() / 2;
    mouse.y_pos = get_mode_height() / 2;
    mouse.left_hold = false;
    mouse.left_click = false;
    max_buttons_size = num_rounds;
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


void mp_is_VS_round()
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

void mp_get_winner()
{
    if (is_player_1)
    {
        ws = WINNER2;
        game_end = true;
    }
    else
    {
        ws = WINNER1;
        game_end = true;
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

