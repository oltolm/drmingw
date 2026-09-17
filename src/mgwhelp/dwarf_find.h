/*
 * Copyright 2013-2015 Jose Fonseca
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#pragma once

#include "dwarfstack.h"
#include <string>
#include <vector>

#include <stdbool.h>

#include <dwarf.h>
#include <libdwarf.h>


struct dwarf_frame_info {
    std::string functionname;
    Dwarf_Addr function_addr = 0;
    unsigned int offset_addr = 0;

    std::wstring filename;
    unsigned int line = 0;
    Dwarf_Addr line_addr = 0;
    unsigned int line_offset_addr = 0;
};

/*
 * Find all frames for an address: the innermost inlined function first, and
 * the function that physically contains the address last.  Callers of inlined
 * functions report the line of the call site.
 *
 * Returns an empty vector if the address has no DWARF information.
 */
std::vector<dwarf_frame_info>
dwarf_find_frames(dwst_module *module, wchar_t *name, Dwarf_Addr image_base, Dwarf_Addr addr);
