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

#ifndef ELF_H_
# define ELF_H_

# define EI_NIDENT 16
# define EM_SPARC  2
# define PT_LOAD   1

# define PF_R 0x4
# define PF_W 0x2
# define PF_X 0x1

# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr

typedef unsigned short Elf32_Half;
typedef unsigned int Elf32_Addr;
typedef unsigned int Elf32_Off;
typedef unsigned int Elf32_Word;

/* ELF File Header */
typedef struct
{
	unsigned char e_ident[EI_NIDENT];	/* Magic number and other info */
	Elf32_Half e_type;		/* Object file type */
	Elf32_Half e_machine;	/* Architecture */
	Elf32_Word e_version;	/* Object file version */
	Elf32_Addr e_entry;		/* Entry point virtual address */
	Elf32_Off e_phoff;		/* Program header table file offset */
	Elf32_Off e_shoff;		/* Section header table file offset */
	Elf32_Word e_flags;		/* Processor-specific flags */
	Elf32_Half e_ehsize;	/* ELF header size in bytes */
	Elf32_Half e_phentsize;	/* Program header table entry size */
	Elf32_Half e_phnum;		/* Program header table entry count */
	Elf32_Half e_shentsize;	/* Section header table entry size */
	Elf32_Half e_shnum;		/* Section header table entry count */
	Elf32_Half e_shstrndx;	/* Section header string table index */
} Elf32_Ehdr;

/* Program segment header.  */
typedef struct
{
	Elf32_Word p_type;		/* Segment type */
	Elf32_Off p_offset;		/* Segment file offset */
	Elf32_Addr p_vaddr;		/* Segment virtual address */
	Elf32_Addr p_paddr;		/* Segment physical address */
	Elf32_Word p_filesz;	/* Segment size in file */
	Elf32_Word p_memsz;		/* Segment size in memory */
	Elf32_Word p_flags;		/* Segment flags */
	Elf32_Word p_align;		/* Segment alignment */
} Elf32_Phdr;

/* Section header.  */
typedef struct
{
	Elf32_Word sh_name;		/* Section name (string tbl index) */
	Elf32_Word sh_type;		/* Section type */
	Elf32_Word sh_flags;	/* Section flags */
	Elf32_Addr sh_addr;		/* Section virtual addr at execution */
	Elf32_Off sh_offset;	/* Section file offset */
	Elf32_Word sh_size;		/* Section size in bytes */
	Elf32_Word sh_link;		/* Link to another section */
	Elf32_Word sh_info;		/* Additional section information */
	Elf32_Word sh_addralign;	/* Section alignment */
	Elf32_Word sh_entsize;	/* Entry size if section holds table */
} Elf32_Shdr;

#endif /* ! ELF_H_ */
