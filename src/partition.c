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

#include "partition.h"
#include "core.h"
#include "utils.h"
#include "elf.h"
#include "debug.h"
#include "context.h"
#include "scheduler.h"
#include "thread_state.h"

struct partition g_partitions[PARTITION_NBR];

const unsigned char magic[4] = { 0x7f, 0x45, 0x4c, 0x46 };

int partition_init(void)
{
	Elf_Ehdr *elf_hdr = (Elf_Ehdr *) _binary_initpartition_o_start;
	int i = 0;
	int status = 0;
	struct as *as = NULL;

	if ((as = new_as()) == NULL)
		return -ENOMEM;

	g_partitions[i].as = as;
	g_partitions[i].context_nbr = as->ctx_nbr;
	g_partitions[i].num_threads = 0;

	pdebug(DS_PARTITION, DL_DEBUG,  "elf_hdr at 0x%x\r\n", elf_hdr);

	int k = 0;
	for (k = 0; k < 4; k++)
		if (magic[k] != elf_hdr->e_ident[k])
		{
			pdebug(DS_PARTITION, DL_ERROR, "Elf loader: bad magic\r\n");
			return ERROR;
		}

	if (elf_hdr->e_machine != EM_SPARC)
	{
		pdebug(DS_PARTITION, DL_ERROR, "Elf loader: bad machine type\r\n");
		return ERROR;
	}

	if (elf_hdr->e_phnum == 0)
	{
		pdebug(DS_PARTITION, DL_ERROR, "Elf loader: No program header\r\n");
		return ERROR;
	}

	if (elf_hdr->e_entry > PARTITION_SIZE)
	{
		pdebug(DS_PARTITION, DL_ERROR, "Elf loader: entry address: 0x%x\r\n",
					 elf_hdr->e_entry);
		return ERROR;
	}

	if ((status = partition_add_thread(&(g_partitions[i]), elf_hdr->e_entry, 0)) < 0)
	{
		pdebug(DS_PARTITION, DL_ERROR, "Elf loader: add thread failed: 0x%x\r\n",
					 status);
		return status;
	}

	if ((status = thread_set_state(&(g_partitions[i]), status,
																 THREAD_STATE_RUN)) < 0)
		return status;

	/* select the first thread */
	schedule();
	g_curr_th = g_next_th;

	if (elf_hdr->e_phnum > 1)
		pdebug(DS_PARTITION, DL_WARNING, "Elf loader: more than one program\r\n");

	const Elf_Phdr *phdr = (Elf_Phdr *) ((char *) elf_hdr + elf_hdr->e_phoff);

	if (phdr->p_type == PT_LOAD)
	{
		const unsigned char *src = (unsigned char *) elf_hdr + phdr->p_offset;

		if (!is_in_code_space(phdr->p_vaddr, phdr->p_memsz))
		{
			pdebug(DS_PARTITION, DL_ERROR,
						 "Elf loader: bad program size or address\r\n");
			return ERROR;
		}

		as->code->start = phdr->p_vaddr / PAGE_SIZE;
		vm_block_enlarge(as->code, (phdr->p_memsz / PAGE_SIZE) + 1);

		context_switch_to(as->ctx_nbr);

		unsigned char *dst = (unsigned char *) phdr->p_vaddr;

		unsigned int j = 0;

		for (j = 0; j < phdr->p_filesz; j++)
		{
			dst[j] = src[j];
		}
		for (; j < phdr->p_memsz; j++)
		{
			dst[j] = 0x0;
		}

		pdebug(DS_PARTITION, DL_INFO,
					 "Program of partition #%u loaded at vaddr:0x%x\r\n", i, dst);
	}
	else
	{
		pdebug(DS_PARTITION, DL_ERROR,
					 "Elf loader: Program not loaded p_type=0x%x\r\n", phdr->p_type);
		return ERROR;
	}

	context_switch_to(g_curr_th->part->context_nbr); /* go to current context */
	return NOERROR;
}
