include_directories ( ${GBEMU_ARCH_INCLUDE} )
include_directories ( ${GBEMU_CORE_INCLUDE} )

set ( HEADERS
)

set ( SOURCE
  main.c
)

add_executable ( GBEmuTest ${SOURCE} ${HEADERS} )

target_link_libraries ( GBEmuTest GBEmuCore )
