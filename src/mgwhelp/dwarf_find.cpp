/*
 * Copyright 2013-2015 Jose Fonseca
 * Copyright 2009 Kai Wang
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 * Based on elftoolchain-0.6.1/addr2line/addr2line.c
 */


#include "dwarf_find.h"

#include <cassert>
#include <stdlib.h>

#include "libdwarf.h"
#include <dwarf_arange.h>
#include <dwarfstack.h>

struct find_frames_context {
    Dwarf_Addr addr;
    Dwarf_Addr function_addr = 0;
    Dwarf_Addr line_addr = 0;
    std::vector<dwarf_frame_info> frames;
};

// dwarfstack reports DWST_FUNC_ADDR and DWST_LINE_ADDR (when known) before the
// file/line of each frame, innermost inlined function first.
static void
find_frames_cbW(uint64_t addr,
                const wchar_t *filename,
                int lineno,
                const char *funcname,
                void *context,
                int columnno)
{
    auto ctx = (struct find_frames_context *)context;

    switch (lineno) {
    case DWST_BASE_ADDR:
    case DWST_NOT_FOUND:
    case DWST_NO_DBG_SYM:
    case DWST_NO_SRC_FILE:
        break;

    case DWST_FUNC_ADDR:
        ctx->function_addr = addr;
        break;

    case DWST_LINE_ADDR:
        ctx->line_addr = addr;
        break;

    default: {
        dwarf_frame_info info;
        info.functionname = funcname ? funcname : "";
        info.function_addr = ctx->function_addr;
        if (info.function_addr) {
            info.offset_addr = ctx->addr - info.function_addr;
        }
        info.filename = filename ? filename : L"";
        info.line = lineno;
        info.line_addr = ctx->line_addr;
        if (info.line_addr) {
            info.line_offset_addr = ctx->addr - info.line_addr;
        }
        ctx->frames.push_back(std::move(info));

        ctx->function_addr = 0;
        ctx->line_addr = 0;
        break;
    }
    }
}

std::vector<dwarf_frame_info>
dwarf_find_frames(dwst_module *module, wchar_t *name, Dwarf_Addr image_base, Dwarf_Addr addr)
{
    struct find_frames_context ctx;
    ctx.addr = addr;
    dwstOfModuleW(module, name, image_base, &addr, 1, &find_frames_cbW, &ctx);
    return std::move(ctx.frames);
}
