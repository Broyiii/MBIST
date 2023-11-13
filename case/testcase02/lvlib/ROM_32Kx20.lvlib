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
/*                       Template Revision : 3.1.0                    */
/*   --------------------------------------------------------------   */

/*                      * Tessent MemoryBIST Model *                  */
/* This memBIST model does not contain sufficient information to guarantee exact checkerboard pattern. */

   MemoryTemplate ( ROM_32Kx20 ) {
   CellName:      ROM_32Kx20;
   MemoryType:    ROM;

   LogicalPorts:  1R; 
   NumberofWords:  32768;
   NumberofBits :  20;
   Algorithm:  ReadOnly ;
   OperationSet: ROM;  
   MinHold:  0.775;
   MilliWattsPerMegaHertz:  8.015e-03;
   ROMContentsFile: auto.vh;
   ShadowRead: Off;
   AddressCounter {
     Function (Address) {
       LogicalAddressMaP {
         ColumnAddress [4:0] : Address [4:0] ;
         RowAddress [9:0] : Address [14:5] ;
       }
     }
     Function (Rowaddress) {
       CountRange [0:1023];
     }
     Function (Columnaddress) {
       CountRange [0:31];
     }
   }
Port (  Q[19:0] )
             {   
               Direction: OUTPUT;
               Function: data;
             }
Port (  ADR[14:0] )
             {   
               Direction: INPUT;
               Function: Address;
             }
Port (  ME )
             {   
               Direction: INPUT;
               Function: Select;
               Polarity: ActiveHigh;
             }
Port (  CLK )
             {   
               Direction: INPUT;
               Function: Clock;
               Polarity: ActiveHigh;
             }
Port (  LS )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
Port (  TEST1 )
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
Port (  RME )
             {   
               Direction: INPUT;
               Function: None;
               Safevalue: 0;
             }
}
