#include <lcom/lcf.h>

#include "how_to_play.h"
#include "screen.h"
#include "vc.h"
#include "keyboard.h"

void draw_how_to_play_graphics()
{
    draw_xpm(game.s[5]);
}

void how_to_play_update_status(struct key_press key)
{
    char key_char = key.key;
    if (key_char == 8)
    {
        game.mode = MENU;
    }
}
