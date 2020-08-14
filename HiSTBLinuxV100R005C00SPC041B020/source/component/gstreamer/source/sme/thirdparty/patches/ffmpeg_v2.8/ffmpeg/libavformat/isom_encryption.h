
#ifndef AVFORMAT_ISOM_ENCRYPTION_H
#define AVFORMAT_ISOM_ENCRYPTION_H

#include "avformat.h"

typedef uint8_t ff_uuid_extended_type[16];

typedef struct {
    uint16_t    clear_data_size;
    uint32_t    encrypted_data_size;
}SubSample;

typedef struct {
    uint8_t     *data;
    int         size;
    int         use_subsample;
    uint16_t    subsample_count;
    SubSample   **subsample;
}IV;

typedef struct {
    uint32_t    default_algorithm_id;
    uint8_t     default_iv_size;
    uint8_t     default_kid[16];
}MOVTenc;

typedef struct {
    int         sample_count;
    IV          **iv;
}MOVSenc;

typedef struct {
    uint8_t     data[16];
}KID;

typedef struct {
    uint8_t     system_id[16];
    uint32_t    kid_count;
    KID         **kid;
    uint32_t    data_size;
    uint8_t     *data;
}MOVPssh;

MOVPssh* ff_mov_get_pssh(AVFormatContext *s);
MOVTenc* ff_mov_get_tenc(AVFormatContext *s);

#endif /* AVFORMAT_ISOM_ENCRYPTION_H */
