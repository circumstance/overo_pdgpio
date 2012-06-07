/* overo_gpioout
   
   pd external for output to gumstix overo gpio

   Copyright (C) 2012 Tim Redfern
   tim@eclectronics.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   allows use of GPIO pins 144,145 and 170 on overo expansion header for output
   creation parameter 1 - 3 - (gpio 144 / 170/ 145)

   WARNING use at your own risk!
*/

#include "m_pd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))


t_class *overo_gpioout_class;

typedef struct overo_gpioout
{
	t_object x_obj;
	int gpioNum;
	char outfile[40];
	FILE *fs;
	int set; //0 or 1
} t_overo_gpioout;

void overo_gpioout_float(t_overo_gpioout *x, t_floatarg f)
{
	int newset=(int) f;
	if (newset!=x->set) {
		x->fs = fopen( x->outfile, "w" );
		
		int t;
		
		if (newset) t=fwrite( "1", sizeof(char), 1, x->fs );
		else t=fwrite( "0", sizeof(char), 1, x->fs );

		fclose(x->fs);
		x->set=newset;
	}
}

void overo_gpioout_free(t_overo_gpioout *x)
{
	//
}

void *overo_gpioout_new(t_floatarg f)
{
    t_overo_gpioout *x = (t_overo_gpioout *)pd_new(overo_gpioout_class);
	int nums[]={144,170,145};
	x->gpioNum=nums[min(2,max(0,((int) f)-1))]; //maps switch number to GPIO number
	post("overo_gpoout: using GPIO(%d): switch %d\n",x->gpioNum,min(3,max(1,(int) f)));
	sprintf(x->outfile,"/sys/class/gpio/gpio%d/value",x->gpioNum);
	//post("outfile: %s\n",x->outfile);

	char gpionum[4];
	sprintf(gpionum,"%d",x->gpioNum);

	//if ((
	x->fs = fopen( "/sys/class/gpio/export", "w" );
	//) < 0 )
	//{
	//	error( "Unable to open exportfile" );
	//	exit( 1 );
	//}
	int t=fwrite( gpionum, sizeof(char), 3, x->fs );
	fclose(x->fs);
	
	char dirfile[40];
	sprintf(dirfile,"/sys/class/gpio/gpio%d/direction",x->gpioNum);
	x->fs = fopen(dirfile, "w" );
	t=fwrite( "out", sizeof(char), 3, x->fs );
	fclose(x->fs);
	
	//if (( 
	x->fs = fopen( x->outfile, "w" );
	//) < 0 )
	//
	//	error( "Unable to open outfile" );
	//	exit( 1 );
	//}
	t=fwrite( "0", sizeof(char), 1, x->fs );
	x->set=0;
	fclose(x->fs);
    return (void *)x;
}

void overo_gpioout_setup(void)
{
  overo_gpioout_class = class_new(gensym("overo_gpioout"), (t_newmethod)overo_gpioout_new, (t_method)overo_gpioout_free, sizeof(t_overo_gpioout), 0, A_DEFFLOAT, 0);
  class_addfloat(overo_gpioout_class,overo_gpioout_float);
}
