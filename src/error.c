/*
	mips2c_errors
*/
#include "headers/mips2c.h"
#include <stdio.h>
#include <stdarg.h>

// may want to print this to stderr no matter what? how would we handle this
// in ncurses mode? some errors might be salvagable and we'll want to bounce back from
// them?
void error(const char * errmsg, ... )
{
	PRINT(ANSI_COLOR_RED "\nERROR" ANSI_COLOR_RESET ": ");
	va_list args;
	va_start (args, errmsg);
	if (!check_flag(f_curses))
		vprintf (errmsg, args);
	else if (check_flag(f_curses))
		vwprintw(stdscr, errmsg, args);
	va_end (args);

	exit_info();
	exit(1);

}
