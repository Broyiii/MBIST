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
/*                       Template Revision : 4.5.2                    */
/*   --------------------------------------------------------------   */
/*                      * Tessent MemoryBIST Model *                  */

   MemoryTemplate ( RF_2PHD_256x64 ) {
   CellName:      RF_2PHD_256x64;
   MemoryType:    SRAM;

   LogicalPorts:  1R1W;
   NumberofWords:  256;
   NumberofBits :  64;
   Algorithm:  SMarchCHKBvcd ;
   BitGrouping : 1;
   OperationSet: SyncWRvcd;
   MinHold:  0.420;
   MilliWattsPerMegaHertz:  5.022e-03;
   ShadowRead: Off;
   WriteOutOfRange: Off;

/* Scrambling Information */ 
   AddressCounter { 
     Function (Address) { 
       LogicalAddressMaP { 
         ColumnAddress[0] : Address [0]; 
         BankAddress[1:0] : Address [4:3];
         RowAddress[1:0] : Address [2:1];
         RowAddress[4:2] : Address [7:5];
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
Port (  QB[63:0] )
             {   
               Direction: OUTPUT;
               LogicalPort: B;
               Function: data;
             }
Port (  ADRA[7:0] )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: Address;
             }
Port (  DA[63:0] )
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
Port (  CLKA )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: Clock;
               Polarity: ActiveHigh;
             }
Port (  TEST1A )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: None;
               Safevalue: 0;
             }
Port (  RMEA )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: None;
               Safevalue: 0;
             }
Port (  RMA[3:0] )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: None;
               Safevalue: 4'b0010;
             }
Port (  LS )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  ADRB[7:0] )
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
Port (  CLKB )
             {   
               Direction: INPUT;
               LogicalPort: B;
               Function: Clock;
               Polarity: ActiveHigh;
             }
Port (  TEST1B )
             {   
               Direction: INPUT;
               LogicalPort: B;
               Function: None;
               Safevalue: 0;
             }
Port (  RMEB )
             {   
               Direction: INPUT;
               LogicalPort: B;
               Function: None;
               Safevalue: 0;
             }
Port (  RMB[3:0] )
             {   
               Direction: INPUT;
               LogicalPort: B;
               Function: None;
               Safevalue: 4'b0010;
             }
RedundancyAnalysis { 
	ColumnSegment (Right) { 
		ShiftedIORange: QB[63:32]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[3:0] { 
				ShiftedIO(QB[32]):   4'b0000; 
				ShiftedIO(QB[33]):   4'b0000; 
				ShiftedIO(QB[34]):   4'b0001; 
				ShiftedIO(QB[35]):   4'b0001; 
				ShiftedIO(QB[36]):   4'b0010; 
				ShiftedIO(QB[37]):   4'b0010; 
				ShiftedIO(QB[38]):   4'b0011; 
				ShiftedIO(QB[39]):   4'b0011; 
				ShiftedIO(QB[40]):   4'b0100; 
				ShiftedIO(QB[41]):   4'b0100; 
				ShiftedIO(QB[42]):   4'b0101; 
				ShiftedIO(QB[43]):   4'b0101; 
				ShiftedIO(QB[44]):   4'b0110; 
				ShiftedIO(QB[45]):   4'b0110; 
				ShiftedIO(QB[46]):   4'b0111; 
				ShiftedIO(QB[47]):   4'b0111; 
				ShiftedIO(QB[48]):   4'b1000; 
				ShiftedIO(QB[49]):   4'b1000; 
				ShiftedIO(QB[50]):   4'b1001; 
				ShiftedIO(QB[51]):   4'b1001; 
				ShiftedIO(QB[52]):   4'b1010; 
				ShiftedIO(QB[53]):   4'b1010; 
				ShiftedIO(QB[54]):   4'b1011; 
				ShiftedIO(QB[55]):   4'b1011; 
				ShiftedIO(QB[56]):   4'b1100; 
				ShiftedIO(QB[57]):   4'b1100; 
				ShiftedIO(QB[58]):   4'b1101; 
				ShiftedIO(QB[59]):   4'b1101; 
				ShiftedIO(QB[60]):   4'b1110; 
				ShiftedIO(QB[61]):   4'b1110; 
				ShiftedIO(QB[62]):   4'b1111; 
				ShiftedIO(QB[63]):   4'b1111; 
			} 
		} 
		PinMap { 
			SpareElement { 
				RepairEnable: RepairRegister[0]; 
                                FuseMap[0]: RepairRegister[1]; 
                                FuseMap[1]: RepairRegister[2]; 
                                FuseMap[2]: RepairRegister[3]; 
                                FuseMap[3]: RepairRegister[4]; 
				LogicLow  : RepairRegister[5]; 
				LogicLow  : RepairRegister[6]; 
				LogicLow  : RepairRegister[7]; 
				LogicLow  : RepairRegister[8]; 
			} 
		} 
	} 
	ColumnSegment (Left) { 
		ShiftedIORange: QB[31:0]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[3:0] { 
				ShiftedIO(QB[0]):   4'b0000; 
				ShiftedIO(QB[1]):   4'b0000; 
				ShiftedIO(QB[2]):   4'b0001; 
				ShiftedIO(QB[3]):   4'b0001; 
				ShiftedIO(QB[4]):   4'b0010; 
				ShiftedIO(QB[5]):   4'b0010; 
				ShiftedIO(QB[6]):   4'b0011; 
				ShiftedIO(QB[7]):   4'b0011; 
				ShiftedIO(QB[8]):   4'b0100; 
				ShiftedIO(QB[9]):   4'b0100; 
				ShiftedIO(QB[10]):   4'b0101; 
				ShiftedIO(QB[11]):   4'b0101; 
				ShiftedIO(QB[12]):   4'b0110; 
				ShiftedIO(QB[13]):   4'b0110; 
				ShiftedIO(QB[14]):   4'b0111; 
				ShiftedIO(QB[15]):   4'b0111; 
				ShiftedIO(QB[16]):   4'b1000; 
				ShiftedIO(QB[17]):   4'b1000; 
				ShiftedIO(QB[18]):   4'b1001; 
				ShiftedIO(QB[19]):   4'b1001; 
				ShiftedIO(QB[20]):   4'b1010; 
				ShiftedIO(QB[21]):   4'b1010; 
				ShiftedIO(QB[22]):   4'b1011; 
				ShiftedIO(QB[23]):   4'b1011; 
				ShiftedIO(QB[24]):   4'b1100; 
				ShiftedIO(QB[25]):   4'b1100; 
				ShiftedIO(QB[26]):   4'b1101; 
				ShiftedIO(QB[27]):   4'b1101; 
				ShiftedIO(QB[28]):   4'b1110; 
				ShiftedIO(QB[29]):   4'b1110; 
				ShiftedIO(QB[30]):   4'b1111; 
				ShiftedIO(QB[31]):   4'b1111; 
			} 
		} 
		PinMap { 
			SpareElement { 
				RepairEnable: RepairRegister[9]; 
                                FuseMap[0]: RepairRegister[10]; 
                                FuseMap[1]: RepairRegister[11]; 
                                FuseMap[2]: RepairRegister[12]; 
                                FuseMap[3]: RepairRegister[13]; 
				LogicLow  : RepairRegister[14]; 
				LogicLow  : RepairRegister[15]; 
				LogicLow  : RepairRegister[16]; 
				LogicLow  : RepairRegister[17]; 
			} 
		} 
	} 
} 

}