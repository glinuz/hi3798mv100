#include "tee_common.h"
#include "uuid_disp.h"
#include "tee_internal_api.h"
#include "tee_mem_mgmt_api.h"
#define TA_DEBUG
#include "tee_log.h"
#include "tee_fs.h"
#include "tee_trusted_storage_api.h"
#include "tee_time_api.h"
#include "rpmb_fcntl.h"


#define HISI_DEMO_CMD_HELLO				0
#define HISI_DEMO_CMD_STORAGE			1
#define HISI_DEMO_CMD_MMZ				2
#define HISI_DEMO_CMD_RPMB				3
#define HISI_DEMO_CMD_WR_RPMB_KEY		4

#define TEST1     //test transient object operation
#define TEST2     //simple Persistent test: test cteate,close,open,closeanddelete persistent

static uint32_t storageID=TEE_OBJECT_STORAGE_PRIVATE;

TEE_Result test_case1_transient_object(void)
{
    TEE_ObjectHandle first_object;
    TEE_Result ret;
    TEE_Attribute *test_attribute;
    void* attr_buffer="test save attribute\n";

    ta_debug("Test_case1_transient_object start:============\n");
    ret = TEE_AllocateTransientObject(TEE_TYPE_AES, 0x00000800,(&first_object));
    if (TEE_SUCCESS != ret)
    {
        ta_error("Failed to execute TEE_AllocateTransientObject:ret = %d\n", ret);
        goto case1_error;
    }

#ifdef GP_V1_1_SUPPORTED
     //test for default objectinfo
    if(first_object->ObjectInfo->objectType != TEE_TYPE_AES || first_object->ObjectInfo->keySize !=0 ||
        first_object->ObjectInfo->maxKeySize != 0x00000800 ||
        first_object->ObjectInfo->objectUsage != TEE_USAGE_DEFAULT||
        first_object->ObjectInfo->dataSize != 0 || first_object->ObjectInfo->dataPosition != 0 ||
        first_object->ObjectInfo->handleFlags != 0)
#else
    //test for default objectinfo
    if(first_object->ObjectInfo->objectType != TEE_TYPE_AES || first_object->ObjectInfo->objectSize !=0 ||
        first_object->ObjectInfo->maxObjectSize != 0x00000800 ||
        first_object->ObjectInfo->objectUsage != TEE_USAGE_DEFAULT||
        first_object->ObjectInfo->dataSize != 0 || first_object->ObjectInfo->dataPosition != 0 ||
        first_object->ObjectInfo->handleFlags != 0)
#endif
    {
        ta_error("check objectInfo error\n");
        goto case1_error;
    }
    ta_debug("objectinfo is right \n");

    //need to allocate
    test_attribute = (TEE_Attribute*)TEE_Malloc(sizeof(TEE_Attribute),0);
    if(test_attribute == NULL) {
        ta_error("not available to allocate \n");
        goto case1_error;
    }
    TEE_InitRefAttribute(test_attribute, TEE_ATTR_SECRET_VALUE, attr_buffer,(size_t)strlen(attr_buffer));

    ta_debug("Initattribute is end! \n");
    ta_debug("attribute ID is 0x%x \n",test_attribute->attributeID);
    ta_debug("buffer of attribute is %s \n",test_attribute->content.ref.buffer);
    ta_debug("lengrh of attribute is 0x%x \n",test_attribute->content.ref.length);

    ret = TEE_PopulateTransientObject(first_object, test_attribute, 1);
    if (TEE_SUCCESS != ret)
    {
        ta_error("Failed to execute TEE_PopulateTransientObject:ret = %d\n", ret);
        goto case1_error;
    }
    ta_debug("Populate is end! \n");
    ta_debug("attribute ID is 0x%x \n",first_object->Attribute->attributeID);
    ta_debug("buffer of attribute is %s \n",first_object->Attribute->content.ref.buffer);
    ta_debug("lengrh of attribute is 0x%x \n",first_object->Attribute->content.ref.length);

    char testget_buff[128],testget_buff1[128];
    size_t size,size1;
    size = 128;
    ret=TEE_GetObjectBufferAttribute(first_object,TEE_ATTR_SECRET_VALUE, testget_buff,  &size);
    if(ret == TEE_SUCCESS){
        testget_buff[size]='\0';
        ta_debug("buffer of attribute is %s \n",testget_buff);
        ta_debug("lengrh of attribute is 0x%x \n",size);
    }
    else
    {
        ta_error("Failed to execute TEE_GetObjectBufferAttribute:ret = %d\n", ret);
        goto case1_error;
    }

    uint32_t objectUsage = TEE_USAGE_ENCRYPT |  TEE_USAGE_DECRYPT;

#ifdef GP_V1_1_SUPPORTED
    TEE_RestrictObjectUsage1(first_object, objectUsage);
#else
    TEE_RestrictObjectUsage(first_object, objectUsage);
#endif

    ta_debug("objectinfo objectUsage1 is 0x%x \n",first_object->ObjectInfo->objectUsage);
    size = 128;
    ret = TEE_GetObjectBufferAttribute(first_object,TEE_ATTR_SECRET_VALUE, testget_buff,  &size);

    /*check the return value.Make sure get objectBufferAttribute is denied*/
    if (TEE_ERROR_ACCESS_DENIED != ret)
    {
        ta_error("Failed to execute TEE_GetObjectBufferAttribute:does not return TEE_ERROR_ACCESS_DENIED ret = %d\n", ret);
        goto case1_error;
    }
    else
    {
        ta_debug("success to restrict object usage \n");
    }

    ta_debug("FreeTransient is end! \n");
    TEE_ResetTransientObject(first_object);
    ta_debug("reset is end\n");
    TEE_FreeTransientObject(first_object);
    ta_debug("free is end\n");

    TEE_Free((void*)test_attribute);

case1_pass:
    ta_debug("Test_case1_transient_object pass:^o^   ^o^\n");
    return TEE_SUCCESS;

case1_error:
    ta_error("test_case1_transient_object failed.\n");
    return TEE_FAIL;
    case2_error:
    ta_error("test_case1_transient_object failed.\n");
    return TEE_FAIL;
}


