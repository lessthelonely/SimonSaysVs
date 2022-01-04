#include <lcom/lcf.h>
#include "host.h"
#include "simon.h"
#include "mp_game.h"
#include "number_of_rounds.h"
#include "screen.h"

void draw_host_graphics()
{
    //checks which host_position we are and draws the screen
    switch (host_position)
    {
    case HOST_SELECT_HOST:
        //printf("Play\n");
        draw_xpm(game.s[24]); // host select host screen
        break;
    case HOST_SELECT_CONNECT:
        draw_xpm(game.s[25]); // host select connect
        break;
    }
}

void host_update_status(struct key_press key)
{
    char key_char = key.key;
    bool down = key.down;
    printf("%x, %x \n", key_char, down);
    if (down)
    {
        return;
    }
    if (key_char == UP_ARROW || key_char == 'w')
    {
        host_position = (host_position - 1) % HOST_NUM_SELECTIONS;
    }
    else if (key_char == DOWN_ARROW || key_char == 's')
    {
        host_position = (host_position + 1) % HOST_NUM_SELECTIONS;
    }
    else if (key_char == 10)
    {
        if (host_position == HOST_SELECT_HOST)
        {
            game.mode = NUM_ROUNDS;
            reset_number_of_rounds(1);
        }
        else if (host_position == HOST_SELECT_CONNECT)
        {
            game.mode = MP_GAME;
            connect_mp_game();
        }
        host_position = HOST_SELECT_HOST; //Resets to the first state when leaving the menu
    }
}
