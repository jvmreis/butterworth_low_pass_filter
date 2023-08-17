
#define ARM_MATH_CM4  // Use ARM Cortex M4
#define __FPU_PRESENT 1

#include <arm_math.h>    // Include CMSIS header


extern float32_t filter1_coefficients[10];
static const int filter1_numStages = 2;

typedef struct
{
	arm_biquad_casd_df1_inst_f32 instance;
	float32_t state[8];
	float32_t output;
} filter1Type;

//filter1Type *filter1Type_M;

filter1Type *filter1_create( void );
void filter1_destroy( filter1Type *pObject );
void filter1_init( filter1Type * pThis );
void filter1_reset( filter1Type * pThis );

#define filter1_writeInput( pThis, input )  \
	arm_biquad_cascade_df1_f32( &pThis->instance, &input, &pThis->output, 1 );

#define filter1_readOutput( pThis )  \
	pThis->output


int filterCMSIS_filterBlock( filter1Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define filter1_outputToFloat( output )  \
    (output)

#define filter1_inputFromFloat( input )  \
    (input)