/*Create file(data stream object), write flash and read */
TEE_Result test_case2_persistent_object(void)
{
    uint32_t r_flags=TEE_DATA_FLAG_ACCESS_READ,w_flags=TEE_DATA_FLAG_ACCESS_WRITE;
    void* create_objectID="crt.txtcase2";
    size_t objectIDLen=0x00000007;
    void* initialData="This a trustedcore created sample initial data\n";
    TEE_ObjectHandle first_object;
    TEE_Result crt_ret_val, open_write_retval, read_ret_val;
    char  read_buffer[255];
    size_t read_test_size=46;
    uint32_t count[1];
    ta_debug("test_case2_persistent_object start:============\n");
    ta_debug("creat file, write file for data and infoattr  \n");

    /*创建persistent object: flag为写操作(会将initialData和object info和attributes写入flash)*/
    crt_ret_val=TEE_CreatePersistentObject(storageID, create_objectID,
    strlen(create_objectID),w_flags  | TEE_DATA_FLAG_ACCESS_WRITE_META,
    TEE_HANDLE_NULL , initialData, (size_t)(strlen((char*)initialData)), (&first_object));
    if (TEE_SUCCESS != crt_ret_val)
    {
        ta_error("Failed to execute TEE_CreatePersistentObject:ret = %x\n", crt_ret_val);
        goto case2_error;
    }
    ta_debug("the creat persistent fun return value is  0x%x \n \n",crt_ret_val);
    ta_debug("datasize is 0x%x\n",first_object->ObjectInfo->dataSize);
    TEE_CloseObject( first_object);
    //TEE_CloseAndDeletePersistentObject( first_object);

#if 1
    /*打开已经创建的文件，进行initial data的读取*/
    open_write_retval = TEE_OpenPersistentObject(storageID, create_objectID,
    strlen(create_objectID), r_flags | TEE_DATA_FLAG_ACCESS_WRITE_META, (&first_object));
    if (TEE_SUCCESS != open_write_retval)
    {
        ta_error("Failed to execute TEE_OpenPersistentObject:ret = %x\n", open_write_retval);
        goto case2_error;
    }
    ta_debug("datasize is 0x%x\n",first_object->ObjectInfo->dataSize);
    ta_debug("the objectType is 0x%x \n",first_object->ObjectInfo->objectType);
    ta_debug("objectinfo objectUsage is 0x%x \n",first_object->ObjectInfo->objectUsage);
    read_ret_val = TEE_ReadObjectData(first_object,read_buffer,read_test_size, count);
    if(read_ret_val == TEE_SUCCESS)
    {
        ta_debug("read success!\n");
        read_buffer[*count]='\0';
        /*对读出的数据进行判断，保证读出的数据与creat object时写入的相同*/
        if (memcmp(read_buffer, initialData, read_test_size))
        {
            ta_error("read content is %s \n",read_buffer);
            ta_error("flash content is %s \n",initialData);
            ta_error("Failed to read buffer.\n");
            goto case2_error;
        }
        if (*count != read_test_size)
        {
            ta_error("read length(read_test_size=%d) is not equal to count(count=%d).\n", read_test_size, *count);
            goto case2_error;
        }

        ta_debug("the objectType is 0x%x \n",first_object->ObjectInfo->objectType);
        ta_debug("read is end  \n");
        ta_debug("read count is 0x%x  \n",*count);
        ta_debug("read content is %s \n",read_buffer);
    }
    else{
        ta_error("read error\n");
        ta_error("the objectType is 0x%x \n",first_object->ObjectInfo->objectType);
        ta_error("read is end  \n");
        ta_error("read count is 0x%x  \n",*count);
        ta_error("read content is %s \n",read_buffer);
        goto case2_error;
    }

    //TEE_CloseObject( first_object);
    /*删除已经创建的persistent object,保证不影响后续的测试用例*/
    TEE_CloseAndDeletePersistentObject(first_object);
    /*判断是否删除成功*/
    /*    open_write_retval = TEE_OpenPersistentObject(storageID, create_objectID, objectIDLen, r_flags, (&
first_object));
          if (TEE_SUCCESS == open_write_retval)
          {
          ta_error("Close and delete persistent object failed :ret = %x\n", open_write_retval);
          goto case2_error;
          }
          TEE_CloseObject(first_object);*/
#endif
//tee_storage_exit();
case2_pass:
    ta_debug("test_case2_persistent_object pass:^o^   ^o^\n");
    return TEE_SUCCESS;

case2_error:
    ta_error("test_case2_persistent_object failed.\n");
    return TEE_FAIL;
}

