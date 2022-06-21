#include "ht_tensor.hpp"
#include "host_tensor_generator.hpp"
#include "ht_tensor_generator_utility.hpp"

#define HIPTENSOR_MAX_TENSORS 3

std::size_t hiptensorTensorDescriptor_t::hiptensorGetElementSpace() const
{	
    return ht_desc.GetElementSpace();
}

hiptensorStatus_t hiptensorInitTensorDescriptor(const hiptensorHandle_t* handle,
			  hiptensorTensorDescriptor_t* desc, const uint32_t numModes,
              		  const int64_t lens[], const int64_t strides[], 
              		  hiptensorDataType_t dataType, hiptensorOperator_t unaryOp)
{   
    if (!handle || !desc)
        return HIPTENSOR_STATUS_NOT_INITIALIZED;
    
    if (((!lens) && (!strides)) || dataType != HIPTENSOR_R_32F || unaryOp != HIPTENSOR_OP_IDENTITY)
        return  HIPTENSOR_STATUS_INVALID_VALUE;

    using descType = float;
    int ht_index = 0;

    std::vector<std::int64_t> ht_lens;
    std::vector<std::int64_t> ht_strides;

    for (int index=0; index < numModes; index++)
    {
	ht_lens.push_back(lens[index]);
	if (strides)
	    ht_strides.push_back(strides[index]);
    }
	
    if (!strides) 
   	desc->ht_desc = HostTensorDescriptor(std::vector<std::size_t>(ht_lens.begin(), ht_lens.end()));
    else
      	desc->ht_desc = HostTensorDescriptor(std::vector<std::size_t>(ht_lens.begin(), ht_lens.end()),
		       				    std::vector<std::size_t>(ht_strides.begin(), ht_strides.end()));
    
    desc->ht_type = dataType;
    
    std::cout << desc << std::endl;

    return HIPTENSOR_STATUS_SUCCESS;
}

hiptensorStatus_t hiptensorGetAlignmentRequirement(const hiptensorHandle_t* handle,
                          const void *ptr, const hiptensorTensorDescriptor_t* desc, uint32_t* alignmentRequirement)
{
    if (!desc)
        return HIPTENSOR_STATUS_NOT_INITIALIZED;
    
    *alignmentRequirement = desc->hiptensorGetElementSpace();
    return HIPTENSOR_STATUS_SUCCESS;
}

#if 0
hiptensorStatus_t hiptensorGenerateInputTensorElements(const hiptensorHandle_t *handle, 
                          hiptensorTensorDescriptor_t* ipDesc1, int64_t *A,
                          hiptensorTensorDescriptor_t* ipDesc2, int64_t *B,
                          const uint8_t init_style)
{
    if (!handle || !ipDesc1 || !ipDesc2)
        return  HIPTENSOR_STATUS_NOT_INITIALIZED;

    if ((ipDesc1->ht_type != HIPTENSOR_R_32F) && (ipDesc2->ht_type != HIPTENSOR_R_32F))
        return  HIPTENSOR_STATUS_INVALID_VALUE;

    using descType1 = float;
    using descType2 = float;

    switch(init_style)
    {
        case 0: break;
        case 1:
            ht_tensor[0].GenerateTensorValue(GeneratorTensor_2<descType1>{-5, 5});
            ht_tensor[1].GenerateTensorValue(GeneratorTensor_2<descType2>{-5, 5});
            break;
        case 2:
            ht_tensor[0].GenerateTensorValue(GeneratorTensor_3<descType1>{0.0, 1.0});
            ht_tensor[1].GenerateTensorValue(GeneratorTensor_3<descType2>{-0.5, 0.5});
            break;
        case 3:
            ht_tensor[0].GenerateTensorValue(GeneratorTensor_cuTensor<descType1>{});
            ht_tensor[1].GenerateTensorValue(GeneratorTensor_cuTensor<descType2>{});
            break;
        default:
            ht_tensor[0].GenerateTensorValue(GeneratorTensor_Sequential<0>{});
            ht_tensor[1].GenerateTensorValue(GeneratorTensor_Sequential<1>{});
    }

    A = ht_tensor[0].mData.data();
    B = ht_tensor[1].mData.data();

    return HIPTENSOR_STATUS_SUCCESS;
}

#endif