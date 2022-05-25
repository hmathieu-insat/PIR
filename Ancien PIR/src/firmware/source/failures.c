/*****************************************************************************/
/* Projectname     :  ENSD-Jammer                                            */
/* Programm        :  Failures			                                     */
/* Controller      :  dsPIC33F                                               */
/* Latest change   :  31.08.2020                                             */
/* Author          :  Gr�goire Chabin/Christian Ringlstetter/Thomas Pichler  */
/*****************************************************************************/

// ### Basic includes ###

	//#include "p33FJ256GP710A.h"
	#include "stdbool.h"
	#include "failures.h"

// ### Programm includes ###
	#include "timer.h"
    #include "synchronization.h"

// ### Variables ###
	
	extern bool CRK_signal;
	extern bool CAM_signal[2];
	
    
	//** Synchronization variables **
	extern unsigned long timer_overflow_CRK;
	extern unsigned long T_TOOTH_RAW;
	extern unsigned int teeth_count_CAM_CRK_synch;
	extern unsigned int teeth_count_CAM_CRK_synch_ahead;
	extern unsigned int teeth_count_CRK;
	extern unsigned int number_real_teeth;
	extern unsigned int number_teeth_between_gaps;
    extern unsigned int number_segments_CRK;
    extern unsigned int first_seg_start_tooth;
	extern int number_teeth_CRK;
    extern int number_gap;
	extern int number_miss_teeth;
	extern int number_teeth_CAM;
	extern double tdc_to_gap;
    extern double first_seg_angle;
	extern double revolution_CRK;
	extern bool engine_start;
    extern bool CRK_synch;
	extern bool CRK_CAM_synch;
	extern bool CAM_CRK_synch_ahead[2];
	extern char active_CAM_edges[2];
    extern int teeth_count_overall;
    
    //** CAM variables **
    extern double filter_time_CAM[2];
    

	//** Timer overflow values for CRK and CAM - failures **
	extern unsigned long timer_overflow_CRK_failure;
	extern unsigned long timer_overflow_CAM_failure;
	extern unsigned long timer_overflow_CAM_REF_CRK;

	//** Failure treatment **
	extern char failure_identify;
    extern int active_cam_failure;

	//** Labels indicating failure status **
	extern bool failure_active;
	extern bool failure_active_CAM_blank_out;
	extern bool failure_set;
    extern bool failure_passed;
    extern bool failure_waiting;

	//** CRK_RUN_OUT **
    extern char sc_type_CRK_RUN_OUT;
    extern unsigned int teeth_counter_CRK_RUN_OUT;
	extern unsigned int angle_to_start_failure_CRK_RUN_OUT;
	extern unsigned int failure_period_CRK_RUN_OUT;
	extern unsigned int counter_TDC0;
	extern int period_counter_CRK_RUN_OUT;
	extern double difference_to_edge_TDC0;
	extern double difference_to_edge_failure_start;
	extern bool detected_TDC0;
	extern bool counter_ready;
	extern bool counter_reset;
	extern bool TDC_on_edge;
	extern bool start_on_edge_CRK_RUN_OUT;

	//** CAM_PER **		
	extern unsigned int number_edges_CAM_PER;
	extern unsigned int counter_CAM_PER[2];
    extern double microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
	extern bool all_active_edges_CAM_PER;
	extern char active_edges_CAM_PER; 

	//** CRK_TOOTH_PER **
	extern unsigned int start_tooth_CRK_TOOTH_PER;
	extern unsigned int number_pulses_CRK_TOOTH_PER;
	extern unsigned int engine_revolutions_CRK_TOOTH_PER;
	extern unsigned int teeth_counter_CRK_TOOTH_PER;
	extern unsigned int revolution_counter_CRK_TOOTH_PER;
	extern long double const milli_second_per_timer_tick_CRK;
	extern double mycrosecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
	extern double calculation_factor_CRK_TOOTH_PER;

	//** CAM_DELAY(CAM_TOOTH_OFF / CAM_REF_CRK / CAM_SYN / CAM_SYN_CRK) **
	extern unsigned int number_processing_edges_CAM_delay[2];
	extern unsigned int shift_counter_CAM_delay[2][6];
	extern const int edges_beetween_shift_CAM_delay;
	extern double angle_to_edge_CAM_delay[2][6];
	extern double delay_angle_CAM_delay;
	extern double delay_factor_CAM_delay;
	extern bool timer_active_CAM_delay[2];
	extern bool interrupt_check_CAM_delay[2];

	//** CAM_REF_CRK **
	extern unsigned int delay_counter_CAM_REF_CRK;								
	extern long double angle_time_to_start_failure_CAM_REF_CRK;			// Value Delay (ms or �CRK)	
	extern double difference_to_edge_failure_start_CAM_REF_CRK;			
	extern char delay_type_CAM_REF_CRK;									// t: time / c: �CRK
   
   
    //** CRK_TOOTH_OFF **
    extern unsigned int number_tooth_off;
    extern unsigned int number_teeth_between_tooth_off;
    extern unsigned int teeth_counter_CRK_TOOTH_OFF;
    extern unsigned int counter_tooth_off;               // counts the number of tooth that were skiped
    extern unsigned int counter_gap_left_CRK_TOOTH_OFF;  // counts the number of gap left befor a full CRK rotation
    
    //** CRK_GAP_NOT_DET **
    extern unsigned int teeth_counter_CRK_GAP_NOT_DET;
    extern unsigned int timer_Counter_CRK_GAP_NOT_DET;  

    //** SEG_ADP_ER_LIM **
    extern unsigned int teeth_counter_SEG_ADP_ER_LIM;
    extern unsigned int timer_Counter_SEG_ADP_ER_LIM; 
    extern double crk_delay_SEG_ADP_ER_LIM;
    
    //** CRK_PLS_ORNG **
    extern double crk_pulse_duration_CRK_PLS_ORNG;

    //** POSN_ENG_STST_LOSS **
    extern unsigned int crk_teeth_off_POSN_ENG_STST;
    extern unsigned int counter_POSN_ENG_STST;

    //** CAM_PAT_ERR **
    extern unsigned int active_CAM_edges_counter[2];
    
    //** SC_CAM_CRK **
    extern unsigned int sc_type_SC_CAM_CRK;


