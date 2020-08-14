#ifndef PKTQ_H
#define PKTQ_H
#include <stdint.h>

typedef struct DataPacket {
    uint8_t              *data;     /**< ponit of data *//**< CNcomment:TS内容数据*/
    void                 *priv;   /**< private *//**< CNcomment:私有*/
    int                   data_size;/**< data size *//**< CNcomment:数据长度 */
    int                   buf_size; /**< data buf size *//**< CNcomment:缓冲长度 */
    int                   rd_idx;   /**< read index *//**< CNcomment:读取指针 */
    struct DataPacket      *next;
} DataPacket;

//create a pkt queue
//@max_buf_size : the max limit buffer size of the queue.
//@forbidden_size: when queue is full, pktq_put_pkt() is allowed
//                 only after actual buffer size <= (@max_buf_size - @forbidden_size)
//@log_ctx: the context of av_log(...)
int pktq_create(void **handle, int max_buf_size, int forbidden_size, void *log_ctx);

int pktq_destroy(void *handle);

//read data, maybe not a complete pkt
//@return the data size readed or < 0 when error occured.
//@timeout_ms:<0,blocked until there is data;==0, if no data return immediately
//            > 0, blocked unitl there is data, or time_ms passed.
int pktq_read(void *handle, uint8_t *buf, int len, int timeout_ms);

//put a pkt into queue, you can put a null pkt to wake up && interrupt
//pktq_get_pkt() or pktq_read().return immediately, time out is not supported yet.
//@pkt->data_size=0 means put a empty buffer into the queue.
//@pkt->next must be null.
//@return the data size of this pkt(may be 0),or < 0 for error.
int pktq_put_pkt(void *handle , DataPacket *pkt, int timeout_ms);

//write eof flag,this will wake up pktq_read()/pktq_get_pkt();
int pktq_set_eof(void *handle, int eof);

//get a complete pkt.
//@return the data size of returned pkt(may be 0),or < 0 for error.
//@timeout_ms:<0,blocked until there is data;==0, if no data return immediately
//            > 0, blocked unitl there is data, or time_ms passed.
int pktq_get_pkt(void *handle, DataPacket *pkt, int timeout_ms);
int pktq_get_data_size(void *handle);
int pktq_get_buf_size(void *handle);
int pktq_clear(void *handle);

//seek in the pkt buffer.
//@return 0 for OK, <0 for error.
//@off: the offset(in bytes) relative to the first byte of pktq buffer.
int pktq_seek(void *handle, int64_t off);
#endif

