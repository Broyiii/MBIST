# Grouping Constraints for tc02

if (Multiple Clock Domains)
  CLK Frequency (MHz) | xxx;

if (Multiple Power Domains)
  Power Domain | xxx;

if (DEF File Exists)
  UNITS_DISTANCE_MICRONS = 1000;
  max_distance = xxx;
else
  max_design_hierarchical_distance = 3;

Dynamical Test Power:
  max_dynamic_power = xxx;

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