TEE_Result test_storage_sample(void)
{
    uint32_t mem_usage_before;
    uint32_t mem_usage_after;
    TEE_Result ret;
    ta_debug("test_storage_sample start:============\n");

#ifdef TEST1
    /****************************test_case1_transient_object:start ****************************/
    ta_debug("#####Before test_case1_transient_object executed.\n");
    mem_usage_before = get_mem_usage(false);

    ret = test_case1_transient_object();
    if (TEE_SUCCESS != ret)
    {
        ta_error("test_case1_transient_object execute failed.\n");
        goto test_error;
    }

    mem_usage_after = get_mem_usage(false);
    if (mem_usage_before!= mem_usage_after)
    {
        ta_error("Memory Leak.mem region usage before execute = %d.%d%\n", mem_usage_before/100, mem_usage_before%100);
        ta_error("Memory Leak.mem region usage after execute= %d.%d%\n", mem_usage_after/100, mem_usage_after%100);
        goto test_error;
    }
    /****************************test_case1_transient_object:end ****************************/
#endif

#ifdef TEST2
    /****************************test_case2_persistent_object:start ****************************/
    ta_debug("#####Before test_case2_persistent_object executed.\n");
    mem_usage_before = get_mem_usage(false);

    ret = test_case2_persistent_object();
    if (TEE_SUCCESS != ret)
    {
        ta_error("test_case2_persistent_object execute failed.\n");
        goto test_error;
    }

    mem_usage_after = get_mem_usage(false);
    if (mem_usage_before != mem_usage_after)
    {
        ta_error("Memory Leak.mem region usage before execute = %d.%d%\n", mem_usage_before/100, mem_usage_before%100);
        ta_error("Memory Leak.mem region usage after execute= %d.%d%\n", mem_usage_after/100, mem_usage_after%100);
        goto test_error;
    }
    /****************************test_case2_persistent_object:end ****************************/
#endif

test_success:
    ta_debug("#######success to execute test case.   ^o^     OK #####\n");
    return 0;
test_error:
    ta_error("#######Execute test storage errror.   ^&^       FAILED ####\n");
    return -1;
}

