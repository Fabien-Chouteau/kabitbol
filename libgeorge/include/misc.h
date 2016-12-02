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

#ifndef MISC_H_
# define MISC_H_

# define NULL ((void *) 0)

# define FALSE 0
# define TRUE  ( ! FALSE)

# define ASSERT(test) {\
	} while (0)

# define min_t(type, a, b) ((type)(a) < (type)(b) ? (type)(a) : (type)(b))
# define max_t(type, a, b) ((type)(a) > (type)(b) ? (type)(a) : (type)(b))
# define abs(a) ((a) < 0 ? -(a) : (a))

#endif	/* ! MISC_H_ */