//### Functions ###

//## Output Function CRK
	void Output_CRK(char failure_ident)  //processed at each CRK edge
	{
		switch(failure_ident)
		{
			case '1': 
				{	
					break; 
				}
			case '2': 
				{	
					break; 
				}
			case '3': 
				{
					break; 
				}
			case '4': //CRK_RUN_OUT
				{
					Output_CRK_RUN_OUT();
					break; 
				}
			case '6': //CRK_TOOTH_PER
				{
					Output_CRK_TOOTH_PER();
					
					break; 
				}
			case '7': //CAM_TOOTH_OFF
				{
					CAM_delay_counter(active_cam_failure);
					
					break;
				}
			case '8': //CAM_PAT_ERR
				{			
					break;
				}
			case '9': //CAM_SYN_CRK
				{
                    if(active_cam_failure == 2){
                        CAM_delay_counter (0);	// for CAM1
                        CAM_delay_counter (1);	// for CAM2
                    }
                    else if( active_cam_failure == 1){
                        CAM_delay_counter (1);	// for CAM2                        
                    }
                    else{
                        CAM_delay_counter (0);	// for CAM1
                    }					
					break;
				}
			case 'a': //
				{				
					break;
				}
            case 'b': //
				{
					break;
				}
            case 'f': //
				{
					break; 
				}         
            case 'h': //CRK_TOOTH_OFF
				{
					Output_CRK_TOOTH_OFF();
					break;
				}
            case 'i': //CRK_GAP_NOT_DET
				{
                    Output_CRK_GAP_NOT_DET();
					break;
				}  
            case 'j': //SEG_ADP_ER_LIM
				{
                    Output_SEG_ADP_ER_LIM();
					break;
				}   
            case 'k': //CRK_pulse_duration
				{
                    Output_CRK_pulse_duration();
					break;
				}  
            case 'l': //POSN_ENG_STST
				{
                    Output_POSN_ENG_STST();
					break;
				}     
            case 'm': //SC_CAM_CRK
				{
                    Output_SC_CAM_CRK(active_cam_failure);
					break; 
				}   
			default: 
				break;		
		}
	}
	
//## Output Function CAM
	void Output_CAM(char failure_ident, int cam_id)
	{
        if ((cam_id == active_cam_failure) || active_cam_failure == 2 )
        {
            switch(failure_ident)
            {
                case '0': //NO_FAILURE
                    {
                        Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
                        break; 
                    }	
                case '1': //CRK_NO_SIG
                    {	
                        Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
                        break; 
                    }
                case '2': //CAM_NO_SIG
                    {
                        Output_CAM_no_failure(cam_id);
                        break; 
                    }
                case '3': //CRK/CAM_NO_SIG
                    {
                        Output_CAM_no_failure(cam_id);
                        break; 
                    }
                case '4': //CRK_RUN_OUT
                    {
                        Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
                        break; 
                    }
                case '5': //CAM_PER
                    { 		
                        Output_CAM_no_failure(cam_id);
                        Output_CAM_PER(cam_id);
                        break; 
                    }
                case '6': //CRK_TOOTH_PER
                    {
                        Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
                        break; 
                    }
                case '7': //
                    {				
                        break;
                    }
                case '8': //CAM_PAT_ERR
                    {				
                        Output_CAM_PAT_ERR(cam_id);
                        break;
                    }
                case '9': //CAM_SYN_CRK
                    {				
                        Output_CAM_delay(cam_id);
                        break;
                    }
                case 'a': //
                    {				
                        break;
                    }
                case 'b': //CRK_SHO_LEVEL
                    {				
                        Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
                        break;
                    }
                case 'f': //CRK_TOOTH_NR
                    {
                        Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
                        break; 
                    }
                case 'h': //CRK_TOOTH_OFF
                    {
                        Output_CAM_no_failure(cam_id);
                        break;
                    }
                case 'i': //CRK_Gap_Not_Det
                    {
                        Output_CAM_no_failure(cam_id);
                        break;
                    } 
                case 'j': //SEG_ADP_ER_LIM
                    {
                        Output_CAM_no_failure(cam_id);
                        break;
                    }  
                case 'k': //CRK_pulse_duration
                    {
                        Output_CAM_no_failure(cam_id);
                        break;
                    }  
                case 'l': //POSN_ENG_STST
                    {
                        Output_CAM_no_failure(cam_id);
                        break;
                    }          
                case 'm': //SC_CAM_CRK
                    {
                        Output_SC_CAM_CRK(cam_id);
                        break;
                    }
                default: 
                    break;

            }
        }
        else
        {         
            Output_CAM_no_failure(cam_id); //no failure to be put out on CAM 
        }
	}

