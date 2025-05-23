/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2005-2007, Joseph Koshy
 * Copyright (c) 2007 The FreeBSD Foundation
 * All rights reserved.
 *
 * Copyright (c) 2014 Netflix, Inc.
 * Written by: Adrian Chadd <adrian@FreeBSD.org>
 *
 * Portions of this software were developed by A. Joseph Koshy under
 * sponsorship from the FreeBSD Foundation and Google, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * CHERI CHANGES START
 * {
 *   "updated": 20180629,
 *   "target_type": "prog",
 *   "changes": [
 *     "pointer_as_integer"
 *   ]
 * }
 * CHERI CHANGES END
 */

/*
 * Transform a hwpmc(4) log into human readable form, and into
 * gprof(1) compatible profiles.
 */

#include <sys/param.h>
#include <sys/endian.h>
#include <sys/gmon.h>
#include <sys/imgact_aout.h>
#include <sys/imgact_elf.h>
#include <sys/mman.h>
#include <sys/pmc.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include <netinet/in.h>

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <gelf.h>
#include <libgen.h>
#include <limits.h>
#include <netdb.h>
#include <pmc.h>
#include <pmclog.h>
#include <sysexits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pmcstat.h"
#include "pmcstat_log.h"
#include "pmcpl_annotate_cg.h"

/*
 * Record a callchain.
 */

void
pmcpl_annotate_cg_process(struct pmcstat_process *pp, struct pmcstat_pmcrecord *pmcr,
    uint32_t nsamples, ptraddr_t *cc, int usermode, uint32_t cpu)
{
	struct pmcstat_pcmap *map;
	struct pmcstat_symbol *sym;
	ptraddr_t newpc;
	struct pmcstat_image *image;
	int i;
	char filename[PATH_MAX], funcname[PATH_MAX];
	unsigned sline;

	(void) pmcr; (void) nsamples; (void) usermode; (void) cpu;

	for (i = 0; i < (int) nsamples; i++) {
		map = NULL;
		sym = NULL;
		image = NULL;
		filename[0] = '\0';
		funcname[0] = '\0';
		sline = 0;

		map = pmcstat_process_find_map(usermode ? pp : pmcstat_kernproc, cc[i]);
		if (map != NULL) {
			assert(cc[i] >= map->ppm_lowpc && cc[i] < map->ppm_highpc);
			image = map->ppm_image;
			newpc = cc[i] - (map->ppm_lowpc +
				(image->pi_vaddr - image->pi_start));
			sym = pmcstat_symbol_search(image, newpc);
		}

		if (map != NULL && image != NULL && sym != NULL) {
			(void) pmcstat_image_addr2line(image, cc[i],
			    filename, sizeof(filename), &sline, funcname, sizeof(funcname));
		}

		if (map != NULL && sym != NULL) {
			fprintf(args.pa_graphfile, "%p %s %s:%d\n",
			    (void *)(intptr_t)cc[i],
			    funcname,
			    filename,
			    sline);
		} else {
			fprintf(args.pa_graphfile, "%p <unknown> ??:0\n",
			    (void *)(intptr_t)cc[i]);
		}
	}
	fprintf(args.pa_graphfile, "--\n");
}
