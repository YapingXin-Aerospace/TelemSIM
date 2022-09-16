/*

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

*/

INSERT INTO StepLog( StepId, PhaseId, StepName, Desp ) VALUES( 1001, 1, 'LoopTelemSrcFiles',    'Loop the *.dat files under the TelemetrySource directory.' );