TEE_Result test_storage_api(void)
{
	char *obj_id = "test_file.txt";
	char *data_init = "1234567890";
	char *data_append = "<data append>";
	unsigned int flags = 0;
	TEE_ObjectHandle obj;
	TEE_Result ret = 0;
	TEE_ObjectHandle attr_obj;
	char buffer[255];
    unsigned int count;

	ta_debug("_______________________test_storage_api start____________________\n");

	/* object attribute */
    ret = TEE_AllocateTransientObject(TEE_TYPE_AES, 0x00000800, &attr_obj);
    if (TEE_SUCCESS != ret)
    {
        ta_error("TEE_AllocateTransientObject fail. ret[0x%x]\n", ret);
		return ret;
    }

	/* create object */
	ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, 
								(void *)obj_id, 
								strlen(obj_id), 
								TEE_DATA_FLAG_EXCLUSIVE, 
								attr_obj, 
								(void *)data_init, 
								strlen(data_init), 
								&obj);
	if (TEE_SUCCESS != ret) {
		ta_error("TEE_CreatePersistentObject fail. file[%s]. data[%s] ret[%x]\n", obj_id, data_init, ret);
	} else {
		TEE_CloseObject(obj);
	}

	/* open object */
    ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, 
    							   (void *)obj_id, 
    							   strlen(obj_id), 
    							   TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_READ, 
    							   &obj);
    if (TEE_SUCCESS != ret)
    {
        ta_debug("File [%s] not exit. ret[%x]\n", obj_id, ret);
		/* create object */
		ret = TEE_CreatePersistentObject(TEE_OBJECT_STORAGE_PRIVATE, 
									(void *)obj_id, 
									strlen(obj_id), 
									TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_READ, 
									attr_obj, 
									(void *)data_init, 
									strlen(data_init), 
									&obj);
		if (TEE_SUCCESS != ret) {
			ta_error("TEE_CreatePersistentObject fail. file[%s]. data[%s] ret[%x]\n", obj_id, data_init, ret);
			return ret;
		} else {
			TEE_CloseObject(obj);
		}
		
		/* open object */
		ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, 
	    							   (void *)obj_id, 
	    							   strlen(obj_id), 
	    							   TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_READ, 
	    							   &obj);
    	if (TEE_SUCCESS != ret) {
	    	ta_error("TEE_OpenPersistentObject fail. file[%s]. ret[%x]\n", obj_id, ret);
			return ret;
    	}
    }

	memset(&buffer, 0x0, sizeof(buffer));
	ret = TEE_ReadObjectData(obj, (void *)&buffer, sizeof(buffer), &count);
    if (TEE_SUCCESS != ret) {
        ta_error("TEE_ReadObjectData fail. ret[%x]\n", ret);
		return ret;
    } else {
		ta_debug("Read object[%s] OK. buffer[%s] count[0x%x]\n", obj->dataName, buffer, count);
    }

	ret = TEE_WriteObjectData(obj, data_append, strlen(data_append));
    if(TEE_SUCCESS != ret)
    {
        ta_error("TEE_WriteObjectData fail. ret[%x]\n", ret);
		return ret;
    }

	TEE_CloseObject(obj);
	/* open object */
	ret = TEE_OpenPersistentObject(TEE_OBJECT_STORAGE_PRIVATE, 
    							   (void *)obj_id, 
    							   strlen(obj_id), 
    							   TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_READ, 
    							   &obj);
	if (TEE_SUCCESS != ret) {
    	ta_error("TEE_OpenPersistentObject fail. file[%s]. ret[%x]\n", obj_id, ret);
		return ret;
	}

	memset(&buffer, 0x0, sizeof(buffer));
	ret = TEE_ReadObjectData(obj, (void *)&buffer, sizeof(buffer), &count);
    if (TEE_SUCCESS != ret) {
        ta_error("TEE_ReadObjectData fail. ret[%x]\n", ret);
		return ret;
    } else {
		ta_debug("Read object[%s] OK. buffer[%s] count[0x%x]\n", obj->dataName, buffer, count);
    }
	
	TEE_CloseObject(attr_obj);
	TEE_CloseObject(obj);
	
	return ret;
}

