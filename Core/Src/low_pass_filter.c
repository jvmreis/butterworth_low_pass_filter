

#include "low_pass_filter.h"

#include <stdlib.h> // For malloc/free
#include <string.h> // For memset

//float32_t filter1_coefficients[10] =
//{
//// Scaled for floating point
//    0.013640878526405022, 0.027281757052810045, 0.013640878526405022, 1.5590543011416917, -0.614051781937882,// b0, b1, b2, a1, a2
//    0.015625, 0.03125, 0.015625, 1.7577536094827262, -0.8197604429273134// b0, b1, b2, a1, a2
//
//};

//float32_t filter1_coefficients[10] =
//{
//    0.000213138726975, 0.000426277458891, 0.000213138723137, 1.000000000000000, -1.559054301141583,
//    1.000000000000000, 1.999999976819692, 1.000000018008094, 1.000000000000000, -1.757753609482836};
//float32_t filter1_coefficients[10] =
float32_t filter1_coefficients[10] ={2.1314e-04,4.2628e-04,2.1314e-04,1.5591,-0.6141,
										1,2,1.7578,-0.8198};

filter1Type *filter1_create( void )
{
    filter1Type *result = (filter1Type *)malloc( sizeof( filter1Type ) ); // Allocate memory for the object
    filter1_init( result );                                               // Initialize it
    return result;                                                        // Return the result
}

void filter1_destroy( filter1Type *pObject )
{
    free( pObject );
}

void filter1_init( filter1Type * pThis )
{
	arm_biquad_cascade_df1_init_f32(	&pThis->instance, filter1_numStages, filter1_coefficients, pThis->state );
	filter1_reset( pThis );
}

void filter1_reset( filter1Type * pThis )
{
	memset( &pThis->state, 0, sizeof( pThis->state ) ); // Reset state to 0
	pThis->output = 0;									// Reset output
}

int filterCMSIS_filterBlock( filter1Type * pThis, float * pInput, float * pOutput, unsigned int count )
{
	arm_biquad_cascade_df1_f32( &pThis->instance, pInput, pOutput, count );
	return count;
}

void init_filter(){
	  filter1Type *filtro_low_pass;
	  filter1_init(filtro_low_pass);
}
