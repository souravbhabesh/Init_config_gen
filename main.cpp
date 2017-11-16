#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include "stdint.h"
#include "gsd.h"
#include "variables.h"
#include "gsd_read.h"
#include "analyze.h"

  int NX,NY,RUN;
  double KAPPA;
  int STEPS,FRAMES,CLONE;

int main(int argc, char **argv)
{
  FILE *therm;
  char init_strip[256],trajectory_file[256],thermalpos_file[256];
  int frame_cnt=0;
  double slider_thermal=0;

   switch (argc){
     case 7:
       sscanf(argv[1],"%d",&NX);    
       sscanf(argv[2],"%d",&NY);
       sscanf(argv[3],"%lf",&KAPPA);
       sscanf(argv[4],"%d",&RUN);
       sscanf(argv[5],"%d",&STEPS);
       sscanf(argv[6],"%d",&CLONE); 
       break;
     default:
       print_and_exit("Usage: %s NX NY KAPPA RUN STEPS CLONE (generate multiple clamped files total 10(CLONE+1))\n",
           argv[0]);
   }

  FRAMES=STEPS/PERIOD;

  // Init_strip.gsd filepath
  sprintf(init_strip,"../Sim_dump_ribbon/init_strip_L%d_W%d.gsd",NX,NY);
  printf("Init_strip.gsd : %s\n",init_strip);
  load_gsd(init_strip,0); 

  for(int run=1;run<=RUN;run++)
  {
	// Trajectory.gsd filepath
	sprintf(trajectory_file,"../Sim_dump_ribbon/L%d/W%d/k%.1f/r%d/traj.gsd",NX,NY,KAPPA,run);
	for(int frames=FRAMES/2;frames<FRAMES;frames++)
	{
		load_gsd(trajectory_file,frames);
		slider_thermal+=avg_slider_pos();
		frame_cnt++;
	}
  }
  /*`	Average thermalised position of right end of the ribbon	*/
  slider_thermal/=frame_cnt;
  printf ("Thermal position of right end %.8g\n",slider_thermal);

  
  /*	fixing last frame from each run at the thermalised position	*/
  double frame_slider=0;
  int slider_node=0;
  double delta_pos=0;
  double shifted_frame_slider=0;

  for(int run=1;run<=RUN;run++)
  {
	frame_slider=0;
	slider_node=0;
	delta_pos=0;
	shifted_frame_slider=0;
	// Trajectory.gsd filepath
        sprintf(trajectory_file,"../Sim_dump_ribbon/L%d/W%d/k%.1f/r%d/traj.gsd",NX,NY,KAPPA,run);
    for(int k=0;k<CLONE;k++)
    {
	sprintf(thermalpos_file,"../Sim_dump_ribbon/L%d/W%d/k%.1f/r%d/thermalPosFrame.bin",NX,NY,KAPPA,run+10*k);
	printf("thermalposition file : %s\n",thermalpos_file);
	therm = fopen(thermalpos_file, "wb");
	if (therm == NULL)
   	{
        	print_and_exit("Could Not Open File to write thermalised position data");
   	}
	load_gsd(trajectory_file,FRAMES-k-1);

        /*	Shifting frame left end to averaged thermalized position	*/
	for(int i=0;i<N;i++)
	{
        	if(particleID[i]==3)
        	{
                	frame_slider+=position[3*i];
                	slider_node++;
        	}
   	}
	frame_slider/=slider_node; /*	Average position of left end of the frame	*/
	printf ("Slider position of frame %.8f\n",frame_slider);
	delta_pos = slider_thermal - frame_slider;

	/*	Shifting right end nodes to the thermal average 	*/
	slider_node=0;
	for(int i=0;i<N;i++)
        {
                if(particleID[i]==3)
                {
                        position[3*i] = position[3*i] + delta_pos; //shifting x coordinates
			shifted_frame_slider += position[3*i];
                        slider_node++;
                }
        }
	shifted_frame_slider /= slider_node;
	printf ("shifted Slider position of frame %.8f\n\n",shifted_frame_slider);
	
	/* writing position data to binary file		*/
	fwrite(position,sizeof(position),1,therm);
	fclose(therm);	
     }

  }

  return 0;
}
