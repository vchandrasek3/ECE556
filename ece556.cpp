// ECE556 - Copyright 2014 University of Wisconsin-Madison.  All Rights Reserved.

#include "ece556.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <cmath.h>
#define LINE_SIZE 256
int readBenchmark(const char *fileName, routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/  
  int blockages = 0;
  FILE *fp;
  char *line;
  int line_no=0;
  char *field;
  line = new char (LINE_SIZE);
  fp = fopen (fileName,"r");
  fgets(line,LINE_SIZE,fp);


    field = strtok(line, " \n");
    field = strtok(NULL, " \n");
	rst->gx = atoi(field);
    field = strtok(NULL, " \n");
	rst->gy = atoi(field);
    printf("%d,%d\n",rst->gx,rst->gy);

     fgets(line,LINE_SIZE,fp);
     field = strtok(line, " \n");
     field = strtok(NULL, " \n");
	rst->cap = atoi(field);
     printf("%d\n",rst->cap);

     fgets(line,LINE_SIZE,fp);
     field = strtok(line, " \n");
     field = strtok(NULL, "  ");
     field = strtok(NULL, "  ");
	rst->numNets = atoi(field);
     printf("%d\n",rst->numNets);

     net net_array[rst->numNets];
     rst->nets = net_array;
     int net_count=0;
     int pin_count=0;
	for (net_count=0;net_count<rst->numNets;net_count++)
		{
		  fgets(line,LINE_SIZE,fp);
		 
		  field = strtok(line, "n \n");
		  net_array[net_count].id = atoi(field);
		
		printf("net id:%d",net_array[net_count].id);
		field = strtok(NULL, " \n");
		net_array[net_count].numPins = atoi(field);	
   		point pin_loc[net_array[net_count].numPins];
		net_array[net_count].pins = pin_loc;
		printf(" pin count:%d\n",net_array[net_count].numPins);
		for (pin_count=0;pin_count<net_array[net_count].numPins;pin_count++)
			{
			fgets(line,LINE_SIZE,fp);
			field = strtok(line, " \n");
			pin_loc[pin_count].x = atoi(field);
			printf("x:%d ",pin_loc[pin_count].x);
			field = strtok(NULL, " \n");
			pin_loc[pin_count].y = atoi(field);
			printf("y:%d\n",pin_loc[pin_count].y);
			}
		}
   

int *bx = new int (blockages*2);
int *by = new int (blockages*2);
int *bc = new int (blockages);
fgets(line,LINE_SIZE,fp);
field = strtok(line, "n \n");
blockages = atoi(field);
printf("blockages:%d",blockages);

int loop1,loop2;

for (loop1=0;loop1<blockages;loop1++)
	{
	fgets(line,LINE_SIZE,fp);
	field = strtok(line, " \n");
		
		  *(bx + loop1*2) = atoi(field);
		  printf("\nbx:%d ",*(bx + loop1*2));
		  
		  field = strtok(NULL, " \n");
		  *(by + loop1*2) = atoi(field);
		   printf("by:%d ",*(by + loop1*2));
		  

		 field = strtok(NULL, " \n");
		  *(bx + loop1*2 + 1) = atoi(field);
		  printf("\nbx:%d ",*(bx + loop1*2 +1));
		 

		field = strtok(NULL, " \n");
		*(by + loop1*2 + 1) = atoi(field);
		   printf("by:%d ",*(by + loop1*2 + 1));
		

		field = strtok(NULL, " \n");
		*(bc+loop1) = atoi(field);
		printf("bc:%d\n",*(bc + loop1));
		
	}

//calculating the number of edges
rst->numEdges =  rst->gy*(rst->gx-1) + rst->gx*(rst->gy-1);

//populating the capacities of the edges
rst->edgeCaps = new int (rst->numEdges);
for (loop1=0;loop1<rst->numEdges;loop1++)
	{
	*(rst->edgeCaps+loop1) = 1;
	printf("edgecaps %d ",*(rst->edgeCaps+loop1));
	}
	printf("\n");

//overwriting the blockages with the new values
for (loop1=0;loop1<blockages;loop1++)
{
if ( (*(bx+loop1*2) == *(bx+loop1*2+1)) && (*(by+loop1*2) != *(by+loop1*2+1)) )
	{
	//printf("its a column edge to change"); 
	   if (*(by+loop1*2)<*(by+loop1*2+1))
		{
		//rst->edgeCaps[((*(bx+loop1*2))*(rst->gy-1)) + (*(by+loop1*2))] = *(bc+loop1);
		rst->edgeCaps[(rst->gy*(rst->gx-1)) + (*(bx+loop1*2)) + (rst->gx * (*(by+loop1*2)) ) ]  = *(bc+loop1);
		}
	   else
		{
		//rst->edgeCaps[((*(bx+loop1*2+1))*(rst->gy-1)) + (*(by+loop1*2+1))] = *(bc+loop1);
		rst->edgeCaps[(rst->gy*(rst->gx-1)) + (*(bx+loop1*2+1)) + (rst->gx * (*(by+loop1*2+1)) ) ]  = *(bc+loop1);
		}

	}

else
	{
	//printf("its a row edge to change");
	   if (*(bx+loop1*2)<*(bx+loop1*2+1))
		{
		//rst->edgeCaps[(rst->gx*(rst->gy-1)) + (*(by+loop1*2)) + (rst->gy * (*(bx+loop1*2)) ) ]  = *(bc+loop1);
		rst->edgeCaps[((*(by+loop1*2))*(rst->gx-1)) + (*(bx+loop1*2))] = *(bc+loop1);
		} 
	   else
		{
		//rst->edgeCaps[(rst->gx*(rst->gy-1)) + (*(by+loop1*2+1)) + (rst->gy * (*(bx+loop1*2+1)) ) ]  = *(bc+loop1);
		rst->edgeCaps[((*(by+loop1*2+1))*(rst->gx-1)) + (*(bx+loop1*2+1))] = *(bc+loop1);
		}

	}


}

for (loop1=0;loop1<rst->numEdges;loop1++)
	{
	printf("edgecaps %d ",*(rst->edgeCaps+loop1));
	
	}
return 1;	
}

