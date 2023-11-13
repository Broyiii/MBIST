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

   MemoryTemplate ( RF_2PHD_256x39 ) {
   CellName:      RF_2PHD_256x39;
   MemoryType:    SRAM;

   LogicalPorts:  1R1W;
   NumberofWords:  256;
   NumberofBits :  39;
   Algorithm:  SMarchCHKBvcd ;
   BitGrouping : 1;
   OperationSet: SyncWRvcd;
   MinHold:  0.455;
   MilliWattsPerMegaHertz:  4.690e-03;
   ShadowRead: Off;
   WriteOutOfRange: Off;

/* Scrambling Information */ 
   AddressCounter { 
     Function (Address) { 
       LogicalAddressMaP { 
         ColumnAddress[1:0] : Address [1:0];
         BankAddress[1:0] : Address [5:4];
         RowAddress[1:0] : Address [3:2];
         RowAddress[3:2] : Address [7:6];
       } 
     } 
     Function (Rowaddress) { 
       CountRange [0:15]; 
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
Port (  QB[38:0] )
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
Port (  DA[38:0] )
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
		ShiftedIORange: QB[38:20]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[4:0] { 
				ShiftedIO(QB[20]):   5'b00000; 
				ShiftedIO(QB[21]):   5'b00001; 
				ShiftedIO(QB[22]):   5'b00010; 
				ShiftedIO(QB[23]):   5'b00011; 
				ShiftedIO(QB[24]):   5'b00100; 
				ShiftedIO(QB[25]):   5'b00101; 
				ShiftedIO(QB[26]):   5'b00110; 
				ShiftedIO(QB[27]):   5'b00111; 
				ShiftedIO(QB[28]):   5'b01000; 
				ShiftedIO(QB[29]):   5'b01001; 
				ShiftedIO(QB[30]):   5'b01010; 
				ShiftedIO(QB[31]):   5'b01011; 
				ShiftedIO(QB[32]):   5'b01100; 
				ShiftedIO(QB[33]):   5'b01101; 
				ShiftedIO(QB[34]):   5'b01110; 
				ShiftedIO(QB[35]):   5'b01111; 
				ShiftedIO(QB[36]):   5'b10000; 
				ShiftedIO(QB[37]):   5'b10001; 
				ShiftedIO(QB[38]):   5'b10010; 
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
		ShiftedIORange: QB[19:0]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[4:0] { 
				ShiftedIO(QB[0]):   5'b00000; 
				ShiftedIO(QB[1]):   5'b00001; 
				ShiftedIO(QB[2]):   5'b00010; 
				ShiftedIO(QB[3]):   5'b00011; 
				ShiftedIO(QB[4]):   5'b00100; 
				ShiftedIO(QB[5]):   5'b00101; 
				ShiftedIO(QB[6]):   5'b00110; 
				ShiftedIO(QB[7]):   5'b00111; 
				ShiftedIO(QB[8]):   5'b01000; 
				ShiftedIO(QB[9]):   5'b01001; 
				ShiftedIO(QB[10]):   5'b01010; 
				ShiftedIO(QB[11]):   5'b01011; 
				ShiftedIO(QB[12]):   5'b01100; 
				ShiftedIO(QB[13]):   5'b01101; 
				ShiftedIO(QB[14]):   5'b01110; 
				ShiftedIO(QB[15]):   5'b01111; 
				ShiftedIO(QB[16]):   5'b10000; 
				ShiftedIO(QB[17]):   5'b10001; 
				ShiftedIO(QB[18]):   5'b10010; 
				ShiftedIO(QB[19]):   5'b10011; 
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
