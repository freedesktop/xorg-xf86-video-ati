/*
 * Copyright © 2007 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "radeon_dri_bufmgr.h"

/** @file dri_bufmgr.c
 *
 * Convenience functions for buffer management methods.
 */

dri_bo *
dri_bo_alloc(dri_bufmgr *bufmgr, const char *name, unsigned long size,
	     unsigned int alignment, uint64_t location_mask)
{
   return bufmgr->bo_alloc(bufmgr, name, size, alignment, location_mask);
}

dri_bo *
dri_bo_alloc_static(dri_bufmgr *bufmgr, const char *name, unsigned long offset,
		    unsigned long size, void *virtual,
		    uint64_t location_mask)
{
   return bufmgr->bo_alloc_static(bufmgr, name, offset, size, virtual,
				  location_mask);
}

void
dri_bo_reference(dri_bo *bo)
{
   bo->bufmgr->bo_reference(bo);
}

void
dri_bo_unreference(dri_bo *bo)
{
   if (bo == NULL)
      return;

   bo->bufmgr->bo_unreference(bo);
}

int
dri_bo_map(dri_bo *buf, int write_enable)
{
   return buf->bufmgr->bo_map(buf, write_enable);
}

int
dri_bo_unmap(dri_bo *buf)
{
   return buf->bufmgr->bo_unmap(buf);
}

void
dri_fence_wait(dri_fence *fence)
{
   fence->bufmgr->fence_wait(fence);
}

void
dri_fence_reference(dri_fence *fence)
{
   fence->bufmgr->fence_reference(fence);
}

void
dri_fence_unreference(dri_fence *fence)
{
   if (fence == NULL)
      return;

   fence->bufmgr->fence_unreference(fence);
}

void
dri_bo_subdata(dri_bo *bo, unsigned long offset,
	       unsigned long size, const void *data)
{
   if (size == 0 || data == NULL)
      return;

   dri_bo_map(bo, 1);
   memcpy((unsigned char *)bo->virtual + offset, data, size);
   dri_bo_unmap(bo);
}

void
dri_bo_get_subdata(dri_bo *bo, unsigned long offset,
		   unsigned long size, void *data)
{
   if (size == 0 || data == NULL)
      return;

   dri_bo_map(bo, 0);
   memcpy(data, (unsigned char *)bo->virtual + offset, size);
   dri_bo_unmap(bo);
}

void
dri_bufmgr_destroy(dri_bufmgr *bufmgr)
{
   bufmgr->destroy(bufmgr);
}


int dri_emit_reloc(dri_bo *reloc_buf, uint64_t flags, uint32_t delta,
		    uint32_t offset, dri_bo *target_buf)
{
   return reloc_buf->bufmgr->emit_reloc(reloc_buf, flags, delta, offset, target_buf);
}

void *dri_process_relocs(dri_bo *batch_buf, uint32_t *count)
{
   return batch_buf->bufmgr->process_relocs(batch_buf, count);
}

void dri_post_submit(dri_bo *batch_buf, dri_fence **last_fence)
{
   batch_buf->bufmgr->post_submit(batch_buf, last_fence);
}

void
dri_bufmgr_set_debug(dri_bufmgr *bufmgr, int enable_debug)
{
   bufmgr->debug = enable_debug;
}

int
dri_bufmgr_check_aperture_space(struct radeon_space_check *bos, int num_bo)
{
  if (num_bo == 0)
    return BUFMGR_SPACE_OK;

  return bos[0].buf->bufmgr->check_aperture_space(bos, num_bo);
}

int dri_bo_pin(dri_bo *bo, int domain)
{
    return bo->bufmgr->pin(bo, domain);
}

void dri_bo_unpin(dri_bo *bo)
{
    bo->bufmgr->unpin(bo);
}

uint32_t dri_bo_get_handle(dri_bo *bo)
{
    return bo->bufmgr->get_handle(bo);
}