//## Output_CRK no Failure Function
	void Output_CRK_no_failure(void)
	{		
		if(CRK_signal == true)
		{
			LATGbits.LATG6 = 1;
		}
		else
		{
			LATGbits.LATG6 = 0;
		}
	}


//## Output_CAM no Failure Function
	void Output_CAM_no_failure(int cam_id) 
	{
		if(CAM_signal[cam_id] == true)
		{
            if(cam_id == 0){
                LATGbits.LATG7 = 1;
            }
            else{
                LATGbits.LATG8 = 1;
            }
		}
		else
		{
			if(cam_id == 0){
                LATGbits.LATG7 = 0;
            }
            else{
                LATGbits.LATG8 = 0;
            }
		}	
	}


//### Output CRK: RUN_OUT ###
	void Output_CRK_RUN_OUT(void)
	{
        if(CRK_signal == true)
        {
            if(teeth_count_CRK != number_teeth_between_gaps)
            { //count the teeths on th rising edge
                teeth_counter_CRK_RUN_OUT = teeth_count_CRK + 1;
            }
            else if(teeth_count_CRK == number_teeth_between_gaps)
            { // if we reched a gap 
                teeth_counter_CRK_RUN_OUT = 1;
            }        
        }
        
        if(failure_active == true)
        { // if failure is active             
            if((teeth_counter_CRK_RUN_OUT * revolution_CRK) >= (failure_period_CRK_RUN_OUT + angle_to_start_failure_CRK_RUN_OUT) )
            {  //once we have reached the failure end
                failure_active = false;
                failure_passed = true;
            }
            else
            {  // if failure is active 
                if(sc_type_CRK_RUN_OUT == 'g'){
                    LATGbits.LATG6 = 0; // set CRK as low
                }
                else if(sc_type_CRK_RUN_OUT == 'b'){
                    LATGbits.LATG6 = 1; // set CRK as hight
                }
            }
        }
        else if(failure_passed == true)
        { // once the failure finished have a normal output 
            if(teeth_count_CRK == number_teeth_between_gaps)
            {
                    failure_passed = false;
            }
            teeth_counter_CRK_RUN_OUT = 1;
            Output_CRK_no_failure();
        }
        else
        {
              
            if((angle_to_start_failure_CRK_RUN_OUT < (teeth_counter_CRK_RUN_OUT * revolution_CRK)) 
                && ((teeth_counter_CRK_RUN_OUT * revolution_CRK) < (angle_to_start_failure_CRK_RUN_OUT + failure_period_CRK_RUN_OUT)))
            { // if we are past the the angle of fairue start and still in the failure period 
               failure_active = true; 
            }
            else{}
            Output_CRK_no_failure();
        }
	}

//### CRK_RUN_OUT_reset ###
	void CRK_RUN_OUT_reset(void)
	{
        failure_active = false;
        failure_passed = false;
		period_counter_CRK_RUN_OUT = 0;
	}


//## Output CAM: CAM_PER
	void Output_CAM_PER(int cam_id)
	{
		if(active_edges_CAM_PER == 'b')
		{
            if(cam_id == 0){ // For CAM1
                T6CONbits.TON = 1; 			// Enable Timer6 
                PR6 = (filter_time_CAM[cam_id] / 2.0 - 5) / microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
            }
            else{ // For CAM2
                T7CONbits.TON = 1; 			// Enable Timer7
                PR7 = (filter_time_CAM[cam_id] / 2.0 - 5) / microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
            }					
		}
		else if(active_edges_CAM_PER == 'f' && CAM_signal[cam_id] == false)
		{
            if(cam_id == 0){ // For CAM1
                T6CONbits.TON = 1; 			// Enable Timer6 
                PR6 = (filter_time_CAM[cam_id] / 2.0 - 5) / microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
            }
            else{ // For CAM2
                T7CONbits.TON = 1; 			// Enable Timer7
                PR7 = (filter_time_CAM[cam_id] / 2.0 - 5) / microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
            }
		}
		else if(active_edges_CAM_PER == 'r' && CAM_signal[cam_id] == true)
		{
            if(cam_id == 0){ // For CAM1
                T6CONbits.TON = 1; 			// Enable Timer6  
                PR6 = (filter_time_CAM[cam_id] / 2.0 - 5) / microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
            }
            else{ // For CAM2
                T7CONbits.TON = 1; 			// Enable Timer7
                PR7 = (filter_time_CAM[cam_id] / 2.0 - 5) / microsecond_per_timer_tick_CAM_PER__CRK_TOOTH_PER;
            }	
		}		
	}

