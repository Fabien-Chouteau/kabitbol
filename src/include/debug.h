/*
 *  Copyright (C) 2009-2011 Fabien Chouteau
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DEBUG_H_
# define DEBUG_H_
# include "config.h"
# include "utils.h"

/* Debug levels */

# define DL_INFO     0
# define DL_DEBUG    1
# define DL_WARNING  2
# define DL_ERROR    3
# define DL_CRITICAL 4
# define DL_NO_DEBUG 5

/* Debug sections */
# define DS_CONS      (1 << 0)
# define DS_AMBA      (1 << 1)
# define DS_TIMER     (1 << 2)
# define DS_IRQ       (1 << 3)
# define DS_CORE      (1 << 4)
# define DS_TRAPS     (1 << 5)
# define DS_MM        (1 << 6)
# define DS_SVGA      (1 << 7)
# define DS_PARTITION (1 << 8)
# define DS_SYSCALL   (1 << 9)
# define DS_SCHED     (1 << 10)

# define DEBUG_ENABLED

extern const char *debug_level_str[];

/* print debug function */
# ifdef DEBUG_ENABLED
#  define pdebug(section, lvl, fmt, ...)                                \
  do {                                                                  \
    if (((section & DEBUG_SECTIONS) && (lvl >= DEBUG_LEVELS)) || (lvl == DL_CRITICAL)) \
      printf("%s" fmt, debug_level_str[lvl], ##__VA_ARGS__);           \
    if (lvl == DL_CRITICAL)                                             \
      while (1)                                                         \
        continue;                                                       \
  } while (0)
# else
#  define pdebug(section, lvl, fmt, ...)												\
  do {																													\
  } while (0)
# endif

#endif /* ! DEBUG_H_ */