/* via TA2TA*/
TEE_Result test_mmz_test(void)
{
#if defined(WITH_HISI_SEC_MMZ)
	TEE_UUID uuid = TEE_SERVICE_SEC_MMZ;
    TEE_TASessionHandle ta2ta_session;
    uint32_t ret_origin = 0;
    TEE_Param ta2ta_params[4] = {0};
    TEE_Result ret = 0;
    uint32_t param_types = 0;

    ta_debug("-------TEE_OpenTASession start-------\n");

    param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE);

    ret = TEE_OpenTASession(&uuid, 0,
                            param_types, ta2ta_params,
                            &ta2ta_session, &ret_origin);

    if(ret != TEE_SUCCESS) {
		ta_error("TEE_OpenTASession failed\n");
		return ret;
    }

    param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE,
                                  TEE_PARAM_TYPE_NONE);
    ret = TEE_InvokeTACommand(ta2ta_session, 0,
                              0x1005,
                              param_types, ta2ta_params,
                              &ret_origin);


    if(ret != TEE_SUCCESS) {
		ta_error("TEE_InvokeTACommand failed\n");
		return ret;
    }

    TEE_CloseTASession(ta2ta_session);

	ta_debug("-------TEE_OpenTASession end-------\n");
#endif
    return TEE_SUCCESS;

}

