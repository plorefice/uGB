include_directories ( ${GBEMU_ARCH_INCLUDE} )
include_directories ( ${GBEMU_CORE_INCLUDE} )

set ( HEADERS
  ${GBEMU_CORE_INCLUDE}/cartridge.h
  ${GBEMU_CORE_INCLUDE}/graphics.h
  ${GBEMU_CORE_INCLUDE}/memory.h
  ${GBEMU_CORE_INCLUDE}/opcodes.h
  ${GBEMU_CORE_INCLUDE}/processor.h
)

set ( SOURCE
  cartridge.c
  graphics.c
  memory.c
  opcodes.c
  processor.c
)

add_library(GBEmuCore ${SOURCE} ${HEADERS})
