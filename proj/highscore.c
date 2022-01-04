#include <lcom/lcf.h>

#include "highscore.h"
#include "screen.h"
#include "keyboard.h"
#include "num_and_letters.h"
#include "rtc.h"
#include "vc.h"

void draw_highscore_graph()
{
    draw_xpm(game.s[16]);
    draw_text_box(&new_hiscore);
}

void highscore_update_status(struct key_press key)
{
    bool down = key.down;
    char key_char = key.key;

    //Handle numeric input
    if (down)
    {
        return;
    }

    if (key_char == ENTER && strlen(player) != 0)
    {
        memcpy(newWinner.name, player, sizeof(player));
        memcpy(newWinner.date, getDate(), sizeof(newWinner.date));
        updateData();
        game.mode = LEADERBOARD;
    }

    if (is_alpha_numberical(key_char))
    {
        if (add_to_centered_text_box(key_char, &new_hiscore))
        {
            //Was able to add the number
            //We can add to the value of the number of rounds
            strncat(player, &key_char, 1);
        }
    }
    if (key_char == BACKSPACE)
    {
        remove_from_centered_text_box(&new_hiscore);
        int idxToDel = strlen(player) - 1;
        memmove(&player[idxToDel], &player[idxToDel + 1], strlen(player) - idxToDel);
    }
}

void reset_hiscore(int score)
{
    newWinner.int_score = score;
    create_name_box();
}

void create_name_box()
{
    new_hiscore = create_writeable_text_box(400, 330, 0, -1, 10);
}

char *getDate()
{
    char *date = malloc(10 * sizeof(char));
    struct date d = get_date();
    int size = 0;
    int d1, m1, y1;
    char day[3], month[3], year[5];
    d1 = d.day;
    m1 = d.month;
    y1 = d.year;
    y1 += 2000;
    sprintf(day, "%d", d1);
    for (size_t i = 0; i < strlen(day); i++)
    {
        date[size] = day[i];
        size++;
    }
    date[size] = '/';
    size++;

    sprintf(month, "%d", m1);
    for (size_t i = 0; i < strlen(month); i++)
    {
        date[size] = month[i];
        size++;
    }
    date[size] = '/';
    size++;

    sprintf(year, "%d", y1);
    for (size_t i = 0; i < strlen(year); i++)
    {
        date[size] = year[i];
        size++;
    }
    date[size] = '\0';

    return date;
}
