/*

ToSkip  : uint8_t
PhaseId : uint8_t
StepId  : uint16_t
RetCode : uint32_t

===========================
ToSkip  Desp
---------------------------
0       不略过此步骤；
1       略过此步骤，不予执行
===========================

===========================
PhaseId PhaseName
---------------------------
1       DataPrepare
2       DataProcess
===========================

===========================
StepId  StepName
---------------------------
1001    LoopTelemSrcFiles
===========================

RetCode: 0  Not Started
RetCode: 1  Completed Successful
RetCode: 2  Completed Failed

*/

INSERT INTO StepLog( StepId, PhaseId, StepName, Desp ) VALUES( 1001, 1, 'LoopTelemSrcFiles',    'Loop the *.dat files under the TelemetrySource directory.' );
