/* Copyright 2024 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef XPROF_UTILS_XPROF_GPU_COST_ANALYSIS_H_
#define XPROF_UTILS_XPROF_GPU_COST_ANALYSIS_H_

#include <cstdint>
#include <memory>

#include "absl/status/status.h"
#include "absl/strings/string_view.h"
#include "xla/hlo/ir/hlo_instruction.h"
#include "xla/service/gpu/model/gpu_hlo_cost_analysis.h"
#include "xla/service/hlo_cost_analysis.h"
#include "xprof/utils/hlo_cost_analysis_wrapper.h"

namespace tensorflow {
namespace profiler {

// XProfGpuCostAnalysis provides additional cost analysis for XProf, which
// normalizes the flops to the device flops based on input bit widths.
class XProfGpuCostAnalysis : public xla::gpu::GpuHloCostAnalysis {
 public:
  explicit XProfGpuCostAnalysis(const xla::HloCostAnalysis::Options& options)
      : xla::gpu::GpuHloCostAnalysis(options) {}

  absl::Status HandleCustomCall(const xla::HloInstruction* hlo) override;

  absl::Status Postprocess(const xla::HloInstruction* hlo) override;

  int64_t GetDeviceFlopsAdjustment(const xla::HloInstruction& hlo) const;

 protected:
  std::unique_ptr<xla::HloCostAnalysis> CreateNestedCostAnalysis() override;

  absl::Status DefaultPostprocess(const xla::HloInstruction* hlo);

 private:
  static inline constexpr absl::string_view kDeviceFlopsAdjustment =
      "device_flops_adjustment";
};

// Creates a HloCostAnalysisWrapper for XProfGpuCostAnalysis based on the given
// options.
std::unique_ptr<HloCostAnalysisWrapper> CreateXprofGpuCostAnalysis(
    xla::HloCostAnalysis::Options options = xla::HloCostAnalysis::Options());

}  // namespace profiler
}  // namespace tensorflow

#endif  // XPROF_UTILS_XPROF_GPU_COST_ANALYSIS_H_
