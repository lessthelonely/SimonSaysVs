# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = proj.c simon.c screen.c menu.c leaderboard.c how_to_play.c highscore.c number_of_rounds.c host.c game.c mp_game.c mouse.c keyboard.c timer.c vc.c num_and_letters.c uart.c rtc.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CPPFLAGS += -pedantic  -D __LCOM_OPTIMIZED__

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

