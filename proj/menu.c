#include <lcom/lcf.h>
#include "menu.h"
#include "simon.h"
#include "number_of_rounds.h"
#include "highscore.h"
#include "screen.h"

void draw_menu_graphics()
{
    //checks which position we are and draws the screen
    switch (position)
    {
    case SELECT_ONEPC:
        //printf("Play\n");
        draw_xpm(game.s[19]);
        break;
    case SELECT_TWOPC:
        draw_xpm(game.s[20]);
        break;
    case SELECT_HOW_TO_PLAY:
        draw_xpm(game.s[21]);
        break;
    case SELECT_LEADERBOARD:
        draw_xpm(game.s[22]);
        break;
    case SELECT_EXIT:
        draw_xpm(game.s[23]);

        //after x time: a)event_handler()
        //  b) bye bye game
        break;
    }
}

void menu_update_status(struct key_press key)
{
    char key_char = key.key;
    bool down = key.down;
    if (down)
    {
        return;
    }
    if (key_char == UP_ARROW || key_char == 'w')
    {
        position = (position + 4) % MENU_NUM_SELECTIONS;
    }
    else if (key_char == DOWN_ARROW || key_char == 's')
    {
        printf("DOWN\n");
        position = (position + 1) % MENU_NUM_SELECTIONS;
    }
    else if (key_char == 10)
    {
        if (position == SELECT_ONEPC)
        {
            game.mode = NUM_ROUNDS;
            reset_number_of_rounds(0);
        }
        else if (position == SELECT_TWOPC)
        {
            game.mode = HOST;
        }
        else if (position == SELECT_HOW_TO_PLAY)
        {
            game.mode = HOW_TO_PLAY;
        }
        else if (position == SELECT_LEADERBOARD)
        {
            game.mode = LEADERBOARD;
        }
        else if (position == SELECT_EXIT)
        {
            game.mode = EXIT;
        }
        position = SELECT_ONEPC; //Resets to the first state when leaving the menu
    }
}
