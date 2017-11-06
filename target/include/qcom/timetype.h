/**
 * @file
 */
/******************************************************************************
 * Copyright (c) 2013 Qualcomm Atheros, Inc.
 * All Rights Reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 * $ATH_LICENSE_HOSTSDK0_C$

 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the license is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the license for the specific language governing permissions and
 *    limitations under the license.
 ******************************************************************************/

#ifndef __TIMETYPE_H__
#define __TIMETYPE_H__

//#endif

#ifndef __suseconds_t
typedef unsigned long __suseconds_t;
#endif
#if 1
//#ifndef time_t
#ifndef  __time_t_defined
#define  __time_t_defined
typedef unsigned long time_t;
#endif

struct timeval
{
time_t tv_sec;        /* Seconds. */
time_t tv_usec;    /* Microseconds. */
};
#endif

#endif