//## CAM_PER_reset
	void CAM_PER_reset(void)
	{
		if(T6CONbits.TON == 1)
		{
			T6CONbits.TON = 0;
			TMR6 = 0;
		}

		if(T7CONbits.TON == 1)
		{
			T7CONbits.TON = 0;
			TMR7 = 0;
		}

        Timer6Init();
        Timer7Init();
		counter_CAM_PER[0] = 0;
        counter_CAM_PER[1] = 0;
	}



//## Output CRK: CRK_TOOTH_PER
	void Output_CRK_TOOTH_PER(void)
	{	
		if(CRK_signal == false)
		{
			T6CONbits.TON = 1; 			// Enable Timer6
		}
	}


//## CRK_TOOTH_PER_reset
	void CRK_TOOTH_PER_reset(void)
	{
		if(T6CONbits.TON == 1)
		{
			T6CONbits.TON = 0;
			TMR6 = 0;
		}

		if(T7CONbits.TON == 1)
		{
			T7CONbits.TON = 0;
			TMR7 = 0;
		}

		failure_set = false;
		failure_active = false;
	}


//## Output CAM_delay: CAM_TOOTH_OFF / CAM_REF_CRK / CAM_SYN / CAM_SYN_CRK
	void Output_CAM_delay (int cam_id)
	{
		if ( failure_identify == '9')
		{
			switch (active_CAM_edges[cam_id])
			{
				case ('b'):
					{
						if(engine_start == true)
						{
							T8CONbits.TON = 1; 							// Enable Timer8 till the first falling CRK_edge
							timer_active_CAM_delay[cam_id] = true;
						}
						else
						{
							Output_CAM_no_failure(cam_id);
						}

						break;		
					}
				case ('f'):
					{	
						if(CAM_signal[cam_id] == false)
						{
							if(engine_start == true)
							{
								T8CONbits.TON = 1; 							// Enable Timer8 till the first falling CRK_edge
								timer_active_CAM_delay[cam_id] = true;

								if(failure_active == false)
								{
									failure_active = true;
								}
							}
							else
							{
								Output_CAM_no_failure(cam_id);
							}					
						}
						else if(CAM_signal[cam_id] == true && failure_active == false)
						{
                            if(cam_id == 0){
                               LATGbits.LATG7 = !LATGbits.LATG7; 
                            }else if(cam_id == 1){
                               LATGbits.LATG8 = !LATGbits.LATG8;
                            }
						}

						break;	
					}
				case ('r'):
					{
						if(CAM_signal[cam_id] == true)
						{
							if(engine_start == true)
							{
								T8CONbits.TON = 1; 							// Enable Timer8 till the first falling CRK_edge
								timer_active_CAM_delay[cam_id] = true;

								if(failure_active == false)
								{
									failure_active = true;
								}
							}
							else
							{
								Output_CAM_no_failure(cam_id);
							}
						}
						else if(CAM_signal[cam_id] == false && failure_active == false)
						{
                            if(cam_id == 0){
                                LATGbits.LATG7 = !LATGbits.LATG7; 
                            }else if(cam_id == 1){
                               LATGbits.LATG8 = !LATGbits.LATG8;
                            }
						}

						break;	
					}
				default:
					{
						Output_CAM_no_failure(cam_id);
						break;	
					}
			}	
		}
		else
		{
			Output_CAM_no_failure(cam_id);
		}
	}


