/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 25-Jul-2022
 Filename		: ir_capture.c
 Purpose		: IR capturing
 Description		: IR capturing using GPIO 
 Author(s)		: Shubhadeep Maity
 Email			: shubhadeep@cdac.in
    
 See LICENSE for license details.
******************************************************************************/  
      
  #include "stdlib.h"
  #include "config.h"
  #include "gpio.h"
  //# define LENGTH 105 
  
  void main()
  {
  
  
          unsigned int signal_state=0;                                      
          unsigned int time_high[210]={0,}, time_low[210]={0,},i=0;
          unsigned int arr_high[210], arr_low[210];
          unsigned int start=0;
          unsigned long time=0;
          
          
           while(1){                                        //Waiting for first remote signal
  
                          signal_state=GPIO_read_pin(4);   // gpio 4 being read and assigned to signal_state
                          if(signal_state == 0){          // signal_state == 0, no signal
                          	    start = get_time();   // time using get_time, assigned to start
                          	    time_low[i]= 0;      // ith low time is being initialized with 0, where i=0.
                                    break;             // will come out of loop
                          }
                  }
          //int a=0;  
          unsigned int b=0;
          while(i<105){								// i value depends on the length of bits stream.
          
          
          		 /*  if(i!=0)                                              // when i is not 0
          		    	time_low[i]= get_time() - time_high[i-1] - start;  // ith low time is get_time - time_high[i-1] - start
          		   else						    // when i is 0
          		   	time_low[i]= get_time() - start;                  // ith time_low = get_time - start
          		   	
          		 	time = (time_low[i]*0.000013);                   // time = ith time_low * multiplication factori
          		   
                          
			     printf("time_low[%d]:%d, %ld\n",i,time_low[i], time);

			     printf("time_low start:%d\n", get_time());*/
		   
				time_low[b]=get_time();                       //storing the time of low pulse in an array.

                           while(1){
  
                                  signal_state=GPIO_read_pin(4);
                                  if(signal_state ==1){                    // signal_state change to high.
                                  break;
                                  }
                           }
			

			   //time_high[i]= get_time()-time_low[i]  - start;                          
                          
                         // time = ( time_high[i]*0.000013);
                          
                         // printf("time_high[%d]:%d, %ld\n",i,time_high[i],time);

			//printf("time_high :%d\n", get_time());
		   		time_high[b]=get_time();		//storing the time of high pulse in an array.
			                      
                           while(1){
  
                                  signal_state=GPIO_read_pin(4);
                                  if(signal_state == 0){	     //signal_state == 0, no signal
                                  break;
                                  }
                           }  
					
                           

                           i++;				  //incrementing i
			   //a++;
			    b++;				 //incrementing b
			  //j++;
			  //printf("i=%d\n",i);
 
  		}       
		
			time_low[b]=get_time();

                           while(1){
  
                                  signal_state=GPIO_read_pin(4);
                                  if(signal_state ==1){
                                  break;
                                  }
                           }
                           
                        time_high[b]=get_time();
			
			/*for(a=0;a<105;a++){
			                printf("time_low[%d]:%d\n\n",a,time_low[a]);
					//puts("");
					printf("time_high[%d]:%d\n\n",a,time_high[a]);
					puts("======================");*/
					
					
		for(b=0;b<105;b++){
					  //puts("arr_high={");
					  /*printf("time_low[%d]:%d\n\n",b,time_low[b]);
					  printf("time_high[%d]:%d\n",b,time_high[b]);*/
					  
					  
					  unsigned int low=((time_high[b] - time_low[b])*0.013);         //count of low pulses (multiplication factor is based on operating frequency of the board) 
					  unsigned int high=((time_low[b+1] - time_high[b])*0.013);     //count of high pulses.
					  
					  printf("low=%d\n\n",low);                                    
					  printf("high=%d\n\n",high);
					  
					  arr_low[b]=low;                                            //counts are storing in an array.
					  arr_high[b]=high;
			                 
					  


		        }
			  printf("low[]={");
			  for(int a=0; a<106;a++){			   //loop count is 1 more than the previous loop to capture the last low bit.
				  printf("%d,",arr_low[a]);              //printing count values of low pluses in form of an array.
				  	  
				   
			  }
				  printf("};\n");
			  
			  
			  
			  printf("high[]={");                   
			  for(int c=0; c<106;c++){                     
			  	  printf("%d,",arr_high[c]);           //printing count values of high pluses in form of an array.
				  
			  }
				  printf("};\n");
			/*for(b=0;b<105;b++){
					  //puts("arr_high={");
					  arr_high[b]=time_high[a];
					  printf("arr_high[%d]={%d,}\n",b,arr_high[b]);
					  //puts("}");
					  
					  //puts("arr_low={");
					  arr_low[b]=time_low[a];
					  printf("arr_low[%d]={%d,}\n",b,arr_low[b]);
					  //puts("}");
			}*/
			b++;
}
  
