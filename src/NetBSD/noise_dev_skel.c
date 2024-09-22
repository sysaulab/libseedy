/*	$NetBSD: pseudo_dev_skel.c,v 1.2 2009/05/21 15:07:41 cube Exp $	*/

/*-
 * Copyright (c) 1998-2006 Brett Lymn (blymn@NetBSD.org)
 * All rights reserved.
 *
 * This code has been donated to The NetBSD Foundation by the Author.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software withough specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/device.h>
#include <sys/conf.h>
#include <sys/pseudo_dev_skel.h>
#include "../libseedy/libseedy64.h"
#include "../tools/algos/cc2032.h"
#include "../tools/algos/qx64.h"

/* Autoconfiguration glue */
void	noiseattach(int num);
int     noiseread(dev_t device, struct uio* u, int flags);
int     noiseopen(dev_t device, int flags, int fmt, struct lwp *process);
int     noiseclose(dev_t device, int flags, int fmt, struct lwp *process);
int     noiseioctl(dev_t device, u_long command, caddr_t data, int flags, struct lwp *process);

QX64 prng_qx64;
CC2032 prng_cc2032;
uint64_t seek_offset;
uint64_t seek_bank;

void mt_seeder(uint8_t* b, size_t n)
{
    cc2032_fill(&prng_cc2032,b,n);
}



/* just define the character device handlers because that is all we need */
const struct cdevsw noise_cdevsw = {
        noiseopen,
		noiseclose,
		noiseread,
	nowrite,
        noiseioctl,
	nostop,
	notty,
	nopoll,
	nommap,
	nokqfilter,
};

/*
 * Attach for autoconfig to find.  The parameter is the number given in
 * the configuration file, and defaults to 1.  New code should not expect
 * static configuration from the kernel config file, and thus ignore that
 * parameter.
 */
void
skeletonattach(int num)
{
	cc2032_init(&prng_cc2032, seedy64);
    cc2032_fill(&prng_cc2032, (uint8_t*)prng_qx64.pool, sizeof(prng_qx64.pool));
	seek_offset = 0;

	  /* nothing to do for skeleton, this is where resources that
	     need to be allocated/initialised before open is called
	     can be set up */
}

/*
 * Handle an open request on the device.
 */
int
skeletonopen(dev_t device, int flags, int fmt, struct lwp *process)
{
	return 0; /* this always succeeds */
}

/*
 * Handle the close request for the device.
 */
int
skeletonclose(dev_t device, int flags, int fmt, struct lwp *process)
{
	return 0; /* again this always succeeds */
}

/*
 * Handle the ioctl for the device
 */
int
skeletonioctl(dev_t device, u_long command, caddr_t data, int flags,
	      struct lwp *process)
{
	int error;
	struct skeleton_params *params = (struct skeleton_params *)data;

	error = 0;
	switch (command) {
		case SKELTEST:
			printf("Got number of %d and string of %s\n",
			       params->number, params->string);
			break;

		default:
			error = ENODEV;
	}

	return (error);
}