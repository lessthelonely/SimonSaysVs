# name of the program (Minix service)
PROG=lab4

# source code files to be compiled
SRCS = lab4.c i8254.h timer.c i8042.h mouse.h mouse.c utils.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DLAB4

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>

CPPFLAGS += -D LAB4
