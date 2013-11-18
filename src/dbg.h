#ifndef __DBG_H_
#define __DBG_H_
/**
 * Based on Zed Shaw's debug macros:
 * http://c.learncodethehardway.org/book/ex20.html 
 *
 *   Relies on the following convention: errors result in a jump to an
 * error label, where cleanup is handled.  
 */

#include <stdio.h>
#include <errno.h>
#include <string.h>

// Debug level logging, can be compiled conditionally.
#ifdef NDEBUG
#define LOG_DEBUG(M, ...)
#else
#define LOG_DEBUG(M, ...) fprintf(stderr, "[DEBUG] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

// Helper macro to generate errno strings.
#define clean_errno() (errno == 0 ? "None" : strerror(errno))

// Logging macros.
#define LOG_ERROR(M, ...) fprintf(stderr, "[ERROR] (%s:%d errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define LOG_WARN(M, ...)  fprintf(stderr, "[WARN] (%s:%d errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)
#define LOG_INFO(M, ...)  fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Check that expression A is true, otherwise log message M and jump to error.
#define CHECK(A, M, ...) if(!(A)) { LOG_ERROR(M, ##__VA_ARGS__); errno=0; goto error; }
// Log the message M if this branch is ever reached, then jump to error.
#define SENTINEL(M, ...) { LOG_ERROR(M, ##__VA_ARGS__); errno=0; goto error; }

// Check that a pointer given by (m/c/re)alloc is valid.  Do not use for checking pointers given by user.
#define CHECK_MEM(A) { CHECK(A, "Out of memory.") }
// A varient of CHECK that uses debug level logging.  Use for errors you expect to occur in production and don't want spamming stderr.
#define CHECK_DEBUG(A, M, ...) if(!(A)) { LOG_DEBUG(M, ##__VA_ARGS__); errno=0; goto error; }

#endif