#ifndef SREC_H
#define SREC_H

    #include <stdint.h>
    #define SREC_DATA_MAX_SIZE (64U)

    typedef enum
    {
        SREC_OK = 0U,
        SREC_ERROR_TYPE,
        SREC_ERROR_CHECKSUM,
        SREC_ERROR_FORMAT,
        SREC_ERROR_NULL_PTR
    } SREC_Status_t;

    typedef struct
    {
        uint32_t address;
        uint8_t data[SREC_DATA_MAX_SIZE];
        uint8_t data_length;
        uint8_t type;
    }   SREC_Record_t;

    uint8_t SREC_HexToByte(const uint8_t *hex);

    SREC_Status_t SREC_ParseLine(const uint8_t* line, SREC_Record_t *result);

#endif