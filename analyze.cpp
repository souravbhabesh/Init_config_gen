#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stdint.h"
#include "variables.h"
#include "analyze.h"

/*	Slider average position	*/
double avg_slider_pos()
{
   double slider_pos=0;
   int slider_node=0;
   for(int i=0;i<N;i++)
   {
	if(particleID[i]==3)
	{
		slider_pos+=position[3*i];
		slider_node++;
	}
   }
   return (slider_pos/slider_node);
}