//## CAM_delay: CAM_TOOTH_OFF / CAM_REF_CRK / CAM_SYN / CAM_SYN_CRK
	void CAM_delay (int cam_id)
	{
		if(T4CONbits.TON == 1)
		{
			double former_teeth_time; 
			former_teeth_time = Former_teeth_time_calculation(T_TOOTH_RAW, teeth_count_CRK, number_miss_teeth);
	
			if(((double)TMR4/former_teeth_time)*revolution_CRK >= (revolution_CRK/2.0))
			{
                if(cam_id == 0){
                   LATGbits.LATG7 = !LATGbits.LATG7; 
                }else if(cam_id == 1){
                   LATGbits.LATG8 = !LATGbits.LATG8;
                }
				T4CONbits.TON = 0;
				Timer4Reset();
			}
		}
		

		if(timer_active_CAM_delay[cam_id] == true && number_processing_edges_CAM_delay[cam_id] == 0)
		{
			interrupt_check_CAM_delay[cam_id] = false;
			
			double former_teeth_time; 
			former_teeth_time = Former_teeth_time_calculation(T_TOOTH_RAW, teeth_count_CRK, number_miss_teeth);

			if(((double)TMR8/former_teeth_time) * revolution_CRK >= (delay_angle_CAM_delay * delay_factor_CAM_delay))
			{
				T8CONbits.TON = 0;
				TMR8 = 0;
				timer_active_CAM_delay[cam_id] = false;

				if(interrupt_check_CAM_delay[cam_id] == false)
				{
                    if(cam_id == 0){
                        LATGbits.LATG7 = !LATGbits.LATG7; 
                    }else if(cam_id == 1){
                        LATGbits.LATG8 = !LATGbits.LATG8;
                    }
	
					if(active_CAM_edges[cam_id] == 'r' || active_CAM_edges[cam_id] == 'f')
					{
						T4CONbits.TON = 1;
					}
				}
			}
		}
		else if(number_processing_edges_CAM_delay[cam_id] > 0)
		{
			int i = 0;
			int count = 0;

			double former_teeth_time; 
			former_teeth_time = Former_teeth_time_calculation(T_TOOTH_RAW, teeth_count_CRK, number_miss_teeth);

			for(i = 0; i < edges_beetween_shift_CAM_delay; i++)
			{
				if(count == number_processing_edges_CAM_delay[cam_id])
				{
					break;
				}

				if(shift_counter_CAM_delay[cam_id][i] != 0)
				{
					if(angle_to_edge_CAM_delay[cam_id][i] + ((double)(shift_counter_CAM_delay[cam_id][i] - 1) + ((double)((unsigned long)TMR2 + timer_overflow_CRK*(unsigned long)PR2))/former_teeth_time) * revolution_CRK >= (delay_angle_CAM_delay * delay_factor_CAM_delay))
					{
						shift_counter_CAM_delay[cam_id][i] = 0;
						angle_to_edge_CAM_delay[cam_id][i] = 0;
						number_processing_edges_CAM_delay[cam_id]--;
                        if(cam_id == 0){
                           LATGbits.LATG7 = !LATGbits.LATG7; 
                        }else if(cam_id == 1){
                           LATGbits.LATG8 = !LATGbits.LATG8;
                        }
						
			
						if(active_CAM_edges[cam_id] == 'r' || active_CAM_edges[cam_id] == 'f')
						{
							T4CONbits.TON = 1;
						}

						break;
					}

					count++;
				}
			}
		}		
	}

//## CAM_delay_counter: CAM_TOOTH_OFF / CAM_REF_CRK / CAM_SYN / CAM_SYN_CRK
	void CAM_delay_counter(int cam_id)
	{
		if ((failure_identify == '9') && CRK_signal == false)
		{

			interrupt_check_CAM_delay[cam_id] = true;

			if(timer_active_CAM_delay[cam_id] == true)
			{
				int i = 0;
				int count = 0;
	
				if(number_processing_edges_CAM_delay[cam_id] != 0)
				{
					for(i = 0; i < edges_beetween_shift_CAM_delay; i++) // edges_beetween_shift_CAM_delay = 6
					{
						if(count == number_processing_edges_CAM_delay[cam_id])
						{
							break;
						}
		
						if(shift_counter_CAM_delay[cam_id][i] != 0)
						{
							if(teeth_count_CRK == number_teeth_between_gaps)
							{
								shift_counter_CAM_delay[cam_id][i] = shift_counter_CAM_delay[cam_id][i] + number_miss_teeth + 1;
							}
							else
							{
								shift_counter_CAM_delay[cam_id][i]++;
							}
		
							count++;
						}			
					}
				}
	
				i = 0;
			
				double former_teeth_time; 
				former_teeth_time = Former_teeth_time_calculation_output(T_TOOTH_RAW, teeth_count_CRK, number_miss_teeth);

				for(i = 0; i < edges_beetween_shift_CAM_delay; i++)
				{
					if(shift_counter_CAM_delay[cam_id][i] == 0)
					{
						shift_counter_CAM_delay[cam_id][i] = 1;
						angle_to_edge_CAM_delay[cam_id][i] = ((double)(TMR8)/former_teeth_time)*revolution_CRK;
						T8CONbits.TON = 0;
						TMR8 = 0;
						timer_active_CAM_delay[cam_id] = false;
                        number_processing_edges_CAM_delay[cam_id]++;
						break;
					}	
				}	
			}
			else if(number_processing_edges_CAM_delay[cam_id] != 0)
			{
				int count = 0;
				int i = 0;

				for(i = 0; i < edges_beetween_shift_CAM_delay; i++)
				{
					if(count == number_processing_edges_CAM_delay[cam_id])
					{
						break;
					}
		
					if(shift_counter_CAM_delay[cam_id][i] != 0)
					{
						if(teeth_count_CRK == number_teeth_between_gaps)
						{
							shift_counter_CAM_delay[cam_id][i] = shift_counter_CAM_delay[cam_id][i] + number_miss_teeth + 1;
						}
						else
						{
							shift_counter_CAM_delay[cam_id][i]++;
						}
		
						count++;
					}			
				}
			}
		}
	}


