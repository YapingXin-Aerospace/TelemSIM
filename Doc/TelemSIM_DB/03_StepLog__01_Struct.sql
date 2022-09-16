DROP TABLE IF EXISTS "StepLog";
CREATE TABLE "StepLog" (
    "StepId"    INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "PhaseId"   INTEGER NOT NULL DEFAULT 0,
    "StepName"  TEXT NOT NULL DEFAULT '',
    "Desp"      TEXT NOT NULL DEFAULT '',
    "RetCode"   INTEGER NOT NULL DEFAULT 0,
    "MK0"       INTEGER NOT NULL DEFAULT 0,
    "MK1"       INTEGER NOT NULL DEFAULT 0,
    "MK2"       INTEGER NOT NULL DEFAULT 0,
    "MK3"       INTEGER NOT NULL DEFAULT 0
);

-- ----------------------------
-- Auto increment value for StepLog
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'StepLog';

PRAGMA foreign_keys = true;
