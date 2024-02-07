#ifndef __LOGGING_H
#define __LOGGING_H

/*Log levels*/

#define LOG_LEVEL_DEBUG     4
#define LOG_LEVEL_INFO      3
#define LOG_LEVEL_WARNING   2
#define LOG_LEVEL_ERROR     1
#define LOG_LEVEL_CRITICAL  0

/*Default log level*/

// #ifndef LOG_LEVEL
// #define LOG_LEVEL LOG_LEVEL_ERROR
// #endif

#define LOG_LEVEL LOG_LEVEL_DEBUG

/*Logging macros*/

#define LOG_CRITICAL(msg)
#define LOG_ERROR(msg)
#define LOG_WARNING(msg)
#define LOG_INFO(msg)
#define LOG_DEBUG(msg)

#if LOG_LEVEL >= LOG_LEVEL_CRITICAL
#undef LOG_CRITICAL 
#define LOG_CRITICAL(msg) logging_log(LOG_LEVEL, msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_ERROR
#undef LOG_ERROR
#define LOG_ERROR(msg) logging_log(LOG_LEVEL, msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_WARNING
#undef LOG_WARNING
#define LOG_WARNING(msg) logging_log(LOG_LEVEL, msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_INFO
#undef LOG_INFO
#define LOG_INFO(msg) logging_log(LOG_LEVEL, msg)
#endif

#if LOG_LEVEL >= LOG_LEVEL_DEBUG
#undef LOG_DEBUG
#define LOG_DEBUG(msg) logging_log(LOG_LEVEL, msg) 
#endif

/*Logging function*/
void logging_log(int severity, const char* msg);

#endif