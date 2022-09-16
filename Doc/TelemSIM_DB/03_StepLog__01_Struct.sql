DROP TABLE IF EXISTS "StepLog";
CREATE TABLE "StepLog" (
    "StepId"    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "PhaseId"   INTEGER NOT NULL DEFAULT 0,
    "StepName"  TEXT NOT NULL DEFAULT '',
    "Desp"      TEXT NOT NULL DEFAULT '',
    "RetCode"   INTEGER NOT NULL DEFAULT 0,
    "ToSkip"    INTEGER NOT NULL DEFAULT 0
);

-- ----------------------------
-- Auto increment value for StepLog
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'StepLog';

PRAGMA foreign_keys = true;
