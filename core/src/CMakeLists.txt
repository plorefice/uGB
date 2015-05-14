include_directories ( ${GBEMU_ARCH_INCLUDE} )
include_directories ( ${GBEMU_CORE_INCLUDE} )

set ( HEADERS
  ${GBEMU_CORE_INCLUDE}/memory.h
  ${GBEMU_CORE_INCLUDE}/opcodes.h
  ${GBEMU_CORE_INCLUDE}/processor.h
)

set ( SOURCE
  opcodes.c
)

add_library(GBEmuCore SHARED ${SOURCE} ${HEADERS})
