/* loci_tcpinfo.h - portable tcp_info shim */
/* Created: Mon Aug 25 05:19:12 PM PDT 2026 dulrik */

/* Copyright © 2026 SK Coder <dulrik@shatteredkingdoms.org>
 *
 * This file is part of LociTerm - Last Outpost Client Implementation Terminal
 *
 * LociTerm is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * LociTerm is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with LociTerm.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOCI_TCPINFO_H
#define LOCI_TCPINFO_H

#include <netinet/tcp.h>

/* Darwin/macOS doesn't have the Linux struct tcp_info, but its struct
 * tcp_connection_info is close enough for monitoring the rtt.  Note that
 * Darwin reports rtt in milliseconds where Linux uses microseconds, so the
 * accessor macros scale to keep the reported units the same. */
#ifdef __APPLE__
#define loci_tcp_info tcp_connection_info
#define LOCI_TCP_INFO TCP_CONNECTION_INFO
#define loci_tcpi_rtt(ti) ((ti).tcpi_srtt * 1000)
#define loci_tcpi_rttvar(ti) ((ti).tcpi_rttvar * 1000)
#else
#define loci_tcp_info tcp_info
#define LOCI_TCP_INFO TCP_INFO
#define loci_tcpi_rtt(ti) ((ti).tcpi_rtt)
#define loci_tcpi_rttvar(ti) ((ti).tcpi_rttvar)
#endif

#endif /* LOCI_TCPINFO_H */
