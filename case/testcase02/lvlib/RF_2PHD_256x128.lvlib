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

   MemoryTemplate ( RF_2PHD_256x128 ) {
   CellName:      RF_2PHD_256x128;
   MemoryType:    SRAM;

   LogicalPorts:  1R1W;
   NumberofWords:  256;
   NumberofBits :  128;
   Algorithm:  SMarchCHKBvcd ;
   OperationSet: SyncWRvcd;
   MinHold:  0.468;
   MilliWattsPerMegaHertz:  7.948e-03;
   ShadowRead: Off;
   WriteOutOfRange: Off;

/* Scrambling Information */ 
   AddressCounter { 
     Function (Address) { 
       LogicalAddressMaP { 
         BankAddress[1:0] : Address [3:2];
         RowAddress[1:0] : Address [1:0];
         RowAddress[5:2] : Address [7:4];
       } 
     } 
     Function (Rowaddress) { 
       CountRange [0:63]; 
     } 
   } 


Port (  RSCOUT )
             {   
               Direction: OUTPUT;
               Function: BisrSerialData;
             }
Port (  QB[127:0] )
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
Port (  DA[127:0] )
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
		ShiftedIORange: QB[127:64]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[3:0] { 
				ShiftedIO(QB[64]):   4'b0000; 
				ShiftedIO(QB[65]):   4'b0000; 
				ShiftedIO(QB[66]):   4'b0000; 
				ShiftedIO(QB[67]):   4'b0000; 
				ShiftedIO(QB[68]):   4'b0001; 
				ShiftedIO(QB[69]):   4'b0001; 
				ShiftedIO(QB[70]):   4'b0001; 
				ShiftedIO(QB[71]):   4'b0001; 
				ShiftedIO(QB[72]):   4'b0010; 
				ShiftedIO(QB[73]):   4'b0010; 
				ShiftedIO(QB[74]):   4'b0010; 
				ShiftedIO(QB[75]):   4'b0010; 
				ShiftedIO(QB[76]):   4'b0011; 
				ShiftedIO(QB[77]):   4'b0011; 
				ShiftedIO(QB[78]):   4'b0011; 
				ShiftedIO(QB[79]):   4'b0011; 
				ShiftedIO(QB[80]):   4'b0100; 
				ShiftedIO(QB[81]):   4'b0100; 
				ShiftedIO(QB[82]):   4'b0100; 
				ShiftedIO(QB[83]):   4'b0100; 
				ShiftedIO(QB[84]):   4'b0101; 
				ShiftedIO(QB[85]):   4'b0101; 
				ShiftedIO(QB[86]):   4'b0101; 
				ShiftedIO(QB[87]):   4'b0101; 
				ShiftedIO(QB[88]):   4'b0110; 
				ShiftedIO(QB[89]):   4'b0110; 
				ShiftedIO(QB[90]):   4'b0110; 
				ShiftedIO(QB[91]):   4'b0110; 
				ShiftedIO(QB[92]):   4'b0111; 
				ShiftedIO(QB[93]):   4'b0111; 
				ShiftedIO(QB[94]):   4'b0111; 
				ShiftedIO(QB[95]):   4'b0111; 
				ShiftedIO(QB[96]):   4'b1000; 
				ShiftedIO(QB[97]):   4'b1000; 
				ShiftedIO(QB[98]):   4'b1000; 
				ShiftedIO(QB[99]):   4'b1000; 
				ShiftedIO(QB[100]):   4'b1001; 
				ShiftedIO(QB[101]):   4'b1001; 
				ShiftedIO(QB[102]):   4'b1001; 
				ShiftedIO(QB[103]):   4'b1001; 
				ShiftedIO(QB[104]):   4'b1010; 
				ShiftedIO(QB[105]):   4'b1010; 
				ShiftedIO(QB[106]):   4'b1010; 
				ShiftedIO(QB[107]):   4'b1010; 
				ShiftedIO(QB[108]):   4'b1011; 
				ShiftedIO(QB[109]):   4'b1011; 
				ShiftedIO(QB[110]):   4'b1011; 
				ShiftedIO(QB[111]):   4'b1011; 
				ShiftedIO(QB[112]):   4'b1100; 
				ShiftedIO(QB[113]):   4'b1100; 
				ShiftedIO(QB[114]):   4'b1100; 
				ShiftedIO(QB[115]):   4'b1100; 
				ShiftedIO(QB[116]):   4'b1101; 
				ShiftedIO(QB[117]):   4'b1101; 
				ShiftedIO(QB[118]):   4'b1101; 
				ShiftedIO(QB[119]):   4'b1101; 
				ShiftedIO(QB[120]):   4'b1110; 
				ShiftedIO(QB[121]):   4'b1110; 
				ShiftedIO(QB[122]):   4'b1110; 
				ShiftedIO(QB[123]):   4'b1110; 
				ShiftedIO(QB[124]):   4'b1111; 
				ShiftedIO(QB[125]):   4'b1111; 
				ShiftedIO(QB[126]):   4'b1111; 
				ShiftedIO(QB[127]):   4'b1111; 
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
		ShiftedIORange: QB[63:0]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[3:0] { 
				ShiftedIO(QB[0]):   4'b0000; 
				ShiftedIO(QB[1]):   4'b0000; 
				ShiftedIO(QB[2]):   4'b0000; 
				ShiftedIO(QB[3]):   4'b0000; 
				ShiftedIO(QB[4]):   4'b0001; 
				ShiftedIO(QB[5]):   4'b0001; 
				ShiftedIO(QB[6]):   4'b0001; 
				ShiftedIO(QB[7]):   4'b0001; 
				ShiftedIO(QB[8]):   4'b0010; 
				ShiftedIO(QB[9]):   4'b0010; 
				ShiftedIO(QB[10]):   4'b0010; 
				ShiftedIO(QB[11]):   4'b0010; 
				ShiftedIO(QB[12]):   4'b0011; 
				ShiftedIO(QB[13]):   4'b0011; 
				ShiftedIO(QB[14]):   4'b0011; 
				ShiftedIO(QB[15]):   4'b0011; 
				ShiftedIO(QB[16]):   4'b0100; 
				ShiftedIO(QB[17]):   4'b0100; 
				ShiftedIO(QB[18]):   4'b0100; 
				ShiftedIO(QB[19]):   4'b0100; 
				ShiftedIO(QB[20]):   4'b0101; 
				ShiftedIO(QB[21]):   4'b0101; 
				ShiftedIO(QB[22]):   4'b0101; 
				ShiftedIO(QB[23]):   4'b0101; 
				ShiftedIO(QB[24]):   4'b0110; 
				ShiftedIO(QB[25]):   4'b0110; 
				ShiftedIO(QB[26]):   4'b0110; 
				ShiftedIO(QB[27]):   4'b0110; 
				ShiftedIO(QB[28]):   4'b0111; 
				ShiftedIO(QB[29]):   4'b0111; 
				ShiftedIO(QB[30]):   4'b0111; 
				ShiftedIO(QB[31]):   4'b0111; 
				ShiftedIO(QB[32]):   4'b1000; 
				ShiftedIO(QB[33]):   4'b1000; 
				ShiftedIO(QB[34]):   4'b1000; 
				ShiftedIO(QB[35]):   4'b1000; 
				ShiftedIO(QB[36]):   4'b1001; 
				ShiftedIO(QB[37]):   4'b1001; 
				ShiftedIO(QB[38]):   4'b1001; 
				ShiftedIO(QB[39]):   4'b1001; 
				ShiftedIO(QB[40]):   4'b1010; 
				ShiftedIO(QB[41]):   4'b1010; 
				ShiftedIO(QB[42]):   4'b1010; 
				ShiftedIO(QB[43]):   4'b1010; 
				ShiftedIO(QB[44]):   4'b1011; 
				ShiftedIO(QB[45]):   4'b1011; 
				ShiftedIO(QB[46]):   4'b1011; 
				ShiftedIO(QB[47]):   4'b1011; 
				ShiftedIO(QB[48]):   4'b1100; 
				ShiftedIO(QB[49]):   4'b1100; 
				ShiftedIO(QB[50]):   4'b1100; 
				ShiftedIO(QB[51]):   4'b1100; 
				ShiftedIO(QB[52]):   4'b1101; 
				ShiftedIO(QB[53]):   4'b1101; 
				ShiftedIO(QB[54]):   4'b1101; 
				ShiftedIO(QB[55]):   4'b1101; 
				ShiftedIO(QB[56]):   4'b1110; 
				ShiftedIO(QB[57]):   4'b1110; 
				ShiftedIO(QB[58]):   4'b1110; 
				ShiftedIO(QB[59]):   4'b1110; 
				ShiftedIO(QB[60]):   4'b1111; 
				ShiftedIO(QB[61]):   4'b1111; 
				ShiftedIO(QB[62]):   4'b1111; 
				ShiftedIO(QB[63]):   4'b1111; 
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
