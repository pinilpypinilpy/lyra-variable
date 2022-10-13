// Copyright 2021 Google LLC
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

#include "lyra_components.h"

#include <memory>

#include "feature_extractor_interface.h"
#include "generative_model_interface.h"
#include "lyra_gan_model.h"
#include "packet.h"
#include "packet_interface.h"
#include "residual_vector_quantizer.h"
#include "soundstream_encoder.h"
#include "vector_quantizer_interface.h"
#include "zero_feature_estimator.h"

namespace chromemedia {
namespace codec {
namespace {

//  LINT.IfChange
constexpr int kMaxNumPacketBits = 480;
// LINT.ThenChange(
// lyra_config.cc,
// residual_vector_quantizer.h,
// )

}  // namespace

std::unique_ptr<VectorQuantizerInterface> CreateQuantizer(
    int num_output_features, const ghc::filesystem::path& model_path) {
  return ResidualVectorQuantizer::Create(model_path);
}

std::unique_ptr<GenerativeModelInterface> CreateGenerativeModel(
    int num_samples_per_hop, int num_output_features,
    const ghc::filesystem::path& model_path) {
  return LyraGanModel::Create(model_path, num_output_features);
}

std::unique_ptr<FeatureExtractorInterface> CreateFeatureExtractor(
    int sample_rate_hz, int num_features, int num_samples_per_hop,
    int num_samples_per_window, const ghc::filesystem::path& model_path) {
  return SoundStreamEncoder::Create(model_path);
}

std::unique_ptr<PacketInterface> CreatePacket(int num_header_bits,
                                              int num_quantized_bits) {
  return Packet<kMaxNumPacketBits>::Create(num_header_bits, num_quantized_bits);
}

std::unique_ptr<FeatureEstimatorInterface> CreateFeatureEstimator(
    int num_features) {
  return std::make_unique<ZeroFeatureEstimator>(num_features);
}

}  // namespace codec
}  // namespace chromemedia
