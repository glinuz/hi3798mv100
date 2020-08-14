#ifndef __TEE_VDP_VMX__
#define __TEE_VDP_VMX__

// Parameters for symbol rendering 
// used for communication between modules
/*
typedef enum tagVDP_VMX_MODE_E
{
    VDP_VMX_TYP = 0,
    VDP_VMX_4K_SMALL_SYMBOLS ,
    VDP_VMX_MAXIMUM_COVERAGE ,
    VDP_VMX_RAND_MULTIPLY_THRESHOLD,
    VDP_VMX_RAND_THRESHOLD,
    VDP_VMX_NORM ,
    VDP_VMX_RAND ,
    VDP_VMX_MAX  ,
    VDP_VMX_MIN  ,
    VDP_VMX_BUTT
}VDP_VMX_MODE_E;
*/

//add VID_VMX LayerID 

//The VM-Ctrl provides a c-structure as described below. It is provided to a driver that will 
//communicate those values to the hardware. 
// VideoMark Core Parameters 
typedef struct VMX_CORE_PARAMETERS_REND { 

  /*
    Defines the version of the memory buffer as provided by the VM-Ctrl. Increases in the minor 
    version will only append parameters to the end of the buffer allowing earlier versions to 
    understand parameters and ignoring additions. Major versions need to be understood to proceed. 
    Default: VERSION_MAJOR = 1, VERSION_MINOR =0  
    Variation: rarely.  
  */
  //versioning  
  HI_U8 version_major; 
  HI_U8 version_minor; 
 
  //embedding part 
  HI_U8 watermark_on; 
  HI_U8 frequency_distance[3][3]; 
  HI_U8 background_embedding_on; 
  HI_U32 embedding_strength_threshold_8[3];  
  HI_U32 embedding_strength_threshold_bg_8[3]; 
  HI_U32 embedding_strength_threshold_10[12];  
  HI_U32 embedding_strength_threshold_bg_10[12]; 
  HI_U32 embedding_strength_threshold_12[48];  
  HI_U32 embedding_strength_threshold_bg_12[48]; 
  HI_U32 direction_max; 
  HI_U32 strength_multiply; 
 
  //rendering part 
  HI_U8 payload_symbols[1920]; 
  HI_U32 symbols_rows; 
  HI_U32 symbols_cols; 
  HI_U32 symbols_xpos; 
  HI_U32 symbols_ypos; 
  HI_U32 symbol_size; 
  HI_U32 spacing_vert; 
  HI_U32 spacing_horz; 
  HI_U32 symbol_scale_control;
 
}vmx_hw_soc_rend_t;

typedef enum hiVM_HW_Err_t
{
    VM_HW_OK          = 0,  //Expected return code  
    VM_HW_PARAM_ERROR = 1,  //Error signal if one of the parameters is incorrect  
                            //e.g. inconsistent or outside expected range 
    VM_HW_FAIL        = 2,  //Error signal for failed execution, e.g. uninitialized hardware   
}VM_HW_Err_t;

VM_HW_Err_t VM_HW_SetParameters_Rend (HI_U8 bServiceIdx, vmx_hw_soc_rend_t * hwParameters);

//在参数被设置到硬件之前，调用一次，初始化水印硬件  
VM_HW_Err_t VM_HW_Init (HI_VOID);  

//在调用VM_HW_Init函数之后，如果需要标识不同的视频流添加不同的水印，需要调用这个函数初始//化相应码流对应的水印硬件
VM_HW_Err_t VM_HW_OpenSession(HI_U8 bServiceIdx);  

//在水印功能关闭前调用一次
VM_HW_Err_t VM_HW_CloseSession(HI_U8 bServiceIdx); 

//在添加水印终止前调用一次
VM_HW_Err_t VM_HW_Term (HI_VOID);  


#endif

