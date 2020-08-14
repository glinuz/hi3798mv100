/*------------------------------------------------------------------------------
  Copyright 2012 Sony Corporation

  Last Updated  : $Date:: 2012-03-28 01:40:32 #$
  File Revision : $Revision:: 4945 $
------------------------------------------------------------------------------*/

#include "sony_i2c_log.h"

/*------------------------------------------------------------------------------
  I2c functions for logging.
  These function calls "Real" i2c access functions and output the data.
------------------------------------------------------------------------------*/
static sony_result_t LogRead(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t* pData,
                                uint32_t size, uint8_t mode)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_i2c_log_t *pI2cLog = NULL;

    SONY_TRACE_I2C_ENTER("LogRead");

    if((!pI2c) || (!pI2c->user)){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2cLog = (sony_i2c_log_t*)(pI2c->user);

    if(!pI2cLog->pI2cReal){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Real i2c access */
    result = pI2cLog->pI2cReal->Read(pI2cLog->pI2cReal, deviceAddress, pData, size, mode);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_I2C_RETURN(result);
    }

    if(pI2cLog->fp){
        unsigned int i = 0;
        fprintf(pI2cLog->fp, "R (%02X)    ", deviceAddress);
        for(i=0; i<size; i++){
            fprintf(pI2cLog->fp, " %02X", pData[i]);
        }
        fprintf(pI2cLog->fp, "\n");
    }
    SONY_TRACE_I2C_RETURN(result);
}

static sony_result_t LogWrite(sony_i2c_t* pI2c, uint8_t deviceAddress, const uint8_t * pData,
                                uint32_t size, uint8_t mode)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_i2c_log_t *pI2cLog = NULL;

    SONY_TRACE_I2C_ENTER("LogWrite");

    if((!pI2c) || (!pI2c->user)){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2cLog = (sony_i2c_log_t*)(pI2c->user);

    if(!pI2cLog->pI2cReal){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Real i2c access */
    result = pI2cLog->pI2cReal->Write(pI2cLog->pI2cReal, deviceAddress, pData, size, mode);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_I2C_RETURN(result);
    }

    if(pI2cLog->fp){
        unsigned int i = 0;
        fprintf(pI2cLog->fp, "W (%02X)    ", deviceAddress);
        for(i=0; i<size; i++){
            fprintf(pI2cLog->fp, " %02X", pData[i]);
        }
        fprintf(pI2cLog->fp, "\n");
    }
    SONY_TRACE_I2C_RETURN(result);
}

static sony_result_t LogReadRegister(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress,
                                        uint8_t* pData, uint32_t size)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_i2c_log_t *pI2cLog = NULL;

    SONY_TRACE_I2C_ENTER("LogReadRegister");

    if((!pI2c) || (!pI2c->user)){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2cLog = (sony_i2c_log_t*)(pI2c->user);

    if(!pI2cLog->pI2cReal){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Real i2c access */
    result = pI2cLog->pI2cReal->ReadRegister(pI2cLog->pI2cReal, deviceAddress, subAddress, pData, size);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_I2C_RETURN(result);
    }

    if(pI2cLog->fp){
        unsigned int i = 0;
        fprintf(pI2cLog->fp, "RR(%02X, %02X)", deviceAddress, subAddress);
        for(i=0; i<size; i++){
            fprintf(pI2cLog->fp, " %02X", pData[i]);
        }
        fprintf(pI2cLog->fp, "\n");
    }
    SONY_TRACE_I2C_RETURN(result);
}

static sony_result_t LogWriteRegister(sony_i2c_t* pI2c, uint8_t deviceAddress, uint8_t subAddress,
                                        const uint8_t* pData, uint32_t size)
{
    sony_result_t result = SONY_RESULT_OK;
    sony_i2c_log_t *pI2cLog = NULL;

    SONY_TRACE_I2C_ENTER("LogWriteRegister");

    if((!pI2c) || (!pI2c->user)){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2cLog = (sony_i2c_log_t*)(pI2c->user);

    if(!pI2cLog->pI2cReal){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    /* Real i2c access */
    result = pI2cLog->pI2cReal->WriteRegister(pI2cLog->pI2cReal, deviceAddress, subAddress, pData, size);
    if(result != SONY_RESULT_OK){
        SONY_TRACE_I2C_RETURN(result);
    }

    if(pI2cLog->fp){
        unsigned int i = 0;
        fprintf(pI2cLog->fp, "WR(%02X, %02X)", deviceAddress, subAddress);
        for(i=0; i<size; i++){
            fprintf(pI2cLog->fp, " %02X", pData[i]);
        }
        fprintf(pI2cLog->fp, "\n");
    }
    SONY_TRACE_I2C_RETURN(result);
}

/*--------------------------------------------------------------------
  I2c struct instance creation (for logging)

  sony_i2c_t*         pI2c         Instance of i2c control struct
  sony_i2c_t*         pI2cReal     Instance of "Real" i2c control struct
  sony_i2c_log_t*     pI2cLog      Instance of sony_i2c_log_t struct
--------------------------------------------------------------------*/
sony_result_t sony_i2c_CreateI2cLog(sony_i2c_t *pI2c, sony_i2c_t *pI2cReal, sony_i2c_log_t *pI2cLog)
{
    SONY_TRACE_I2C_ENTER("sony_i2c_CreateI2cLog");

    if((!pI2c) || (!pI2cReal) || (!pI2cLog)){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2c->Read = LogRead;
    pI2c->Write = LogWrite;
    pI2c->ReadRegister = LogReadRegister;
    pI2c->WriteRegister = LogWriteRegister;
    pI2c->WriteOneRegister = sony_i2c_CommonWriteOneRegister;
    pI2c->gwAddress = pI2cReal->gwAddress;
    pI2c->gwSub = pI2cReal->gwSub;
    pI2c->user = pI2cLog;

    pI2cLog->pI2cReal = pI2cReal;
    pI2cLog->fp = NULL;

    SONY_TRACE_I2C_RETURN(SONY_RESULT_OK);
}

/*--------------------------------------------------------------------
  Enable/Disable i2c logging

  sony_i2c_t*         pI2c         Instance of i2c control struct
  FILE*               fp           File pointer for saving log (NULL->disable logging)
--------------------------------------------------------------------*/
sony_result_t sony_i2c_EnableI2cLog(sony_i2c_t *pI2c, FILE *fp)
{
    sony_i2c_log_t *pI2cLog = NULL;

    SONY_TRACE_I2C_ENTER("sony_i2c_EnableI2cLog");

    if((!pI2c) || (!pI2c->user)){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2cLog = (sony_i2c_log_t*)(pI2c->user);

    if(!pI2cLog->pI2cReal){
        SONY_TRACE_I2C_RETURN(SONY_RESULT_ERROR_ARG);
    }

    pI2cLog->fp = fp;

    SONY_TRACE_I2C_RETURN(SONY_RESULT_OK);
}

