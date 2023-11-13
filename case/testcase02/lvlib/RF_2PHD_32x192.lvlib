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

   MemoryTemplate ( RF_2PHD_32x192 ) {
   CellName:      RF_2PHD_32x192;
   MemoryType:    SRAM;

   LogicalPorts:  1R1W;
   NumberofWords:  32;
   NumberofBits :  192;
   Algorithm:  SMarchCHKBvcd ;
   OperationSet: SyncWRvcd;
   MinHold:  0.454;
   MilliWattsPerMegaHertz:  8.419e-03;
   ShadowRead: Off;
   WriteOutOfRange: Off;

/* Scrambling Information */ 
   AddressCounter { 
     Function (Address) { 
       LogicalAddressMaP { 
           BankAddress[0] : Address [2]; 
         RowAddress[1:0] : Address [1:0];
         RowAddress[3:2] : Address [4:3];
       } 
     } 
     Function (Rowaddress) { 
       CountRange [0:15]; 
     } 
   } 


Port (  RSCOUT )
             {   
               Direction: OUTPUT;
               Function: BisrSerialData;
             }
Port (  QB[191:0] )
             {   
               Direction: OUTPUT;
               LogicalPort: B;
               Function: data;
             }
Port (  ADRA[4:0] )
             {   
               Direction: INPUT;
               LogicalPort: A;
               Function: Address;
             }
Port (  DA[191:0] )
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
Port (  ADRB[4:0] )
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
		ShiftedIORange: QB[191:96]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[4:0] { 
				ShiftedIO(QB[96]):   5'b00000; 
				ShiftedIO(QB[97]):   5'b00000; 
				ShiftedIO(QB[98]):   5'b00000; 
				ShiftedIO(QB[99]):   5'b00000; 
				ShiftedIO(QB[100]):   5'b00001; 
				ShiftedIO(QB[101]):   5'b00001; 
				ShiftedIO(QB[102]):   5'b00001; 
				ShiftedIO(QB[103]):   5'b00001; 
				ShiftedIO(QB[104]):   5'b00010; 
				ShiftedIO(QB[105]):   5'b00010; 
				ShiftedIO(QB[106]):   5'b00010; 
				ShiftedIO(QB[107]):   5'b00010; 
				ShiftedIO(QB[108]):   5'b00011; 
				ShiftedIO(QB[109]):   5'b00011; 
				ShiftedIO(QB[110]):   5'b00011; 
				ShiftedIO(QB[111]):   5'b00011; 
				ShiftedIO(QB[112]):   5'b00100; 
				ShiftedIO(QB[113]):   5'b00100; 
				ShiftedIO(QB[114]):   5'b00100; 
				ShiftedIO(QB[115]):   5'b00100; 
				ShiftedIO(QB[116]):   5'b00101; 
				ShiftedIO(QB[117]):   5'b00101; 
				ShiftedIO(QB[118]):   5'b00101; 
				ShiftedIO(QB[119]):   5'b00101; 
				ShiftedIO(QB[120]):   5'b00110; 
				ShiftedIO(QB[121]):   5'b00110; 
				ShiftedIO(QB[122]):   5'b00110; 
				ShiftedIO(QB[123]):   5'b00110; 
				ShiftedIO(QB[124]):   5'b00111; 
				ShiftedIO(QB[125]):   5'b00111; 
				ShiftedIO(QB[126]):   5'b00111; 
				ShiftedIO(QB[127]):   5'b00111; 
				ShiftedIO(QB[128]):   5'b01000; 
				ShiftedIO(QB[129]):   5'b01000; 
				ShiftedIO(QB[130]):   5'b01000; 
				ShiftedIO(QB[131]):   5'b01000; 
				ShiftedIO(QB[132]):   5'b01001; 
				ShiftedIO(QB[133]):   5'b01001; 
				ShiftedIO(QB[134]):   5'b01001; 
				ShiftedIO(QB[135]):   5'b01001; 
				ShiftedIO(QB[136]):   5'b01010; 
				ShiftedIO(QB[137]):   5'b01010; 
				ShiftedIO(QB[138]):   5'b01010; 
				ShiftedIO(QB[139]):   5'b01010; 
				ShiftedIO(QB[140]):   5'b01011; 
				ShiftedIO(QB[141]):   5'b01011; 
				ShiftedIO(QB[142]):   5'b01011; 
				ShiftedIO(QB[143]):   5'b01011; 
				ShiftedIO(QB[144]):   5'b01100; 
				ShiftedIO(QB[145]):   5'b01100; 
				ShiftedIO(QB[146]):   5'b01100; 
				ShiftedIO(QB[147]):   5'b01100; 
				ShiftedIO(QB[148]):   5'b01101; 
				ShiftedIO(QB[149]):   5'b01101; 
				ShiftedIO(QB[150]):   5'b01101; 
				ShiftedIO(QB[151]):   5'b01101; 
				ShiftedIO(QB[152]):   5'b01110; 
				ShiftedIO(QB[153]):   5'b01110; 
				ShiftedIO(QB[154]):   5'b01110; 
				ShiftedIO(QB[155]):   5'b01110; 
				ShiftedIO(QB[156]):   5'b01111; 
				ShiftedIO(QB[157]):   5'b01111; 
				ShiftedIO(QB[158]):   5'b01111; 
				ShiftedIO(QB[159]):   5'b01111; 
				ShiftedIO(QB[160]):   5'b10000; 
				ShiftedIO(QB[161]):   5'b10000; 
				ShiftedIO(QB[162]):   5'b10000; 
				ShiftedIO(QB[163]):   5'b10000; 
				ShiftedIO(QB[164]):   5'b10001; 
				ShiftedIO(QB[165]):   5'b10001; 
				ShiftedIO(QB[166]):   5'b10001; 
				ShiftedIO(QB[167]):   5'b10001; 
				ShiftedIO(QB[168]):   5'b10010; 
				ShiftedIO(QB[169]):   5'b10010; 
				ShiftedIO(QB[170]):   5'b10010; 
				ShiftedIO(QB[171]):   5'b10010; 
				ShiftedIO(QB[172]):   5'b10011; 
				ShiftedIO(QB[173]):   5'b10011; 
				ShiftedIO(QB[174]):   5'b10011; 
				ShiftedIO(QB[175]):   5'b10011; 
				ShiftedIO(QB[176]):   5'b10100; 
				ShiftedIO(QB[177]):   5'b10100; 
				ShiftedIO(QB[178]):   5'b10100; 
				ShiftedIO(QB[179]):   5'b10100; 
				ShiftedIO(QB[180]):   5'b10101; 
				ShiftedIO(QB[181]):   5'b10101; 
				ShiftedIO(QB[182]):   5'b10101; 
				ShiftedIO(QB[183]):   5'b10101; 
				ShiftedIO(QB[184]):   5'b10110; 
				ShiftedIO(QB[185]):   5'b10110; 
				ShiftedIO(QB[186]):   5'b10110; 
				ShiftedIO(QB[187]):   5'b10110; 
				ShiftedIO(QB[188]):   5'b10111; 
				ShiftedIO(QB[189]):   5'b10111; 
				ShiftedIO(QB[190]):   5'b10111; 
				ShiftedIO(QB[191]):   5'b10111; 
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
		ShiftedIORange: QB[95:0]; 
                NumberOfSpareElements : 1; 
		FuseSet { 
			FuseMap[4:0] { 
				ShiftedIO(QB[0]):   5'b00000; 
				ShiftedIO(QB[1]):   5'b00000; 
				ShiftedIO(QB[2]):   5'b00000; 
				ShiftedIO(QB[3]):   5'b00000; 
				ShiftedIO(QB[4]):   5'b00001; 
				ShiftedIO(QB[5]):   5'b00001; 
				ShiftedIO(QB[6]):   5'b00001; 
				ShiftedIO(QB[7]):   5'b00001; 
				ShiftedIO(QB[8]):   5'b00010; 
				ShiftedIO(QB[9]):   5'b00010; 
				ShiftedIO(QB[10]):   5'b00010; 
				ShiftedIO(QB[11]):   5'b00010; 
				ShiftedIO(QB[12]):   5'b00011; 
				ShiftedIO(QB[13]):   5'b00011; 
				ShiftedIO(QB[14]):   5'b00011; 
				ShiftedIO(QB[15]):   5'b00011; 
				ShiftedIO(QB[16]):   5'b00100; 
				ShiftedIO(QB[17]):   5'b00100; 
				ShiftedIO(QB[18]):   5'b00100; 
				ShiftedIO(QB[19]):   5'b00100; 
				ShiftedIO(QB[20]):   5'b00101; 
				ShiftedIO(QB[21]):   5'b00101; 
				ShiftedIO(QB[22]):   5'b00101; 
				ShiftedIO(QB[23]):   5'b00101; 
				ShiftedIO(QB[24]):   5'b00110; 
				ShiftedIO(QB[25]):   5'b00110; 
				ShiftedIO(QB[26]):   5'b00110; 
				ShiftedIO(QB[27]):   5'b00110; 
				ShiftedIO(QB[28]):   5'b00111; 
				ShiftedIO(QB[29]):   5'b00111; 
				ShiftedIO(QB[30]):   5'b00111; 
				ShiftedIO(QB[31]):   5'b00111; 
				ShiftedIO(QB[32]):   5'b01000; 
				ShiftedIO(QB[33]):   5'b01000; 
				ShiftedIO(QB[34]):   5'b01000; 
				ShiftedIO(QB[35]):   5'b01000; 
				ShiftedIO(QB[36]):   5'b01001; 
				ShiftedIO(QB[37]):   5'b01001; 
				ShiftedIO(QB[38]):   5'b01001; 
				ShiftedIO(QB[39]):   5'b01001; 
				ShiftedIO(QB[40]):   5'b01010; 
				ShiftedIO(QB[41]):   5'b01010; 
				ShiftedIO(QB[42]):   5'b01010; 
				ShiftedIO(QB[43]):   5'b01010; 
				ShiftedIO(QB[44]):   5'b01011; 
				ShiftedIO(QB[45]):   5'b01011; 
				ShiftedIO(QB[46]):   5'b01011; 
				ShiftedIO(QB[47]):   5'b01011; 
				ShiftedIO(QB[48]):   5'b01100; 
				ShiftedIO(QB[49]):   5'b01100; 
				ShiftedIO(QB[50]):   5'b01100; 
				ShiftedIO(QB[51]):   5'b01100; 
				ShiftedIO(QB[52]):   5'b01101; 
				ShiftedIO(QB[53]):   5'b01101; 
				ShiftedIO(QB[54]):   5'b01101; 
				ShiftedIO(QB[55]):   5'b01101; 
				ShiftedIO(QB[56]):   5'b01110; 
				ShiftedIO(QB[57]):   5'b01110; 
				ShiftedIO(QB[58]):   5'b01110; 
				ShiftedIO(QB[59]):   5'b01110; 
				ShiftedIO(QB[60]):   5'b01111; 
				ShiftedIO(QB[61]):   5'b01111; 
				ShiftedIO(QB[62]):   5'b01111; 
				ShiftedIO(QB[63]):   5'b01111; 
				ShiftedIO(QB[64]):   5'b10000; 
				ShiftedIO(QB[65]):   5'b10000; 
				ShiftedIO(QB[66]):   5'b10000; 
				ShiftedIO(QB[67]):   5'b10000; 
				ShiftedIO(QB[68]):   5'b10001; 
				ShiftedIO(QB[69]):   5'b10001; 
				ShiftedIO(QB[70]):   5'b10001; 
				ShiftedIO(QB[71]):   5'b10001; 
				ShiftedIO(QB[72]):   5'b10010; 
				ShiftedIO(QB[73]):   5'b10010; 
				ShiftedIO(QB[74]):   5'b10010; 
				ShiftedIO(QB[75]):   5'b10010; 
				ShiftedIO(QB[76]):   5'b10011; 
				ShiftedIO(QB[77]):   5'b10011; 
				ShiftedIO(QB[78]):   5'b10011; 
				ShiftedIO(QB[79]):   5'b10011; 
				ShiftedIO(QB[80]):   5'b10100; 
				ShiftedIO(QB[81]):   5'b10100; 
				ShiftedIO(QB[82]):   5'b10100; 
				ShiftedIO(QB[83]):   5'b10100; 
				ShiftedIO(QB[84]):   5'b10101; 
				ShiftedIO(QB[85]):   5'b10101; 
				ShiftedIO(QB[86]):   5'b10101; 
				ShiftedIO(QB[87]):   5'b10101; 
				ShiftedIO(QB[88]):   5'b10110; 
				ShiftedIO(QB[89]):   5'b10110; 
				ShiftedIO(QB[90]):   5'b10110; 
				ShiftedIO(QB[91]):   5'b10110; 
				ShiftedIO(QB[92]):   5'b10111; 
				ShiftedIO(QB[93]):   5'b10111; 
				ShiftedIO(QB[94]):   5'b10111; 
				ShiftedIO(QB[95]):   5'b10111; 
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
