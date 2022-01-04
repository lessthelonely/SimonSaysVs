#include <lcom/lcf.h>

#include "leaderboard.h"
#include "highscore.h"
#include "vc.h"
#include "screen.h"
#include "keyboard.h"
#include "num_and_letters.h"
#include "game.h"
#include "rtc.h"

//Coordinates for name-->(105,137)--->First line
//Coordinates for name-->(105,210)--->Second line
//Coordinates for name-->(107,283)--->Third line
//Coordinates for name-->(107,354)--->Fourth line
//Coordinates for name-->(107,422)--->Fifth line

//Coordinates for round-->(357,137)-->First line
//Coordinates for round-->(357,210)-->Second line
//Coordinates for round-->(357,283)-->Third line
//Coordinates for round-->(357,354)-->Fourth line
//Coordinates for round-->(357,422)-->Fifth line

//Coordinates for date--> (550,137)-->First line
//Coordinates for date--> (550,210)-->Second line
//Coordinates for date--> (550,283)-->Third line
//Coordinates for date--> (550,354)-->Fourth line
//Coordinates for date--> (550,422)-->Fifth line

void loadDummySt()
{
    for (int i = 0; i < 10; i++)
    {
        dummy.name[i] = '0';
        dummy.date[i] = '0';
    }
    dummy.int_score = 0;
}

void draw_leaderboard_graphics()
{

    draw_xpm(game.s[4]);

    for (int i = 0; i < SIZE; i++)
    {
        char name[11];
        char date[11];
        char round[4];
        memcpy(name, leaderboard[i].name, sizeof(leaderboard[i].name));
        memcpy(date, leaderboard[i].date, sizeof(leaderboard[i].date));
        sprintf(round, "%d", leaderboard[i].int_score);

        for (size_t j = 0; j < strlen(name); j++)
        {
            xpm_image_t *img = get_lilprop_from_char(name[j]);
            if (i == 0)
            {
                draw_xpm_x_y(*img, 98 + (j * 22), 143);
            }
            if (i == 1)
            {
                draw_xpm_x_y(*img, 98 + (j * 22), 215);
            }
            if (i == 2)
            {
                draw_xpm_x_y(*img, 98 + (j * 22), 287);
            }
            if (i == 3)
            {

                draw_xpm_x_y(*img, 98 + (j * 22), 358);
            }
            if (i == 4)
            {
                draw_xpm_x_y(*img, 98 + (j * 22), 426);
            }
        }
        for (size_t j = 0; j < strlen(round); j++)
        {
            xpm_image_t *img = get_lilprop_from_char(round[j]);
            if (i == 0)
            {
                draw_xpm_x_y(*img, 357 + (j * 22), 143);
            }
            if (i == 1)
            {
                draw_xpm_x_y(*img, 357 + (j * 22), 215);
            }
            if (i == 2)
            {
                draw_xpm_x_y(*img, 357 + (j * 22), 287);
            }
            if (i == 3)
            {
                draw_xpm_x_y(*img, 357 + (j * 22), 358);
            }
            if (i == 4)
            {

                draw_xpm_x_y(*img, 357 + (j * 22), 426);
            }
        }
        for (size_t j = 0; j < strlen(date); j++)
        {
            xpm_image_t *img = get_lilprop_from_char(date[j]);
            if (i == 0)
            {
                draw_xpm_x_y(*img, 550 + (j * 22), 145);
            }
            if (i == 1)
            {

                draw_xpm_x_y(*img, 550 + (j * 22), 215);
            }
            if (i == 2)
            {
                draw_xpm_x_y(*img, 550 + (j * 22), 287);
            }
            if (i == 3)
            {

                draw_xpm_x_y(*img, 550 + (j * 22), 358);
            }
            if (i == 4)
            {
                draw_xpm_x_y(*img, 550 + (j * 22), 426);
            }
        }
    }
}

void leaderboard_update_status(struct key_press key)
{

    char key_char = key.key;
    if (key_char == BACKSPACE)
    {

        game.mode = MENU;
    }
}

int getNewHiScorePos()
{
    //If it's bigger than the 1st one.
    if (newWinner.int_score > leaderboard[0].int_score)
    {
        return 0;
    }
    //If it's bigger than the 2nd one.
    else if (newWinner.int_score > leaderboard[1].int_score)
    {
        return 1;
    }
    //If it's bigger than the 3rd one.
    else if (newWinner.int_score > leaderboard[2].int_score)
    {
        return 2;
    }
    //If it's bigger than the 4th one.
    else if (newWinner.int_score > leaderboard[3].int_score)
    {
        return 3;
    }
    //If it's bigger than the 5th and last one.
    else if (newWinner.int_score > leaderboard[4].int_score)
    {
        return 4;
    }
    //To avoid the "reaches the end of non-void function" error.
    return 4;
}

void updateData()
{
    char temp_name[11];
    char temp_date[11];
    int score = 0;

    //We get the position where we'll start the changes.
    int i = getNewHiScorePos();
    //Copy it all to temporary structures.
    memcpy(temp_name, leaderboard[i].name, sizeof(leaderboard[i].name));
    memcpy(temp_date, leaderboard[i].date, sizeof(leaderboard[i].date));
    score = leaderboard[i].int_score;
    //Replace the obtained position with the new values.
    memcpy(leaderboard[i].name, newWinner.name, sizeof(newWinner.name));
    memcpy(leaderboard[i].date, newWinner.date, sizeof(newWinner.date));
    leaderboard[i].int_score = newWinner.int_score;
    i++;
    while (i < 5)
    {
        //We use "newWinner" as a temporary structure to pass data between the leaderboard places.
        memcpy(newWinner.name, leaderboard[i].name, sizeof(leaderboard[i].name));
        memcpy(newWinner.date, leaderboard[i].date, sizeof(leaderboard[i].date));
        newWinner.int_score = leaderboard[i].int_score;

        //After that, we replace the current place with its new winner.
        memcpy(leaderboard[i].name, temp_name, sizeof(temp_name));
        memcpy(leaderboard[i].date, temp_date, sizeof(temp_date));
        leaderboard[i].int_score = score;

        //And set things up to repeat it all as many times as needed.
        memcpy(temp_name, newWinner.name, sizeof(newWinner.name));
        memcpy(temp_date, newWinner.date, sizeof(newWinner.date));
        score = newWinner.int_score;
        i++;
    }
}

int getLowestHiScore()
{
    return leaderboard[4].int_score;
}

void saveData()
{
    FILE *lbd = fopen("home/lcom/labs/g06/proj/leaderboard.txt", "w");
    for (int i = 0; i < SIZE; i++)
    {
        //We save the leaderboard data with a specific format.
        fprintf(lbd, "%s\n%d\n%s\n\n", leaderboard[i].name, leaderboard[i].int_score, leaderboard[i].date);
    }
    fclose(lbd);
}

void retrieveData()
{
    loadDummySt();
    char name[11];
    char date[11];

    FILE *lbd = fopen("home/lcom/labs/g06/proj/leaderboard.txt", "r");
    for (int i = 0; i < SIZE; i++)
    {
        //We unpack the format written by saveData and copy it directly to the leaderboard data structure.
        fscanf(lbd, "%s\n", name);
        memcpy(leaderboard[i].name, name, sizeof(name));
        fscanf(lbd, "%d\n", &(leaderboard[i].int_score));
        fscanf(lbd, "%s\n", date);
        memcpy(leaderboard[i].date, date, sizeof(date));
    }
    fclose(lbd);
}