//## CAM_delay reset
	void CAM_delay_reset (void)
	{
		failure_active = false;
		failure_set = false;
		T8CONbits.TON = 0;
		TMR8 = 0;
		T9CONbits.TON = 0;
		Timer9Reset();
		T4CONbits.TON = 0;
		Timer4Reset();		
		number_processing_edges_CAM_delay[0] = 0;
        number_processing_edges_CAM_delay[1] = 0;
		timer_active_CAM_delay[0] = false;
        timer_active_CAM_delay[1] = false;
		delay_counter_CAM_REF_CRK = 0;

		int i;

		for(i = 0; i < edges_beetween_shift_CAM_delay; i++)
		{
			shift_counter_CAM_delay[0][i] = 0;
            shift_counter_CAM_delay[1][i] = 0;
		}
		
		i = 0;

		for(i = 0; i < edges_beetween_shift_CAM_delay; i++)
		{
			angle_to_edge_CAM_delay[0][i] = 0;
            angle_to_edge_CAM_delay[1][i] = 0;
		}
	}



//##Output_CRK_TOOTH_OFF
void Output_CRK_TOOTH_OFF(void){
    
    number_teeth_between_tooth_off = number_teeth_between_gaps / number_tooth_off;

    // only count a tooth when CRK rising edge 
    if(CRK_signal == true){
        if(teeth_count_CRK != number_teeth_between_gaps)
        { 
            teeth_counter_CRK_TOOTH_OFF = teeth_count_CRK + 1;
        }
        else if(teeth_count_CRK == number_teeth_between_gaps)
        {
            teeth_counter_CRK_TOOTH_OFF = 1;
            counter_gap_left_CRK_TOOTH_OFF ++; 
            counter_tooth_off = 0; // reset at every gap
            
            if(counter_gap_left_CRK_TOOTH_OFF == number_gap)
            { // if we made a full rotation
                counter_gap_left_CRK_TOOTH_OFF = 0;
            }
        }        
    }

    if(failure_active == true){
        
        if(CRK_signal == false)
        {   // if failure active and the Crk is set to 0 set Crk output at 1 to miss a tooth
            failure_passed = true;
            LATGbits.LATG6 = 1;  
        }
        else if(failure_passed == true)
        {   // a tooth has been skiped
            failure_passed = false;
            failure_active = false;
            counter_tooth_off ++;
        }

    }
    else{   
        if(((teeth_counter_CRK_TOOTH_OFF % number_teeth_between_tooth_off) == ((number_teeth_between_tooth_off/2) + 1))
        && (CRK_signal == true))
        { //if we passed a tooth and this tooth is separated by the number_teeth_between_tooth_off
            if((counter_gap_left_CRK_TOOTH_OFF == 0) && (counter_tooth_off < ((number_tooth_off / number_gap) + (number_tooth_off % number_gap))))
            {
                failure_active = true;    
            }
            else if(counter_tooth_off < (number_tooth_off / number_gap))
            {
                failure_active = true;    
            }
            else
            {
                Output_CRK_no_failure();
            }
 
        }
        else{}
        
    }
}

//##CRK_TOOTH_OFF_reset
void CRK_TOOTH_OFF_reset(void)
{
    failure_active = false;
    number_teeth_between_tooth_off = 0;
}

//##Output_CRK_GAP_NOT_DET
void Output_CRK_GAP_NOT_DET(void){
    
    if(CRK_signal == true)
    {
        if(teeth_count_CRK != number_teeth_between_gaps)
        { 
            teeth_counter_CRK_GAP_NOT_DET = teeth_count_CRK + 1;    
        }
        else if(teeth_count_CRK == number_teeth_between_gaps)
        {
            teeth_counter_CRK_GAP_NOT_DET = 1;
        } 
    }
 
    if(CRK_synch == false)
    {
        LATGbits.LATG6 = 1;      
    }
    else if(CRK_synch == true)
    {
        Output_CRK_no_failure();  
        
        if((teeth_counter_CRK_GAP_NOT_DET == number_teeth_between_gaps ) && failure_active == false)
        {
            failure_active = true;
            PR8 = T_TOOTH_RAW * 1.5 ; // to be in the midle of the gap 

        }
        else if((failure_active == true) && (CRK_signal == true))
        {
            T8CONbits.TON = 1;  
        }
    }  
}

//## CRK_GAP_NOT_DET_reset
void CRK_GAP_NOT_DET_reset(void){
    failure_active_CAM_blank_out = false;
    failure_active = false;      
    Timer8Init();
}    