int solveRouting(routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/
 route net_route[rst->numNets];
//printf("TEST!!!!!");
 //route *net_route ;
//net_route=new route [rst->numNets];
 //rst->nets[net_count].nroute=net_route[net_count]; // check or *(rst.nets+net_count)? 
 int net_count;
	printf("TEST!!!!!");
 for(net_count=0; net_count < rst->numNets; net_count++)
	{
	net_route[net_count].numSegs=rst->nets[net_count].numPins - 1; // consider 2 pins of the net for a segment. So, segments=pins-1 
	printf("For net %d, pins = %d, segments = %d ",net_count,(rst->nets[net_count]).numPins,net_route[net_count].numSegs);
	point net_pin[(rst->nets[net_count]).numPins];
	
	int pin_count;
	for(pin_count=0;pin_count < rst->nets[net_count].numPins; pin_count++)
		{ 
		net_pin[pin_count].x=rst->nets[net_count].pins[pin_count].x;
		net_pin[pin_count].y=rst->nets[net_count].pins[pin_count].y;
		printf("For net%d, pin%d is [%d,%d]",net_count, pin_count, net_pin[pin_count].x, net_pin[pin_count].y);
		}
		
		segment net_segment[net_route[net_count].numSegs];
		
		int seg_count;
		for(seg_count=0;seg_count < net_route[net_count].numSegs ; seg_count++)
			{
			point P1,P2;
			P1.x=net_pin[seg_count].x;
			P1.y=net_pin[seg_count].y;
			P2.x=net_pin[seg_count+1].x;
			P2.y=net_pin[seg_count+1].y;
			net_segment[seg_count].p1=P1;
			net_segment[seg_count].p2=P2;
			
			printf("For segment%d, P1=[%d,%d], P2=[%d,%d]", net_count, seg_count, P1.x,P1.y,P2.x,P2.y);
			net_segment[seg_count].numEdges= abs(P1.x - P2.x) + abs(P1.y - P2.y);
			printf("For segment%d, numEdges=",seg_count,net_segment[seg_count].numEdges);
			int *edge_id = new int (net_segment[seg_count].numEdges);
			
			// int edge_id[net_segment[seg_count].numEdges]   ; net_segment[seg_count].edges=edge_id;
			
			
			if(P1.x > P2.x)
				swap(P1.x,P2.x); // make sure P1.x<P2.x, coz we traverse from P1 to p2
				
			if(P1.y > P2.y)
				swap(P1.y,P2.y); // make sure P1.y<P2.y, coz we traverse from P1 to p2
				
			printf("For segment%d, after swapping P1=[%d,%d], P2=[%d,%d]", net_count, seg_count, P1.x,P1.y,P2.x,P2.y);
			
			int edge_count=0; int xval; int yval;
					
			for(xval=P1.x; xval < P2.x ; xval++)
				{
				*(edge_id+edge_count)=RowEdgeId(xval,P1.y, rst->gx, rst->gy);
				edge_count++;
				}
					
			for(yval=P1.y; yval < P2.y ; yval++)
				{
				*(edge_id+edge_count)=ColEdgeId(P2.x, yval, rst->gx, rst->gy);
				edge_count++;
				}
			
			printf("Edge Ids for segment%d are: ",seg_count);
			for(edge_count=0; edge_count< net_segment[seg_count].numEdges; edge_count++)
				{
				printf("%d",*(edge_id+edge_count));
				}
				
			net_route[net_count].segments=net_segment;
			}
			
		rst->nets[net_count].nroute=net_route[net_count];
	}
	

	return 1;
}

void swap(int a,int b){
int temp;
temp=a;
a=b;
b=temp;
printf("after swapping the value for a & b is : %d %d",a,b);
}

int RowEdgeId(int x, int y, int gx, int gy ){
return ( y*(gx-1) + x);
}

int ColEdgeId(int x, int y, int gx, int gy ){
return ( (gx-1)*(gx-1) + y*(gy-1) +x);
}


int writeOutput(const char *outRouteFile, routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/
  ofstream out_stream(outRouteFile);
	if (!out_stream{
		cout << "Unable to open the file" << endl;
		out_stream.close();
		return 0;
	}
	for (int i = 0; i < rst->numNets; i++){
		out_stream << "n" << rst->nets[i].id << endl;
		for (int j = 0; j < rst->nets[i].nroute.numSegs; j++){
			segment seg = rst->nets[i].nroute.segments[j];
			out_stream << "(" << seg.p1.x << "," << seg.p1.y << ")-";
			out_stream << "(" << seg.p2.x << "," << seg.p2.y << ")" << endl;
		}
		out_stream << "Done!" << endl;
	}

	out_stream.close();
	return 1;

}


int release(routingInst *rst){
  /*********** TO BE FILLED BY YOU **********/
rst->cap = 0;
	rst->numEdges = 0;
	rst->gx = 0;
	rst->gy = 0;
	
	delete [] rst->edgeCaps;
	rst->edgeCaps = NULL;
	
	delete [] rst->edgeUtils;
	rst->edgeUtils = NULL;
	
	for (int i = 0; i < rst->numNets; i++){
		delete [] rst->nets[i].nroute->segments;
		delete [] rst->nets[i].pins;
	}
	
	rst->numNets = 0;
	rst->nets = NULL;
	return 1;
 }
  