TEE_Result test_rpmb(void)
{
	#define CHECK_DATA_LEN 256
	const char *check_file_path = "rpmb_rw_check.dat";
	uint8_t written_data[CHECK_DATA_LEN] = {0xab};
	uint8_t readed_data[CHECK_DATA_LEN];
	uint32_t readed_size = 0;
	struct rpmb_fs_stat stat = {0};
	struct rpmb_fs_statdisk statdisk = {0};
	TEE_Result result;

	ta_debug("-------test_rpmb start-------\n");

	/* get RPMB key status */
	uint32_t status = TEE_RPMB_KEY_Status();
	if (TEE_RPMB_KEY_SUCCESS != status) {
		ta_error("TEE_RPMB_KEY_Status status = %x\n", status);
		return TEE_ERROR_GENERIC;
	}

	/* write date to rpmb_rw_check.dat */
	result = TEE_RPMB_FS_Write(check_file_path, written_data, CHECK_DATA_LEN);
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_Write result = %x\n", result);
		return result;
	}

	/* read data from rpmb_rw_check.dat */
	result = TEE_RPMB_FS_Read(check_file_path, readed_data, CHECK_DATA_LEN, &readed_size);
	if ((TEE_SUCCESS != result) || (CHECK_DATA_LEN != readed_size))
	{
		ta_error("TEE_RPMB_FS_Read result = %x, readed_size = %d\n", result, readed_size);
		return result;
	}

	int32_t ret = TEE_MemCompare(readed_data, written_data, CHECK_DATA_LEN);
	if (0 != ret)
	{
		ta_error("TEE_MemCompare ret = %d\n", ret);
		return result;
	}

	/* get file rpmb_rw_check.dat stat */
	result = TEE_RPMB_FS_Stat(check_file_path, &stat);
	if(result != TEE_SUCCESS)
	{
		ta_error("get file stat error. ret = %x\n", result);
		return result;
	}
	ta_debug("TEE_RPMB_FS_Stat stat.size[0x%x]\n", stat.size);


	/* rename rpmb_rw_check.dat to helloworld */
	result = TEE_RPMB_FS_Rename(check_file_path, "helloworld");
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_Rename = %x\n", result);
		return result;
	}

	/* stat RPMB partition */
	result = TEE_RPMB_FS_StatDisk(&statdisk);
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_StatDisk result = %x\n", result);
		return result;
	}
	ta_debug("TEE_RPMB_FS_StatDisk disk_size[0x%x] free_size[0x%x] ta_used_size[0x%x]\n",
		statdisk.disk_size, statdisk.free_size, statdisk.ta_used_size);

	/* delete rpmb_rw_check.dat(helloworld) */
	result = TEE_RPMB_FS_Rm("helloworld");
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_Rm result = %x\n", result);
		return result;
	}

	/* stat RPMB partition */
	result = TEE_RPMB_FS_StatDisk(&statdisk);
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_StatDisk result = %x\n", result);
		return result;
	}
	ta_debug("TEE_RPMB_FS_StatDisk disk_size[0x%x] free_size[0x%x] ta_used_size[0x%x]\n",
		statdisk.disk_size, statdisk.free_size, statdisk.ta_used_size);

	/*delete file which attr is "can be earse" */
	if (TEE_SUCCESS != TEE_RPMB_FS_Erase())
	{
		ta_error("TEE_RPMB_FS_Erase result = %x\n", result);
		return result;
	}

	/* stat RPMB partition */
	result = TEE_RPMB_FS_StatDisk(&statdisk);
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_StatDisk result = %x\n", result);
		return result;
	}
	ta_debug("TEE_RPMB_FS_StatDisk disk_size[0x%x] free_size[0x%x] ta_used_size[0x%x]\n",
		statdisk.disk_size, statdisk.free_size, statdisk.ta_used_size);

	/* format RPMB partition */
	if (TEE_SUCCESS != TEE_RPMB_FS_Format())
	{
		ta_error("TEE_RPMB_FS_Format result = %x\n", result);
		return result;
	}

	/* stat RPMB partition */
	result = TEE_RPMB_FS_StatDisk(&statdisk);
	if (TEE_SUCCESS != result)
	{
		ta_error("TEE_RPMB_FS_StatDisk result = %x\n", result);
		return result;
	}
	ta_debug("TEE_RPMB_FS_StatDisk disk_size[0x%x] free_size[0x%x] ta_used_size[0x%x]\n",
		statdisk.disk_size, statdisk.free_size, statdisk.ta_used_size);

	ta_debug("-------test_rpmb end-------\n");
}

TEE_Result TA_CreateEntryPoint(void)
{
    TEE_Result ret=TEE_SUCCESS;

	ret = AddCaller_CA_exec("hisi_demo_client", 0);
	if (TEE_SUCCESS != ret){
		ta_error("AddCaller error %d\n", ret);
		return ret;
	}

    ta_debug("New entry!\n");
    return ret;
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t paramTypes, TEE_Param params[4], void** sessionContext)
{
    TEE_Result ret=TEE_SUCCESS;
    ta_debug("New session!\n");

    return ret;
}

TEE_Result TA_InvokeCommandEntryPoint(void* session_context, uint32_t cmd_id, uint32_t paramTypes, TEE_Param params[4])
{
    TEE_Result ret = TEE_SUCCESS;

    switch(cmd_id) {
    case HISI_DEMO_CMD_HELLO:
        ta_debug("Hello, Secure World!\n");
        params[3].value.a = 0x12345678;
		params[3].value.b = 0x87654321;
        break;
    case HISI_DEMO_CMD_STORAGE:
        test_storage_sample();
		test_storage_api();
        break;
	case HISI_DEMO_CMD_MMZ:
        test_mmz_test();
		break;
	case HISI_DEMO_CMD_RPMB:
		test_rpmb();
		break;
	case HISI_DEMO_CMD_WR_RPMB_KEY:
		ta_debug("start write rpmb key\n");
		//tee_rpmb_write_key();
		ta_debug("end write rpmb key\n");
		break;
    default:
        ta_debug("Invalid command!\n");
        break;
    }

    return  ret;
}

void TA_CloseSessionEntryPoint(void* session_context)
{
    ta_debug("Close session!\n");
}

void TA_DestroyEntryPoint(void)
{
    ta_debug("Destroy entry!\n");
}