//## Output_SEG_ADP_ER_LIM
void Output_SEG_ADP_ER_LIM(void){
    if(CRK_signal == true)
    {
        if(teeth_count_CRK != number_teeth_between_gaps)
        { 
            teeth_counter_SEG_ADP_ER_LIM = teeth_count_CRK + 1;
        }
        else if(teeth_count_CRK == number_teeth_between_gaps)
        {
            teeth_counter_SEG_ADP_ER_LIM = 1;
        } 
    }
    
    if((failure_active == true) && (CRK_signal == false))
    {  // on the falling edge of the CRK start the delay timer
        T7CONbits.TON = 1;
        LATGbits.LATG6 = 1; 

    }
    else if((failure_active == true) && (CRK_signal == true))
    {
        if(failure_passed == true)
        {  // if failure on the falling edge of the CRK is allready set
            LATGbits.LATG6 = 0;
            T7CONbits.TON = 1;
        }
        else
        {  // if failure on the falling edge of the CRK is still not set, this happens a lot on lower frquency
            failure_waiting = true;
        }
    }
    else{
        Output_CRK_no_failure();   
    }
    
    if ( teeth_counter_SEG_ADP_ER_LIM == (( (720 / (number_segments_CRK * revolution_CRK) )  + (first_seg_angle / revolution_CRK )+ (int)(tdc_to_gap / revolution_CRK) + 1)))
    { // if we are one tooth befor the end of the first segment
        double delayTimer = crk_delay_SEG_ADP_ER_LIM * (T_TOOTH_RAW/ revolution_CRK);
        PR7 =  delayTimer * 8; 
        // set de delay for the timer, times 8 because the timer 7 has a prescale of 1:8 and T_TOOTH_RAW has 1:64
        failure_active = true;
    }
}

//## SEG_ADP_ER_LIM_reset
void SEG_ADP_ER_LIM_reset(void){
    failure_active = false;
    failure_passed = false;
    failure_waiting = false;
    Timer7Init();
    Timer8Init();
    timer_Counter_SEG_ADP_ER_LIM = 0;
}    

//## Output_CRK_pulse_duration
void Output_CRK_pulse_duration(void){
    if(CRK_signal == false){
        T7CONbits.TON = 1;  //start the timer 
        LATGbits.LATG6 = 0;  
    }
    else{}
}

//## CRK_PLS_ORNG_reset
void CRK_pulse_duration_reset(void){
    Timer7Init();
    failure_active = false;
} 

//## Output_POSN_ENG_STST
void Output_POSN_ENG_STST(void){
    
    if((CRK_signal == false) && (failure_active == true)){
        counter_POSN_ENG_STST ++; // counte the teeth
        LATGbits.LATG6 = 1; // skip the tooth
        
        if(counter_POSN_ENG_STST >= crk_teeth_off_POSN_ENG_STST)
        { // if number of teeth skip is done stop failure 
            failure_active = false;
            counter_POSN_ENG_STST = 0;
        }
    }
    else if (failure_active == false){
    Output_CRK_no_failure();
    }
}

//## POSN_ENG_STST_LOSS_reset
void POSN_ENG_STST_reset(void){
    failure_active = false;
}

    
 //## Output_CAM_PAT_ERR    
    
void Output_CAM_PAT_ERR(int cam_id){
    switch(active_CAM_edges[cam_id])
	{
        case 'b':
        {	
            active_CAM_edges_counter[cam_id] ++; 
            if(active_CAM_edges_counter[cam_id] >= 7 )
            { // after an abitrary number of CAM edges skip the next falling edge to generate the failure
                if (CAM_signal[cam_id] == false)
                {
                    if(cam_id == 0){
                        LATGbits.LATG7 = 1;
                    }
                    else{
                       LATGbits.LATG8 = 1; 
                    }                 
                } 
                else if (CAM_signal[cam_id] == true)
                {
                    if(cam_id == 0){
                        LATGbits.LATG7 = 0;
                    }
                    else{
                       LATGbits.LATG8 = 0; 
                    }           
                }   
                active_CAM_edges_counter[cam_id] = 0;                
            }
            else
            {
                Output_CAM_no_failure(cam_id);
            }
            break;
        }
        case 'f':
        {	
            if (CAM_signal[cam_id] == false)
            {
                active_CAM_edges_counter[cam_id] ++;
                if(active_CAM_edges_counter[cam_id] >= 7)
                { // after an abitrary number of CAM edges skip this edge in output to generate the failure
                    active_CAM_edges_counter[cam_id] =0; 
                    if(cam_id == 0){
                        LATGbits.LATG7 = 1;
                    }
                    else{
                       LATGbits.LATG8 = 1; 
                    }   
                }
                else
                {
                    Output_CAM_no_failure(cam_id);
                }
            }             
            break;
        }
        case 'r':
        {	
            if (CAM_signal[cam_id] == true)
            {
                active_CAM_edges_counter[cam_id] ++;
                if(active_CAM_edges_counter[cam_id] >= 7)
                { // after an abitrary number of CAM edges skip this edge in output to generate the failure
                    active_CAM_edges_counter[cam_id] =0; 
                    if(cam_id == 0){
                        LATGbits.LATG7 = 0;
                    }
                    else{
                       LATGbits.LATG8 = 0; 
                    } 
                }
                else
                {
                    Output_CAM_no_failure(cam_id);
                }
            }    
            break;
        }
    }
}
    
 //## CAM_PAT_ERR_reset   
    
