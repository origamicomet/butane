// =============================================================================
// This file is part of LWE. See readme.md for details.
//
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// =============================================================================

#ifndef _LWE_RENDER_DEVICE_H_
#define _LWE_RENDER_DEVICE_H_

#include <lwe/foundation.h>
#include <lwe/render_command.h>
#include <lwe/render_command_stream.h>

struct lwe_swap_chain_t;
struct lwe_constant_buffer_t;

extern void lwe_render_device_create(
  uint32_t adapter_id );

extern void lwe_render_device_dispatch(
  lwe_size_t num_constant_buffers,
  struct lwe_constant_buffer_t** constant_buffers,
  struct lwe_swap_chain_t* swap_chain,
  lwe_size_t num_streams,
  const lwe_render_cmd_stream_t** streams );

#endif // _LWE_RENDER_DEVICE_H_