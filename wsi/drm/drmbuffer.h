/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/
#ifndef WSI_DRMBUFFER_H_
#define WSI_DRMBUFFER_H_

#include <platformdefines.h>

#include "framebuffermanager.h"
#include "overlaybuffer.h"

#define METADATA(x) image_.handle_->meta_data_.x

namespace hwcomposer {

class NativeBufferHandler;

class DrmBuffer : public OverlayBuffer {
 public:
  DrmBuffer(DrmBuffer&& rhs) = default;
  DrmBuffer& operator=(DrmBuffer&& other) = default;

  DrmBuffer() = default;

  ~DrmBuffer() override;

  void InitializeFromNativeHandle(HWCNativeHandle handle,
                                  ResourceManager* buffer_manager) override;

  uint32_t GetDataSpace() const override {
    return METADATA(dataspace_);
  }

  uint32_t GetWidth() const override {
    return METADATA(width_);
  }

  uint32_t GetHeight() const override {
    return METADATA(height_);
  }

  uint32_t GetFormat() const override {
    return format_;
  }

  HWCLayerType GetUsage() const override {
    return METADATA(usage_);
  }

  uint32_t GetFb(bool* isNewCreated = NULL) override {
    if (image_.drm_fd_ == 0) {
      CreateFrameBuffer();
      if (isNewCreated && image_.drm_fd_)
        *isNewCreated = true;
    } else if (isNewCreated)
      *isNewCreated = false;
    return image_.drm_fd_;
  }

  uint32_t GetPrimeFD() const override {
    return METADATA(prime_fds_[0]);
  }

  const uint32_t* GetPitches() const override {
    return METADATA(pitches_);
  }

  const uint32_t* GetOffsets() const override {
    return METADATA(offsets_);
  }

  uint32_t GetTilingMode() const override {
    return METADATA(tiling_mode_);
  }

  void SetDataSpace(uint32_t dataspace) override {
    METADATA(dataspace_) = dataspace;
  }

  bool GetInterlace() override {
    return METADATA(is_interlaced_);
  }

  void SetInterlace(bool isInterlaced) override {
    METADATA(is_interlaced_) = isInterlaced;
  }

  const ResourceHandle& GetGpuResource(GpuDisplay egl_display,
                                       bool external_import) override;

  const ResourceHandle& GetGpuResource() override;

  const MediaResourceHandle& GetMediaResource(MediaDisplay display,
                                              uint32_t width,
                                              uint32_t height) override;

  bool CreateFrameBufferWithModifier(uint64_t modifier) override;

  HWCNativeHandle GetOriginalHandle() const override {
    return original_handle_;
  }

  void SetOriginalHandle(HWCNativeHandle handle) override;

  void Dump() override;

 private:
  void Initialize(const HwcMeta& meta);
  bool CreateFrameBuffer();
  uint32_t format_ = 0;
  uint32_t frame_buffer_format_ = 0;
  uint32_t previous_width_ = 0;   // For Media usage.
  uint32_t previous_height_ = 0;  // For Media usage.
  ResourceManager* resource_manager_ = 0;
  ResourceHandle image_;
  MediaResourceHandle media_image_;
  HWCNativeHandle original_handle_;
  FrameBufferManager* fb_manager_ = NULL;
};

}  // namespace hwcomposer
#endif  // WSI_DRMBUFFER_H_
