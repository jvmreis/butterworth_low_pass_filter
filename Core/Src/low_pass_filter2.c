


#include "low_pass_filter2.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset


#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

//float filter2_coefficients[10] =
//{
//// Scaled for floating point
//
//    0.013640878526405022, 0.027281757052810045, 0.013640878526405022, 1.5590543011416917, -0.614051781937882,// b0, b1, b2, a1, a2
//    0.015625, 0.03125, 0.015625, 1.7577536094827262, -0.8197604429273134// b0, b1, b2, a1, a2
//
//};

float32_t filter2_coefficients[10] ={2.1314e-04,4.2628e-04,2.1314e-04,1.5591,-0.6141,
										1,2,1.7578,-0.8198};

filter2Type *filter2_create( void )
{
    filter2Type *result = (filter2Type *)malloc( sizeof( filter2Type ) ); // Allocate memory for the object
    filter2_init( result );                                               // Initialize it
    return result;                                                        // Return the result
}

void filter2_destroy( filter2Type *pObject )
{
    free( pObject );
}

void filter2_init( filter2Type * pThis )
{
    filter2_reset( pThis );
}

void filter2_reset( filter2Type * pThis )
{
    memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
    pThis->output = 0;                                    // Reset output
}

int filterC_filterBlock( filter2Type * pThis, float * pInput, float * pOutput, unsigned int count )
{
    filter2_executionState executionState;          // The executionState structure holds call data, minimizing stack reads and writes
    if( ! count ) return 0;                         // If there are no input samples, return immediately
    executionState.pInput = pInput;                 // Pointers to the input and output buffers that each call to filterBiquad() will use
    executionState.pOutput = pOutput;               // - pInput and pOutput can be equal, allowing reuse of the same memory.
    executionState.count = count;                   // The number of samples to be processed
    executionState.pState = pThis->state;                   // Pointer to the biquad's internal state and coefficients.
    executionState.pCoefficients = filter2_coefficients;    // Each call to filterBiquad() will advance pState and pCoefficients to the next biquad

    // The 1st call to filter1_filterBiquad() reads from the caller supplied input buffer and writes to the output buffer.
    // The remaining calls to filterBiquad() recycle the same output buffer, so that multiple intermediate buffers are not required.

    filter2_filterBiquad( &executionState );		// Run biquad #0
    executionState.pInput = executionState.pOutput;         // The remaining biquads will now re-use the same output buffer.

    filter2_filterBiquad( &executionState );		// Run biquad #1

    // At this point, the caller-supplied output buffer will contain the filtered samples and the input buffer will contain the unmodified input samples.
    return count;		// Return the number of samples processed, the same as the number of input samples
}

void filter2_filterBiquad( filter2_executionState * pExecState )
{

    // Read state variables
    float x0;
    float x1 = pExecState->pState[0];
    float x2 = pExecState->pState[1];
    float y1 = pExecState->pState[2];
    float y2 = pExecState->pState[3];

    // Read coefficients into work registers
    float b0 = *(pExecState->pCoefficients++);
    float b1 = *(pExecState->pCoefficients++);
    float b2 = *(pExecState->pCoefficients++);
    float a1 = *(pExecState->pCoefficients++);
    float a2 = *(pExecState->pCoefficients++);

    // Read source and target pointers
    float *pInput  = pExecState->pInput;
    float *pOutput = pExecState->pOutput;
    short count = pExecState->count;
    float accumulator;

    while( count-- )
    {
        x0 = *(pInput++);

        accumulator  = x2 * b2;
        accumulator += x1 * b1;
        accumulator += x0 * b0;

        x2 = x1;        // Shuffle left history buffer
        x1 = x0;

        accumulator += y2 * a2;
        accumulator += y1 * a1;

        y2 = y1;        // Shuffle right history buffer
        y1 = accumulator ;

        *(pOutput++) = accumulator ;
    }

    *(pExecState->pState++) = x1;
    *(pExecState->pState++) = x2;
    *(pExecState->pState++) = y1;
    *(pExecState->pState++) = y2;

}


