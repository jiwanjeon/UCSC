#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mathlib.h"

#define OPTIONS "asctel"
#define PI 3.1416

void calSin(){
 printf (" %7.4s %16.8s %16.8s %16.10s\n", "X","Sin","Library","Difference") ;
  double start=-2*PI;
  double end=2*PI;
  for(;start<=end;){
     double result=Sin(start);
     printf (" %7.4lf %16.8lf %16.8lf %16.10lf\n",start,result,sin(start),(sin(start)-result)) ;
    start+=0.1;
   }
}


void calCos(){
printf (" %7.4s %16.8s %16.8s %16.10s\n", "X","Cos","Library","Difference") ;
  double start=-2*PI;
  double end=2*PI;
  for(;start<=end;){
     double result=Cos(start);
	 printf (" %7.4lf %16.8lf %16.8lf %16.10lf\n",start,result,cos(start),(cos(start)-result)) ;
   
	start+=0.1;
   }
}

void calTan(){
  printf (" %7.4s %16.8s %16.8s %16.10s\n", "X","Tan","Library","Difference") ;
  double start=-PI/3;
  double end=PI/3;
  for(;start<=end;){
     double result=Tan(start);
	 printf (" %7.4lf %16.8lf %16.8lf %16.10lf\n",start,result,tan(start),(tan(start)-result)) ;
   	start+=0.1;
   }
}

void calExponential(){
printf (" %7.4s %16.8s %16.8s %16.10s\n", "X","Expo","Library","Difference") ;
  
  double start=1;
  double end=10;
  for(;start<=end;){
     double result=Exp(start);
	 printf (" %7.4lf %16.8lf %16.8lf %16.10lf\n",start,result,exp(start),(exp(start)-result)) ;
	start+=0.1;
   }
}

void calLog(){
 printf (" %7.4s %16.8s %16.8s %16.10s\n", "X","Log","Library","Difference") ;
  double start=1;
  double end=10;
  for(;start<=end;){
     double result=Log(start);
	 printf (" %7.4lf %16.8lf %16.8lf %16.10lf\n",start,result,log(start),(log(start)-result)) ;
	start+=0.1;
   }
}
int main (int argc , char ** argv ) {
  int opt = 0;
  while (( opt = getopt (argc , argv , OPTIONS )) != -1) {
  	switch (opt) {
 	 case 'a':
 		calSin();
 		calCos();
 		calTan();
 		calExponential();
 		calLog();
 		break ;
	 case 's':
	 	calSin();
		 break ;
	 case 'c':
	 	calCos();
		 break ;
	 case 't':
		calTan();
		 break ;
	 case 'e':
		 calExponential();
		 break ;
	 case 'l':
	 	calLog();
		 break ;
        
 	}
   }
 return 0;
}
