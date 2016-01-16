/*
 * logging.h - Centralised logging. Part of the Linux-NTFS project.
 *
 * Copyright (c) 2005 Richard Russon
 *
 * This program/include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program/include file is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the main directory of the Linux-NTFS
 * distribution in the file COPYING); if not, write to the Free Software
 * Foundation,Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <cyg/infra/diag.h>	// HAL polled output

//#define NTFS_LOG_DEBUG_ONLY
#define NTFS_LOG_NONE
//#define NTFS_LOG_ERROR_ONLY
//#define NTFS_LOG_TRACE_ONLY
//#define NTFS_LOG_ALL

#ifdef NTFS_LOG_ALL
#define NTFS_LOG_CRITICAL
#define NTFS_LOG_ERROR
//    #define NTFS_LOG_INFO
#define NTFS_LOG_PERROR
#define NTFS_LOG_PROGRESS
#define NTFS_LOG_QUIET
#define NTFS_LOG_VERBOSE
#define NTFS_LOG_WARNING
#define NTFS_LOG_DEBUG
#define NTFS_LOG_TRACE
#define NTFS_LOG_ENTER
#define NTFS_LOG_LEAVE
#elif defined(NTFS_LOG_NORMAL)
#define NTFS_LOG_ERROR
#define NTFS_LOG_PERROR
#define NTFS_LOG_DEBUG
#define NTFS_LOG_TRACE
#define NTFS_LOG_ENTER
#define NTFS_LOG_LEAVE
#elif defined(NTFS_LOG_DEBUG_ONLY)
#define NTFS_LOG_DEBUG
#define NTFS_LOG_ENTER
#define NTFS_LOG_LEAVE
#elif defined(NTFS_LOG_TRACE_ONLY)
#define NTFS_LOG_TRACE
#define NTFS_LOG_ENTER
#define NTFS_LOG_LEAVE
#elif defined(NTFS_LOG_ERROR_ONLY)
#define NTFS_LOG_ERROR
#define NTFS_LOG_PERROR
#elif defined(NTFS_LOG_NONE)

#else
#error "NTFS_LOG is not defined."
#endif

//#define NTFS_LOG_ERROR
//#define NTFS_LOG_PERROR

#ifdef NTFS_LOG_CRITICAL
#define ntfs_log_critical(FORMAT, ARGS...) diag_printf("<critical>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_critical(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_ERROR
#define ntfs_log_error(FORMAT, ARGS...) diag_printf("<error>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_error(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_INFO
#define ntfs_log_info(FORMAT, ARGS...) diag_printf("<info>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_info(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_PERROR
#define ntfs_log_perror(FORMAT, ARGS...) diag_printf("<perror>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_perror(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_PROGRESS
#define ntfs_log_progress(FORMAT, ARGS...) diag_printf("<progress>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_progress(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_QUIET
#define ntfs_log_quiet(FORMAT, ARGS...) diag_printf("<quiet>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_quiet(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_VERBOSE
#define ntfs_log_verbose(FORMAT, ARGS...) diag_printf("<verbose>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_verbose(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_WARNING
#define ntfs_log_warning(FORMAT, ARGS...) diag_printf("<warn>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_warning(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_DEBUG
#define ntfs_log_debug(FORMAT, ARGS...) diag_printf("<debug>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_debug(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_TRACE
#define ntfs_log_trace(FORMAT, ARGS...) diag_printf("<trace>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_trace(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_ENTER
#define ntfs_log_enter(FORMAT, ARGS...) diag_printf("<enter>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_enter(FORMAT, ARGS...)do {} while (0)
#endif

#ifdef NTFS_LOG_LEAVE
#define ntfs_log_leave(FORMAT, ARGS...) diag_printf("<leave>: ""%s()""[%d]              "FORMAT, __FUNCTION__, __LINE__, ##ARGS)
#else
#define ntfs_log_leave(FORMAT, ARGS...)do {} while (0)
#endif

#endif				/* _LOGGING_H_ */
