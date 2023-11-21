# Grouping Constraints for tc01

if (Multiple Clock Domains)
  CLK Frequency (MHz) | xxx;

if (Multiple Power Domains)
  Power Domain | xxx;

if (DEF File Exists)
  UNITS_DISTANCE_MICRONS = 1000;
  max_distance = 500000;
else
  max_design_heirarchical_distance = 3;

Dynamical Test Power:
  max_dynamic_power = 0.05;

Memory Type: SRAM | ROM;

Test Algorithm: SMarchCHKBvcd | ...;

# Grouping Constraints for Ports

Address and LogicalAddressMap;

RedundancyAnalysis: Yes | No;

LogicalPorts:  2RW;

OperationSet: SyncWRvcd;

ShadowRead: on | off;

ShadowWrite: on | off;

ShadowWriteOK: on | off;

WriteOutOfRange: on | off;

# Note for Unconstraints Ports includes D or Q (DOUT)
