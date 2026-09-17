/*
 * Copyright 2009-2012 Jose Fonseca
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

#include <windows.h>
#include <dbghelp.h>

EXTERN_C BOOL WINAPI
MgwSymInitialize(HANDLE hProcess, PCSTR UserSearchPath, BOOL fInvadeProcess);

EXTERN_C BOOL WINAPI
MgwSymCleanup(HANDLE hProcess);

EXTERN_C DWORD WINAPI
MgwSymSetOptions(DWORD SymOptions);

EXTERN_C DWORD64 WINAPI
MgwSymLoadModuleEx(HANDLE hProcess,
                   HANDLE hFile,
                   PCSTR ImageName,
                   PCSTR ModuleName,
                   DWORD64 BaseOfDll,
                   DWORD DllSize,
                   PMODLOAD_DATA Data,
                   DWORD Flags);

EXTERN_C DWORD64 WINAPI
MgwSymLoadModuleExW(HANDLE hProcess,
                    HANDLE hFile,
                    PCWSTR ImageName,
                    PCWSTR ModuleName,
                    DWORD64 BaseOfDll,
                    DWORD DllSize,
                    PMODLOAD_DATA Data,
                    DWORD Flags);

EXTERN_C BOOL WINAPI
MgwSymFromAddr(HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PSYMBOL_INFO Symbol);

EXTERN_C BOOL WINAPI
MgwSymGetLineFromAddr64(HANDLE hProcess,
                        DWORD64 dwAddr,
                        PDWORD pdwDisplacement,
                        PIMAGEHLP_LINE64 Line);

EXTERN_C DWORD WINAPI
MgwUnDecorateSymbolName(PCSTR DecoratedName,
                        PSTR UnDecoratedName,
                        DWORD UndecoratedLength,
                        DWORD Flags);

EXTERN_C BOOL WINAPI
MgwSymInitializeW(HANDLE hProcess, PCWSTR UserSearchPath, BOOL fInvadeProcess);

EXTERN_C BOOL WINAPI
MgwSymFromAddrW(HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PSYMBOL_INFOW SymbolW);

EXTERN_C BOOL WINAPI
MgwSymGetLineFromAddrW64(HANDLE hProcess,
                         DWORD64 dwAddr,
                         PDWORD pdwDisplacement,
                         PIMAGEHLP_LINEW64 LineW);

EXTERN_C DWORD WINAPI
MgwUnDecorateSymbolNameW(PCWSTR DecoratedName,
                         PWSTR UnDecoratedName,
                         DWORD UndecoratedLength,
                         DWORD Flags);

EXTERN_C DWORD WINAPI
MgwSymAddrIncludeInlineTrace(HANDLE hProcess, DWORD64 Address);

EXTERN_C BOOL WINAPI
MgwSymQueryInlineTrace(HANDLE hProcess,
                       DWORD64 StartAddress,
                       DWORD StartContext,
                       DWORD64 StartRetAddress,
                       DWORD64 CurAddress,
                       LPDWORD CurContext,
                       LPDWORD CurFrameIndex);

EXTERN_C BOOL WINAPI
MgwSymFromInlineContext(HANDLE hProcess,
                        DWORD64 Address,
                        ULONG InlineContext,
                        PDWORD64 Displacement,
                        PSYMBOL_INFO Symbol);

EXTERN_C BOOL WINAPI
MgwSymFromInlineContextW(HANDLE hProcess,
                         DWORD64 Address,
                         ULONG InlineContext,
                         PDWORD64 Displacement,
                         PSYMBOL_INFOW Symbol);

EXTERN_C BOOL WINAPI
MgwSymGetLineFromInlineContext(HANDLE hProcess,
                               DWORD64 dwAddr,
                               ULONG InlineContext,
                               DWORD64 qwModuleBaseAddress,
                               PDWORD pdwDisplacement,
                               PIMAGEHLP_LINE64 Line);

EXTERN_C BOOL WINAPI
MgwSymGetLineFromInlineContextW(HANDLE hProcess,
                                DWORD64 dwAddr,
                                ULONG InlineContext,
                                DWORD64 qwModuleBaseAddress,
                                PDWORD pdwDisplacement,
                                PIMAGEHLP_LINEW64 Line);


// dbghelp inline frame functions, which mingw-w64's dbghelp.h doesn't declare.

EXTERN_C DWORD WINAPI
SymAddrIncludeInlineTrace(HANDLE hProcess, DWORD64 Address);

EXTERN_C BOOL WINAPI
SymQueryInlineTrace(HANDLE hProcess,
                    DWORD64 StartAddress,
                    DWORD StartContext,
                    DWORD64 StartRetAddress,
                    DWORD64 CurAddress,
                    LPDWORD CurContext,
                    LPDWORD CurFrameIndex);

EXTERN_C BOOL WINAPI
SymFromInlineContextW(HANDLE hProcess,
                      DWORD64 Address,
                      ULONG InlineContext,
                      PDWORD64 Displacement,
                      PSYMBOL_INFOW Symbol);

EXTERN_C BOOL WINAPI
SymGetLineFromInlineContextW(HANDLE hProcess,
                             DWORD64 dwAddr,
                             ULONG InlineContext,
                             DWORD64 qwModuleBaseAddress,
                             PDWORD pdwDisplacement,
                             PIMAGEHLP_LINEW64 Line);
