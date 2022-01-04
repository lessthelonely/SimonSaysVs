#include <lcom/lcf.h>
#include "screen.h"
#include "vc.h"

Screen loadscreens()
{
    Screen game;
    //Load all xpm images
    xpm_load(play_menu_xpm,XPM_5_6_5,&game.s[0]);
    xpm_load(how_to_play_xpm,XPM_5_6_5,&game.s[1]);
    xpm_load(leaderboard_menu_xpm,XPM_5_6_5,&game.s[2]);
    xpm_load(exit_menu_xpm,XPM_5_6_5,&game.s[3]);
    xpm_load(leaderboard_xpm,XPM_5_6_5,&game.s[4]);
    xpm_load(instructions_xpm,XPM_5_6_5,&game.s[5]);
    xpm_load(winner1_xpm,XPM_5_6_5,&game.s[6]);
    xpm_load(winner2_xpm,XPM_5_6_5,&game.s[7]);
    xpm_load(VS_activated_xpm,XPM_5_6_5,&game.s[8]);
    xpm_load(default_xpm,XPM_5_6_5,&game.s[9]);
    xpm_load(green_pressed_xpm,XPM_5_6_5,&game.s[10]);
    xpm_load(yellow_pressed_xpm,XPM_5_6_5,&game.s[11]);
    xpm_load(purple_pressed_xpm,XPM_5_6_5,&game.s[12]);
    xpm_load(blue_pressed_xpm,XPM_5_6_5,&game.s[13]);
    xpm_load(red_pressed_xpm,XPM_5_6_5,&game.s[14]);
    xpm_load(white_pressed_xpm,XPM_5_6_5,&game.s[15]);
    xpm_load(hiscore_xpm,XPM_5_6_5,&game.s[16]);
    xpm_load(number_of_rounds_xpm,XPM_5_6_5,&game.s[17]);
    xpm_load(tie_xpm,XPM_5_6_5,&game.s[18]);
    //Serial port graphs:
    xpm_load(OnePC_xpm,XPM_5_6_5,&game.s[19]);
    xpm_load(TwoPC_xpm,XPM_5_6_5,&game.s[20]);
    xpm_load(how_to_play_serial_port_xpm,XPM_5_6_5,&game.s[21]);
    xpm_load(leaderboard_serial_port_xpm,XPM_5_6_5,&game.s[22]);
    xpm_load(exit_serial_port_xpm,XPM_5_6_5,&game.s[23]);
    xpm_load(host_xpm,XPM_5_6_5,&game.s[24]);
    xpm_load(connect_xpm,XPM_5_6_5,&game.s[25]);
    
    game.mode=MENU; //games opens with arrow in position play

    return game;
}
