DROP TABLE IF EXISTS "SrcFileInfo";
CREATE TABLE "SrcFileInfo" (
    "FID"       INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "DataSetId" INTEGER NOT NULL DEFAULT 0,
    "Fidx"      INTEGER NOT NULL DEFAULT 0,
    "Status"    INTEGER NOT NULL DEFAULT 0,
    "Filename"  TEXT NOT NULL DEFAULT '',
    "Fullpath"  TEXT NOT NULL DEFAULT '',
    "FileSize"  INTEGER NOT NULL DEFAULT 0,
    "h32_0"     INTEGER NOT NULL DEFAULT 0,
    "h32_1"     INTEGER NOT NULL DEFAULT 0
);

-- ----------------------------
-- Auto increment value for SrcFileInfo
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'SrcFileInfo';

PRAGMA foreign_keys = true;
