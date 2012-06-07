/* overo_gpioin
   
   pd external to take input from gumstix overo gpio

   Copyright (C) 2012 Tim Redfern
   tim@eclectronics.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   allows use of GPIO pins 146 and 147 on overo expansion header for input
   creation parameter 1 - 2 - (gpio 146 / 147)

   WARNING use at your own risk!
*/

#include "m_pd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(X,Y) ((X) < (Y) ? (X) : (Y))
#define max(X,Y) ((X) > (Y) ? (X) : (Y))


t_class *overo_gpioin_class;

typedef struct overo_gpioin
{
	t_object x_obj;
	int gpioNum;
	char inputfile[32];
	char buffer[2];
	FILE *fs;
	int set; //0 or 1
} t_overo_gpioin;

void overo_gpioin_bang(t_overo_gpioin *x)
{
  //fseek( x->fs, 0, SEEK_SET );
	//if (( 
	x->fs = fopen( x->inputfile, "r" );
	//) < 0 )
	//{
	//	error( "Unable to open inputfile" );
	//	exit( 1 );
	//}
  int t=fread( x->buffer, sizeof(char), 1, x->fs );
  int current=(x->buffer[0]=='1');
  if (current!=x->set) {
	t_float f=current;
	outlet_float(x->x_obj.ob_outlet, f);
	x->set=current;
  }
  fclose(x->fs);
}

void overo_gpioin_free(t_overo_gpioin *x)
{
	//
}

void *overo_gpioin_new(t_floatarg f)
{
    t_overo_gpioin *x = (t_overo_gpioin *)pd_new(overo_gpioin_class);
    outlet_new(&x->x_obj, &s_float);
	int nums[]={146,147};
	x->gpioNum=nums[min(1,max(0,((int) f)-1))]; //maps switch number to GPIO number
	post("overo_gpoin: using GPIO(%d): switch %d\n",x->gpioNum,min(2,max(1,(int) f)));
	sprintf(x->inputfile,"/sys/class/gpio/gpio%d/value",x->gpioNum);
	//post("inputfile: %s\n",x->inputfile);

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
	//if (( 
	x->fs = fopen( x->inputfile, "r" );
	//) < 0 )
	//{
	//	error( "Unable to open inputfile" );
	//	exit( 1 );
	//}
	t=fread( x->buffer, sizeof(char), 1, x->fs );
	x->set=(x->buffer[0]=='1');
	fclose(x->fs);
    return (void *)x;
}

void overo_gpioin_setup(void)
{
  overo_gpioin_class = class_new(gensym("overo_gpioin"), (t_newmethod)overo_gpioin_new, (t_method)overo_gpioin_free, sizeof(t_overo_gpioin), 0, A_DEFFLOAT, 0);
  class_addbang(overo_gpioin_class,overo_gpioin_bang);
}