void CAM_PAT_ERR_reset(){
    active_CAM_edges_counter[0] = 0; // reset counter for CAM1
    active_CAM_edges_counter[1] = 0; // reset counter for CAM2
    Output_CAM_no_failure(0); // reset CAM1
    Output_CAM_no_failure(1); // reset CAM2
}


//## Output_SC_CAM_CRK
void Output_SC_CAM_CRK(int cam_id){
    
    if(failure_active == true)
    {
        switch(sc_type_SC_CAM_CRK)
        {
            case(1): // CrkScg
            {
                LATGbits.LATG6 = 0;  
                Output_CAM_no_failure(cam_id); 
                break;
            }
            case(2): // CrkScb
            {
                LATGbits.LATG6 = 1;  
                Output_CAM_no_failure(cam_id);
                break;
            }
            case(3): // CamScg
            {
                if(cam_id == 0){
                    LATGbits.LATG7 = 0;
                }
                else{
                   LATGbits.LATG8 = 0; 
                }  
                Output_CRK_no_failure();
                break;
            }
            case(4): // CamScb
            {
                if(cam_id == 0){
                    LATGbits.LATG7 = 1;
                }
                else{
                   LATGbits.LATG8 = 1; 
                } 
                Output_CRK_no_failure();
                break;
            }                                    
        }
    }
    else if (failure_active == false){
    Output_CRK_no_failure();
    Output_CAM_no_failure(cam_id);
    }
}

//## POSN_ENG_STST_LOSS_reset
void SC_CAM_CRK_reset(void){
    failure_active = false;
    Output_CRK_no_failure();
    Output_CAM_no_failure(0); // reset CAM1
    Output_CAM_no_failure(1); // reset CAM2
}


//### Failure Processing ###
	void Failure_processing(char failure_ident)
	{
		switch(failure_ident)
		{
			case '1':
				{	
					break;
				}
			case'2':
				{
					break;
				}
			case'3':
				{
					break;
				}
			case '4':
				{
					//CRK_RUN_OUT();
					break;
				}
			case '7':
				{		
					break;
				}
			case '8':
				{	
					break;
				}
			case '9': // Cam delay
				{
                    if(active_cam_failure == 2){
                        CAM_delay (0);	// for CAM1
                        CAM_delay (1);	// for CAM2
                    }
                    else if( active_cam_failure == 1){
                        CAM_delay (1);	// for CAM2                        
                    }
                    else{
                        CAM_delay (0);	// for CAM1
                    }
					break;
				}
			case 'a':
				{			
					break;
				}
//            case 'b': //
//				{			
//					break;
//				}
            case 'f':
				{
					break;
				}
            case 'h':
				{
					break;
				} 
            case 'i':
				{
					break;
				} 
            case 'j':
				{
					break;
				}      
            case 'k':
				{
					break;
				}
            case 'l':
				{
					break;
				} 
            case 'm':
				{
					break;
				} 
			default:
				{
					break;
				}
		}
	}


//### Failure Reset ###
	void Failure_reset(void)
	{
   	}

//### Failure Synchronization Reset ###
	void Failure_synch_reset(char failure_ident)
	{
		switch(failure_ident)
		{	
			case '4':
				{
					//CRK_RUN_OUT
					CRK_RUN_OUT_reset();
                    break;
				}
			case '6':
				{
					//CRK_TOOTH_PER
					CRK_TOOTH_PER_reset();
                    break;
				}
			case '7':
				{
					//CAM_DELAY(CAM_TOOTH_OFF)
					CAM_delay_reset();
                    break;
				}
            default:
                break;
            
		}
	}

//### Former teeth time calculation ###
	double Former_teeth_time_calculation(long t_tooth, unsigned int teeth_count, int numb_miss_teeth)
	{
		double form_teeth_time;
		
		form_teeth_time = (double)t_tooth;

		if(teeth_count == 1)
		{
			form_teeth_time = form_teeth_time/(double)(numb_miss_teeth + 1);
		}		

		return(form_teeth_time);
	}

//### Former teeth time calculation output ###
	double Former_teeth_time_calculation_output(long t_tooth, unsigned int teeth_count, int numb_miss_teeth)
	{
		double form_teeth_time;
		
		form_teeth_time = (double)t_tooth;

		if(teeth_count == number_teeth_between_gaps)
		{
			form_teeth_time = form_teeth_time/(double)(numb_miss_teeth + 1);
		}		

		return(form_teeth_time);
	}
