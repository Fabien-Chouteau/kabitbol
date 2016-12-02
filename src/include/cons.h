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

#ifndef CONS_H_
# define CONS_H_

# define CONSOLE 0x80000100

int cons_init();

int print_char(char c);

int print(const char *str);

int print_int(int val);

int print_uint(unsigned int val);

int print_uint_hex(unsigned int val);

int syscall_print(uint32_t ptr, uint32_t len);

#endif /* ! CONS_H_ */
