#==================================================================================#
# Author: GWX Technology
# Attribution: Plain Text
# Birthday: Sun Nov 12 15:21:19 CST 2023
# Organization: GWX Technology
# Copyright: GWX Technology ©2023 GWX Technology Inc. All rights reserved.
#----------------------------------------------------------------------------------#
# Description:
# All the data in the file was generated by GWX Technology. This information was
# prepared only for EDA tools training. GWX Technology does not guarantee the
# accuracy or completeness of the information contained herein. GWX Technology
# shall not be liable for any loss or damage of any kind arising from the use of
# this document or the information contained herein.
#----------------------------------------------------------------------------------#
# Version: 0.9.0.0 Alpha
#==================================================================================#

/*   --------------------------------------------------------------   */
/*                       Template Revision : 5.1.0                    */
/*   --------------------------------------------------------------   */
/*                      * Tessent MemoryBIST Model *                  */

   MemoryTemplate ( RF_2PUHD_128x72 ) {
   CellName:      RF_2PUHD_128x72;
   MemoryType:    SRAM;

   LogicalPorts:  1R1W;
   NumberofWords:  128;
   NumberofBits :  72;
   Algorithm:  SMarchCHKBvcd ;
   BitGrouping : 1;
   OperationSet: SyncWRvcd;
   MinHold:  0.519;
   MilliWattsPerMegaHertz:  4.669e-03;
   ShadowRead: Off;
   WriteOutOfRange: Off;

/* Scrambling Information */ 
   AddressCounter { 
     Function (Address) { 
       LogicalAddressMaP { 
         ColumnAddress[0] : Address [0]; 
           BankAddress[0] : Address [3]; 
         RowAddress[1:0] : Address [2:1];
         RowAddress[4:2] : Address [6:4];
       } 
     } 
     Function (Rowaddress) { 
       CountRange [0:31]; 
     } 
     Function (Columnaddress) { 
       CountRange [0:1]; 
     } 
   } 


Port (  RSCOUT )
             {   
               Direction: OUTPUT;
               Function: BisrSerialData;
             }
Port (  QB[71:0] )
             {   
               Direction: OUTPUT;
               LogicalPort: B;
               Function: data;
             }
Port (  ADRA[6:0] )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: Address;
             }
Port (  DA[71:0] )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: data;
             }
Port (  WEA )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: WriteEnable;
               Polarity: ActiveHigh;
             }
Port (  MEA )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: Select;
               Polarity: ActiveHigh;
             }
Port (  CLK )
             {   
               Direction: INPUT;
               Function: Clock;
               Polarity: ActiveHigh;
             }
Port (  RSCIN )
             {   
               Direction: INPUT;
               Function: BisrSerialData;
             }
Port (  RSCEN )
             {   
               Direction: INPUT;
               Function: BisrScanEnable;
             }
Port (  RSCRST )
             {   
               Direction: INPUT;
               Function: BisrReset;
               Polarity: ActiveHigh;
             }
Port (  RSCLK )
             {   
               Direction: INPUT;
               Function: BisrClock;
             }
Port (  FISO )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  RME )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  RM[3:0] )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 4'b0010;
             }
Port (  TEST_RNM )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  LS )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  BC1 )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  BC2 )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  ADRB[6:0] )
             {   
               Direction: INPUT;
               LogicalPort: B;
               Function: Address;
             }
Port (  MEB )
             {   
               Direction: INPUT;
               LogicalPort: B;
               Function: Select;
               Polarity: ActiveHigh;
             }
