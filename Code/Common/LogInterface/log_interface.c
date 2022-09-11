#include "log_interface.h"

#include "../../LibDev/iLog3/LOGSCONF_KVFILE.h"

#define LOG_STYLES_DOG        ( LOG_STYLE_DATETIMEMS | LOG_STYLE_LOGLEVEL | LOG_STYLE_PID | LOG_STYLE_TID | LOG_STYLE_SOURCE | LOG_STYLE_FORMAT | LOG_STYLE_NEWLINE )
#define LOG_STYLES_EVENT      ( LOG_STYLE_DATETIMEMS | LOG_STYLE_CUSTLABEL1 | LOG_STYLE_FORMAT | LOG_STYLE_NEWLINE )
#define LOG_STYLES_EVENT2     ( LOG_STYLE_DATETIMEMS | LOG_STYLE_LOGLEVEL | LOG_STYLE_TID | LOG_STYLE_FORMAT | LOG_STYLE_NEWLINE )

static LOGS *gs = NULL;

static HResult logging_iLog3_initialize(char* log_filepath);
static void logging_iLog3_release();


HResult logging_initialize(char* log_file)
{
    return logging_iLog3_initialize(log_file);
}

HResult LoggingInitWithConfig(char* config_filename, char* file_ext_name)
{
    HResult result = HResult_OK;

    gs = CreateLogsHandleFromKvFileG(config_filename, file_ext_name);
    if (gs == NULL)
    {
        // Error: The log group object initialize failed.
        result = HResult_CUSTOM_7F_FAIL + 0x01;
        goto EXIT;
    }

EXIT:
    return result;
}


void logging_release()
{
    logging_iLog3_release();
}


static HResult logging_iLog3_initialize(char* log_filepath)
{
    HResult result = HResult_OK;
    LOG *log_file = NULL;
    LOG *log_stdout = NULL;

    gs = CreateLogsHandleG();
    if (gs == NULL)
    {
        // Error: The log group object initialize failed.
        result = HResult_CUSTOM_7F_FAIL + 0x01;
        goto EXIT;
    }

    log_file = CreateLogHandle();
    if (log_file == NULL)
    {
        // Error: The single log object initialize failed.
        result = HResult_CUSTOM_7F_FAIL + 0x02;
        goto EXIT;
    }

    SetLogOutput(log_file, LOG_OUTPUT_FILE, log_filepath, LOG_NO_OUTPUTFUNC);
    SetLogLevel(log_file, LOG_LEVEL_INFO);
    SetLogStyles(log_file, LOG_STYLES_DOG, LOG_NO_STYLEFUNC);
    AddLogToLogs(gs, "file", log_file);

    log_stdout = CreateLogHandle();
    if (log_stdout == NULL)
    {
        // Error: The single log object initialize failed.
        result = HResult_CUSTOM_7F_FAIL + 0x02;
        goto EXIT;
    }

    SetLogOutput(log_stdout, LOG_OUTPUT_STDOUT, NULL, LOG_NO_OUTPUTFUNC);
    SetLogLevel(log_stdout, LOG_LEVEL_DEBUG);
    SetLogCustLabel(log_stdout, 1, "COREIB1");
    SetLogStyles(log_stdout, LOG_STYLES_EVENT2, LOG_NO_STYLEFUNC);
    AddLogToLogs(gs, "stdout", log_stdout);

EXIT:
    return result;
}


static void logging_iLog3_release()
{
    // Disable for Linux platform access violation issue.
    // On Win32 platform this line of code works well but seems no problem when remark it.
    DestroyLogsHandleG();
}
