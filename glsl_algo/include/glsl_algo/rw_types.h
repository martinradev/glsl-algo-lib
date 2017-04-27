#ifndef GLSL_ALGO_RW_TYPES_H
#define GLSL_ALGO_RW_TYPES_H

#ifdef __cplusplus
extern "C"{
#endif

typedef enum
{
    GARWTint1,
    GARWTint2,
    GARWTint4,
    
    GARWTuint1,
    GARWTuint2,
    GARWTuint4,
    
    GARWTfloat1,
    GARWTfloat2,
    GARWTfloat4,
    
    GARWTundefined
} GLSL_ALGO_READ_WRITE_TYPE;

static inline unsigned glsl_algo_convert_rw_type(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
      case GARWTint1:
      case GARWTint2:
      case GARWTint4:
        return 0u;
      case GARWTuint1:
      case GARWTuint2:
      case GARWTuint4:
        return 1u;
      case GARWTfloat1:
      case GARWTfloat2:
      case GARWTfloat4:
        return 2u;
      default:
        break;
    }
    return GARWTundefined;
}

static inline unsigned glsl_algo_get_rw_num_elements(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
        case GARWTint1:
        case GARWTuint1:
        case GARWTfloat1:
            return 1;
        case GARWTint2:
        case GARWTuint2:
        case GARWTfloat2:
            return 2;
        case GARWTint4:
        case GARWTuint4:
        case GARWTfloat4:
            return 4;
        default:
            GLSL_ALGO_ERROR("Read/write type not defined");
    }
    return 0;
}

static inline const char *const glsl_algo_get_rw_type_name(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
        case GARWTint1:
            return "int";
        case GARWTint2:
            return "ivec2";
        case GARWTint4:
            return "ivec4";
        case GARWTuint1:
            return "uint";
        case GARWTuint2:
            return "uvec2";
        case GARWTuint4:
            return "uvec4";
        case GARWTfloat1:
            return "float";
        case GARWTfloat2:
            return "vec2";
        case GARWTfloat4:
            return "vec4";
        default:
            GLSL_ALGO_ERROR("Read/write type not defined");
    }
    return NULL;
}

static inline GLSL_ALGO_READ_WRITE_TYPE get_equivalent_scalar_type(GLSL_ALGO_READ_WRITE_TYPE type)
{
    switch(type)
    {
        case GARWTint1:
            return GARWTint1;
        case GARWTint2:
            return GARWTint1;
        case GARWTint4:
            return GARWTint1;
        case GARWTuint1:
            return GARWTuint1;
        case GARWTuint2:
            return GARWTuint1;
        case GARWTuint4:
            return GARWTuint1;
        case GARWTfloat1:
            return GARWTfloat1;
        case GARWTfloat2:
            return GARWTfloat1;
        case GARWTfloat4:
            return GARWTfloat1;
        default:
            GLSL_ALGO_ERROR("Read/write type not defined");
    }
    return GARWTundefined;
}

static inline GLSL_ALGO_READ_WRITE_TYPE get_equivalent_vector_type(GLSL_ALGO_READ_WRITE_TYPE type, unsigned elements)
{
    switch(elements)
    {
        case 1:
        case 2:
        case 4:
            break;
        default:
            GLSL_ALGO_ERROR("Vector size can be only of size 1,2 and 4.");
    }
    
    switch(type)
    {
        case GARWTfloat1:
        case GARWTfloat2:
        case GARWTfloat4:
        {
            switch(elements)
            {
                case 1:
                    return GARWTfloat1;
                case 2:
                    return GARWTfloat2;
                case 4:
                    return GARWTfloat4;
            }
            break;
        }
        case GARWTint1:
        case GARWTint2:
        case GARWTint4:
        {
            switch(elements)
            {
                case 1:
                    return GARWTint1;
                case 2:
                    return GARWTint2;
                case 4:
                    return GARWTint4;
            }
            break;
        }
        case GARWTuint1:
        case GARWTuint2:
        case GARWTuint4:
        {
            switch(elements)
            {
                case 1:
                    return GARWTuint1;
                case 2:
                    return GARWTuint2;
                case 4:
                    return GARWTuint4;
            }
            break;
        }
    }
    return GARWTundefined;
}

#ifdef __cplusplus
}
#endif

#endif // GLSL_ALGO_RW_TYPES_H