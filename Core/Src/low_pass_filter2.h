#define ARM_MATH_CM4  // Use ARM Cortex M4
#define __FPU_PRESENT 1

#include <arm_math.h>    // Include CMSIS header

static const int filter2_numStages = 2;
static const int filter2_coefficientLength = 10;
extern float filter2_coefficients[10];

typedef struct
{
	float state[8];
	float output;
} filter2Type;

typedef struct
{
    float *pInput;
    float *pOutput;
    float *pState;
    float *pCoefficients;
    short count;
} filter2_executionState;


filter2Type *filter2_create( void );
void filter2_destroy( filter2Type *pObject );
void filter2_init( filter2Type * pThis );
void filter2_reset( filter2Type * pThis );
#define filter2_writeInput( pThis, input )  \
    filter2_filterBlock( pThis, &(input), &(pThis)->output, 1 );

#define filter2_readOutput( pThis )  \
    (pThis)->output

int filterC_filterBlock( filter2Type * pThis, float * pInput, float * pOutput, unsigned int count );
#define filter2_outputToFloat( output )  \
    (output)

#define filter2_inputFromFloat( input )  \
    (input)

void filter2_filterBiquad( filter2_executionState * pExecState );

