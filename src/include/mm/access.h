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

#ifndef ACCESS_H_
# define ACCESS_H_

# include "types.h"
# include "vm_manager.h"

# define ACCESS_READ  (1 << 0)
# define ACCESS_WRITE (1 << 1)

bool check_access(as_s *as, vaddr ptr, uint32_t size, uint32_t flags);
uint32_t write_to_as(struct as *as, vaddr dest, char *src, uint32_t size);

#endif	/* ! ACCESS_H_ */
