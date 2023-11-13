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

   MemoryTemplate ( RF_2PUHD_1440x75 ) {
   CellName:      RF_2PUHD_1440x75;
   MemoryType:    SRAM;

   LogicalPorts:  1R1W;
   NumberofWords:  1440;
   NumberofBits :  75;
   Algorithm:  SMarchCHKBvcd ;
   BitGrouping : 1;
   OperationSet: SyncWRvcd;
   MinHold:  0.833;
   MilliWattsPerMegaHertz:  9.083e-03;
   ShadowRead: Off;
   WriteOutOfRange: Off;

/* Scrambling Information */ 
   AddressCounter { 
     Function (Address) { 
       LogicalAddressMaP { 
         ColumnAddress[1:0] : Address [1:0];
           BankAddress[0] : Address [4]; 
         RowAddress[1:0] : Address [3:2];
         RowAddress[7:2] : Address [10:5];
       } 
     } 
     Function (Rowaddress) { 
       CountRange [0:179]; 
     } 
     Function (Columnaddress) { 
       CountRange [0:3]; 
     } 
   } 


Port (  RSCOUT )
             {   
               Direction: OUTPUT;
               Function: BisrSerialData;
             }
Port (  QB[74:0] )
             {   
               Direction: OUTPUT;
               LogicalPort: B;
               Function: data;
             }
Port (  ADRA[10:0] )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: Address;
             }
Port (  DA[74:0] )
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
Port (  ADRB[10:0] )
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
		ShiftedIORange: QB[74:38]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[5:0] { 
				ShiftedIO(QB[38]):   6'b000000; 
				ShiftedIO(QB[39]):   6'b000001; 
				ShiftedIO(QB[40]):   6'b000010; 
				ShiftedIO(QB[41]):   6'b000011; 
				ShiftedIO(QB[42]):   6'b000100; 
				ShiftedIO(QB[43]):   6'b000101; 
				ShiftedIO(QB[44]):   6'b000110; 
				ShiftedIO(QB[45]):   6'b000111; 
				ShiftedIO(QB[46]):   6'b001000; 
				ShiftedIO(QB[47]):   6'b001001; 
				ShiftedIO(QB[48]):   6'b001010; 
				ShiftedIO(QB[49]):   6'b001011; 
				ShiftedIO(QB[50]):   6'b001100; 
				ShiftedIO(QB[51]):   6'b001101; 
				ShiftedIO(QB[52]):   6'b001110; 
				ShiftedIO(QB[53]):   6'b001111; 
				ShiftedIO(QB[54]):   6'b010000; 
				ShiftedIO(QB[55]):   6'b010001; 
				ShiftedIO(QB[56]):   6'b010010; 
				ShiftedIO(QB[57]):   6'b010011; 
				ShiftedIO(QB[58]):   6'b010100; 
				ShiftedIO(QB[59]):   6'b010101; 
				ShiftedIO(QB[60]):   6'b010110; 
				ShiftedIO(QB[61]):   6'b010111; 
				ShiftedIO(QB[62]):   6'b011000; 
				ShiftedIO(QB[63]):   6'b011001; 
				ShiftedIO(QB[64]):   6'b011010; 
				ShiftedIO(QB[65]):   6'b011011; 
				ShiftedIO(QB[66]):   6'b011100; 
				ShiftedIO(QB[67]):   6'b011101; 
				ShiftedIO(QB[68]):   6'b011110; 
				ShiftedIO(QB[69]):   6'b011111; 
				ShiftedIO(QB[70]):   6'b100000; 
				ShiftedIO(QB[71]):   6'b100001; 
				ShiftedIO(QB[72]):   6'b100010; 
				ShiftedIO(QB[73]):   6'b100011; 
				ShiftedIO(QB[74]):   6'b100100; 
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
                                FuseMap[5]: RepairRegister[6]; 
				LogicLow  : RepairRegister[7]; 
				LogicLow  : RepairRegister[8]; 
			} 
		} 
	} 
	ColumnSegment (Left) { 
		ShiftedIORange: QB[37:0]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[5:0] { 
				ShiftedIO(QB[0]):   6'b000000; 
				ShiftedIO(QB[1]):   6'b000001; 
				ShiftedIO(QB[2]):   6'b000010; 
				ShiftedIO(QB[3]):   6'b000011; 
				ShiftedIO(QB[4]):   6'b000100; 
				ShiftedIO(QB[5]):   6'b000101; 
				ShiftedIO(QB[6]):   6'b000110; 
				ShiftedIO(QB[7]):   6'b000111; 
				ShiftedIO(QB[8]):   6'b001000; 
				ShiftedIO(QB[9]):   6'b001001; 
				ShiftedIO(QB[10]):   6'b001010; 
				ShiftedIO(QB[11]):   6'b001011; 
				ShiftedIO(QB[12]):   6'b001100; 
				ShiftedIO(QB[13]):   6'b001101; 
				ShiftedIO(QB[14]):   6'b001110; 
				ShiftedIO(QB[15]):   6'b001111; 
				ShiftedIO(QB[16]):   6'b010000; 
				ShiftedIO(QB[17]):   6'b010001; 
				ShiftedIO(QB[18]):   6'b010010; 
				ShiftedIO(QB[19]):   6'b010011; 
				ShiftedIO(QB[20]):   6'b010100; 
				ShiftedIO(QB[21]):   6'b010101; 
				ShiftedIO(QB[22]):   6'b010110; 
				ShiftedIO(QB[23]):   6'b010111; 
				ShiftedIO(QB[24]):   6'b011000; 
				ShiftedIO(QB[25]):   6'b011001; 
				ShiftedIO(QB[26]):   6'b011010; 
				ShiftedIO(QB[27]):   6'b011011; 
				ShiftedIO(QB[28]):   6'b011100; 
				ShiftedIO(QB[29]):   6'b011101; 
				ShiftedIO(QB[30]):   6'b011110; 
				ShiftedIO(QB[31]):   6'b011111; 
				ShiftedIO(QB[32]):   6'b100000; 
				ShiftedIO(QB[33]):   6'b100001; 
				ShiftedIO(QB[34]):   6'b100010; 
				ShiftedIO(QB[35]):   6'b100011; 
				ShiftedIO(QB[36]):   6'b100100; 
				ShiftedIO(QB[37]):   6'b100101; 
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
                                FuseMap[5]: RepairRegister[15]; 
				LogicLow  : RepairRegister[16]; 
				LogicLow  : RepairRegister[17]; 
			} 
		} 
	} 
} 

}
