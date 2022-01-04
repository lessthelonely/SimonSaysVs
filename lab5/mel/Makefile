# name of the program (Minix service)
PROG=lab5

# source code files to be compiled
SRCS = lab5.c vc.c vc.h kbc.c kbc.h i8042.h i8254.h timer.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB5

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

CPPFLAGS += -D LAB5
