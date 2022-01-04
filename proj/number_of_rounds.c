#include <lcom/lcf.h>

#include "number_of_rounds.h"
#include "vc.h"
#include "num_and_letters.h"
#include "keyboard.h"
#include "game.h"
#include "mp_game.h"

void draw_num_of_rounds_graphics()
{
    draw_xpm(game.s[17]);
    draw_text_box(&num_rounds);
}

void number_of_rounds_update_status(struct key_press key)
{
    bool down = key.down;
    char key_char = key.key;

    //Handle numeric input
    if (down)
    {
        return;
    }

    if (key_char == 10 && num_rounds_value != 0)
    {

        if (!is_mp)
        {
            game.mode = GAME;
            reset_game(num_rounds_value);
        }
        if (is_mp)
        {
            game.mode = MP_GAME;
            host_mp_game(num_rounds_value); //1 as for host
        }
    }

    if (is_numerical(key_char))
    {
        if (add_to_centered_text_box(key_char, &num_rounds))
        { //Was able to add the number
            //We can add to the value of the number of rounds
            num_rounds_value = num_rounds_value * 10 + (key_char - '0');
        }
    }
    if (key_char == BACKSPACE)
    {
        remove_from_centered_text_box(&num_rounds);
        num_rounds_value /= 10;
    }
}

void reset_number_of_rounds(bool mp)
{
    is_mp = mp;
    create_num_box();
}

void create_num_box()
{
    num_rounds = create_writeable_text_box(400, 360, 0, -1, 3);
    num_rounds_value = 0;
}
