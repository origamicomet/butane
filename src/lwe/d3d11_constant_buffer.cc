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

#include <lwe/d3d11_constant_buffer.h>
#include <lwe/d3d11_render_device.h>
#include <lwe/d3d11_map_flags.h>

lwe_constant_buffer_t* lwe_constant_buffer_create(
  uint32_t flags,
  lwe_size_t num_bytes,
  void* buffer )
{
  lwe_d3d11_constant_buffer_t* constant_buffer =
    (lwe_d3d11_constant_buffer_t*)lwe_alloc(sizeof(lwe_d3d11_constant_buffer_t));

  const D3D11_USAGE usage = (flags & LWE_BUFFER_FLAGS_IMMUTABLE)
    ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;

  D3D11_BUFFER_DESC bd;
  bd.ByteWidth = num_bytes;
  bd.Usage = usage;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bd.CPUAccessFlags = (usage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
  bd.MiscFlags = 0;
  bd.StructureByteStride = 0;

  D3D11_SUBRESOURCE_DATA srd;
  srd.pSysMem = buffer;
  srd.SysMemPitch = 0;
  srd.SysMemSlicePitch = 0;

  HRESULT hr;

  lwe_fail_if(
    FAILED(hr = _d3d11_device->CreateBuffer(&bd, &srd, &constant_buffer->buffer)),
    "ID3D11Device::CreateBuffer failed, hr=%#08X", hr
  );

  return (lwe_constant_buffer_t*)constant_buffer;
}

void* lwe_constant_buffer_map(
  lwe_constant_buffer_t* constant_buffer_,
  uint32_t flags )
{
  lwe_assert(constant_buffer_ != NULL);

  lwe_d3d11_constant_buffer_t* constant_buffer =
    (lwe_d3d11_constant_buffer_t*)constant_buffer_;

  HRESULT hr;
  D3D11_MAPPED_SUBRESOURCE msbr;

  lwe_fail_if(FAILED(
    hr =_d3d11_context->Map(
      constant_buffer->buffer, 0,
      lwe_map_flags_to_d3d(flags), 0,
      &msbr
    )),

    "ID3D11DeviceContext::Map failed, hr=%#08X", hr
  );

  return msbr.pData;
}

void lwe_constant_buffer_unmap(
  lwe_constant_buffer_t* constant_buffer_ )
{
  lwe_assert(constant_buffer_ != NULL);

  lwe_d3d11_constant_buffer_t* constant_buffer =
    (lwe_d3d11_constant_buffer_t*)constant_buffer_;

  _d3d11_context->Unmap(constant_buffer->buffer, 0);
}

void lwe_constant_buffer_destroy(
  lwe_constant_buffer_t* constant_buffer_ )
{
  lwe_assert(constant_buffer_ != NULL);

  lwe_d3d11_constant_buffer_t* constant_buffer =
    (lwe_d3d11_constant_buffer_t*)constant_buffer_;

  constant_buffer->buffer->Release();

  lwe_free((void*)constant_buffer);
}