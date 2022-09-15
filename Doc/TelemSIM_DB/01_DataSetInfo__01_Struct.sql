DROP TABLE IF EXISTS "DataSetInfo";
CREATE TABLE "DataSetInfo" (
    "DataSetId"     INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "DataSetName"   TEXT NOT NULL DEFAULT '',
    "Status"        INTEGER NOT NULL DEFAULT 0,
    "Dirpath"       TEXT NOT NULL DEFAULT '',
    "SrcFilesCnt"   INTEGER NOT NULL DEFAULT 0,
    "h32_0"         INTEGER NOT NULL DEFAULT 0,
    "h32_1"         INTEGER NOT NULL DEFAULT 0,
    CONSTRAINT "UNIQUE_DataSetInfo_Dirpath" UNIQUE ("Dirpath" ASC),
    CONSTRAINT "UNIQUE_DataSetInfo_DataSetName" UNIQUE ("DataSetName" ASC)
);


-- ----------------------------
-- Auto increment value for DataSetInfo
-- ----------------------------
UPDATE "sqlite_sequence" SET seq = 1 WHERE name = 'DataSetInfo';
