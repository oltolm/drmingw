if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND CMAKE_C_COMPILER_ID STREQUAL GNU)
    # Build import library separately, to deal with MinGW issues

    add_library (mgwhelp MODULE mgwhelp32exp.def)

    set (MGWHELP_IMPLIB ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libmgwhelp.dll.a)
    add_custom_command (
        OUTPUT ${MGWHELP_IMPLIB}
        COMMAND ${CMAKE_DLLTOOL} --output-lib ${MGWHELP_IMPLIB} --dllname mgwhelp.dll --kill-at --input-def ${CMAKE_CURRENT_SOURCE_DIR}/mgwhelp32imp.def
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mgwhelp32imp.def
    )
    add_custom_target (libmgwhelp_implib DEPENDS mgwhelp ${MGWHELP_IMPLIB})
    add_library (mgwhelp_implib INTERFACE IMPORTED GLOBAL)
    target_link_libraries (mgwhelp_implib INTERFACE ${MGWHELP_IMPLIB})
    install (FILES ${MGWHELP_IMPLIB} DESTINATION lib)
else ()
    add_library (mgwhelp SHARED mgwhelp64.def)
    add_library (mgwhelp_implib ALIAS mgwhelp)
endif ()

target_sources (mgwhelp PRIVATE
    dwarf_find.cpp
    dwarf_pe.cpp
    mgwhelp.cpp
    version.rc
)

if(ENABLE_DBGHELPMS)
# mgwhelp resolves DWARF and PE exports itself and forwards everything else to a
# real dbghelp. Import that under a private name rather than dbghelp.dll: a file
# called dbghelp.dll in an application directory shadows the OS copy for the whole
# process, and system components that bind to dbghelp -- dbgeng.dll among them --
# then fail to load against an older one. Shipping the forward target under its own
# name keeps the OS dbghelp untouched.
set (MGWHELP_DBGHELP_DLL "dbghelpms.dll" CACHE STRING
     "File name of the dbghelp implementation mgwhelp forwards to")

set (MGWHELP_DBGHELP_IMPLIB "${CMAKE_CURRENT_BINARY_DIR}/libmgwdbghelp.a")
add_custom_command (
    OUTPUT ${MGWHELP_DBGHELP_IMPLIB}
    COMMAND ${CMAKE_DLLTOOL}
            --dllname ${MGWHELP_DBGHELP_DLL}
            --input-def ${CMAKE_CURRENT_SOURCE_DIR}/dbghelp.def
            --output-lib ${MGWHELP_DBGHELP_IMPLIB}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/dbghelp.def
    COMMENT "Generating import library for ${MGWHELP_DBGHELP_DLL}"
    VERBATIM
)
add_custom_target (mgwhelp_dbghelp_implib DEPENDS ${MGWHELP_DBGHELP_IMPLIB})
add_dependencies (mgwhelp mgwhelp_dbghelp_implib)
endif()

target_link_libraries (mgwhelp PRIVATE
    common
    dwarf
    libiberty
    "$<IF:$<BOOL:${ENABLE_DBGHELPMS}>,${MGWHELP_DBGHELP_IMPLIB},dbghelp>"
    dwarfstack
)

set_target_properties (mgwhelp PROPERTIES
    PREFIX ""
)


add_custom_command (
    TARGET mgwhelp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -DCMAKE_OBJDUMP=${CMAKE_OBJDUMP} -DTARGET=$<TARGET_FILE:mgwhelp> -P ${PROJECT_SOURCE_DIR}/cmake/CheckDependencies.cmake
    VERBATIM
)


install (TARGETS mgwhelp LIBRARY DESTINATION bin)


# Bundle DbgHelp and SymSrv DLLs
if (WINDBG_FOUND)
    install (
        FILES
            "${WINDBG_DIR}/dbghelp.dll"
            "${WINDBG_DIR}/symsrv.dll"
            "${WINDBG_DIR}/symsrv.yes"
        DESTINATION bin
    )
    if (EXISTS "${WINDBG_DIR}/dbgcore.dll")
        install (
            FILES
                "${WINDBG_DIR}/dbgcore.dll"
            DESTINATION bin
        )
    endif ()
endif ()
