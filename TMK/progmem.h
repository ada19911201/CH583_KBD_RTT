#ifndef PROGMEM_H
#define PROGMEM_H 1

#   define PROGMEM
#   define pgm_read_byte(p)     *((unsigned char*)p)
#   define pgm_read_word(p)     *((uint16_t*)p)

#endif