Port (  TEST1 )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  TESTRWM )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
RedundancyAnalysis { 
	ColumnSegment (Right) { 
		ShiftedIORange: QB[71:36]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[4:0] { 
				ShiftedIO(QB[36]):   5'b00000; 
				ShiftedIO(QB[37]):   5'b00000; 
				ShiftedIO(QB[38]):   5'b00001; 
				ShiftedIO(QB[39]):   5'b00001; 
				ShiftedIO(QB[40]):   5'b00010; 
				ShiftedIO(QB[41]):   5'b00010; 
				ShiftedIO(QB[42]):   5'b00011; 
				ShiftedIO(QB[43]):   5'b00011; 
				ShiftedIO(QB[44]):   5'b00100; 
				ShiftedIO(QB[45]):   5'b00100; 
				ShiftedIO(QB[46]):   5'b00101; 
				ShiftedIO(QB[47]):   5'b00101; 
				ShiftedIO(QB[48]):   5'b00110; 
				ShiftedIO(QB[49]):   5'b00110; 
				ShiftedIO(QB[50]):   5'b00111; 
				ShiftedIO(QB[51]):   5'b00111; 
				ShiftedIO(QB[52]):   5'b01000; 
				ShiftedIO(QB[53]):   5'b01000; 
				ShiftedIO(QB[54]):   5'b01001; 
				ShiftedIO(QB[55]):   5'b01001; 
				ShiftedIO(QB[56]):   5'b01010; 
				ShiftedIO(QB[57]):   5'b01010; 
				ShiftedIO(QB[58]):   5'b01011; 
				ShiftedIO(QB[59]):   5'b01011; 
				ShiftedIO(QB[60]):   5'b01100; 
				ShiftedIO(QB[61]):   5'b01100; 
				ShiftedIO(QB[62]):   5'b01101; 
				ShiftedIO(QB[63]):   5'b01101; 
				ShiftedIO(QB[64]):   5'b01110; 
				ShiftedIO(QB[65]):   5'b01110; 
				ShiftedIO(QB[66]):   5'b01111; 
				ShiftedIO(QB[67]):   5'b01111; 
				ShiftedIO(QB[68]):   5'b10000; 
				ShiftedIO(QB[69]):   5'b10000; 
				ShiftedIO(QB[70]):   5'b10001; 
				ShiftedIO(QB[71]):   5'b10001; 
			} 
		} 
		PinMap { 
			SpareElement { 
				RepairEnable: RepairRegister[0]; 
                                FuseMap[0]: RepairRegister[1]; 
                                FuseMap[1]: RepairRegister[2]; 
                                FuseMap[2]: RepairRegister[3]; 
                                FuseMap[3]: RepairRegister[4]; 
                                FuseMap[4]: RepairRegister[5]; 
				LogicLow  : RepairRegister[6]; 
				LogicLow  : RepairRegister[7]; 
				LogicLow  : RepairRegister[8]; 
			} 
		} 
	} 
	ColumnSegment (Left) { 
		ShiftedIORange: QB[35:0]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[4:0] { 
				ShiftedIO(QB[0]):   5'b00000; 
				ShiftedIO(QB[1]):   5'b00000; 
				ShiftedIO(QB[2]):   5'b00001; 
				ShiftedIO(QB[3]):   5'b00001; 
				ShiftedIO(QB[4]):   5'b00010; 
				ShiftedIO(QB[5]):   5'b00010; 
				ShiftedIO(QB[6]):   5'b00011; 
				ShiftedIO(QB[7]):   5'b00011; 
				ShiftedIO(QB[8]):   5'b00100; 
				ShiftedIO(QB[9]):   5'b00100; 
				ShiftedIO(QB[10]):   5'b00101; 
				ShiftedIO(QB[11]):   5'b00101; 
				ShiftedIO(QB[12]):   5'b00110; 
				ShiftedIO(QB[13]):   5'b00110; 
				ShiftedIO(QB[14]):   5'b00111; 
				ShiftedIO(QB[15]):   5'b00111; 
				ShiftedIO(QB[16]):   5'b01000; 
				ShiftedIO(QB[17]):   5'b01000; 
				ShiftedIO(QB[18]):   5'b01001; 
				ShiftedIO(QB[19]):   5'b01001; 
				ShiftedIO(QB[20]):   5'b01010; 
				ShiftedIO(QB[21]):   5'b01010; 
				ShiftedIO(QB[22]):   5'b01011; 
				ShiftedIO(QB[23]):   5'b01011; 
				ShiftedIO(QB[24]):   5'b01100; 
				ShiftedIO(QB[25]):   5'b01100; 
				ShiftedIO(QB[26]):   5'b01101; 
				ShiftedIO(QB[27]):   5'b01101; 
				ShiftedIO(QB[28]):   5'b01110; 
				ShiftedIO(QB[29]):   5'b01110; 
				ShiftedIO(QB[30]):   5'b01111; 
				ShiftedIO(QB[31]):   5'b01111; 
				ShiftedIO(QB[32]):   5'b10000; 
				ShiftedIO(QB[33]):   5'b10000; 
				ShiftedIO(QB[34]):   5'b10001; 
				ShiftedIO(QB[35]):   5'b10001; 
			} 
		} 
		PinMap { 
			SpareElement { 
				RepairEnable: RepairRegister[9]; 
                                FuseMap[0]: RepairRegister[10]; 
                                FuseMap[1]: RepairRegister[11]; 
                                FuseMap[2]: RepairRegister[12]; 
                                FuseMap[3]: RepairRegister[13]; 
                                FuseMap[4]: RepairRegister[14]; 
				LogicLow  : RepairRegister[15]; 
				LogicLow  : RepairRegister[16]; 
				LogicLow  : RepairRegister[17]; 
			} 
		} 
	} 
} 

}
