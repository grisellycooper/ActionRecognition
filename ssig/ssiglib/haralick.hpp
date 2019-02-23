/*L*****************************************************************************
*
*  Copyright (c) 2015, Smart Surveillance Interest Group, all rights reserved.
*
*  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
*
*  By downloading, copying, installing or using the software you agree to this
*  license. If you do not agree to this license, do not download, install, copy
*  or use the software.
*
*                Software License Agreement (BSD License)
*             For Smart Surveillance Interest Group Library
*                         http://ssig.dcc.ufmg.br
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*    1. Redistributions of source code must retain the above copyright notice,
*       this list of conditions and the following disclaimer.
*
*    2. Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*
*    3. Neither the name of the copyright holder nor the names of its
*       contributors may be used to endorse or promote products derived from
*       this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
*  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
*  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
*  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
*  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
*  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************L*/

#ifndef _SSIG_DESCRIPTORS_HARALICK_HPP_
#define _SSIG_DESCRIPTORS_HARALICK_HPP_

#include <opencv2/core.hpp>
#include "descriptors_defs.hpp"

#define HARALICK_EPSILON 0.00001

namespace ssig {
class Haralick {
 public:
  DESCRIPTORS_EXPORT static cv::Mat compute(const cv::Mat& mat);
  DESCRIPTORS_EXPORT static cv::Mat computeOld(const cv::Mat& mat);
 private:
  static float f1ASM(const cv::Mat& mat);
  static float f2Contrast(const cv::Mat& mat);
  static float f3Correlation(const cv::Mat& mat);
  static float f4Variance(const cv::Mat& mat);
  static float f5IDM(const cv::Mat& mat);
  static float f6SumAverage(const cv::Mat& mat);
  static float f7SumVariance(const cv::Mat& mat);
  static float f8SumEntropy(const cv::Mat& mat);
  static float f9Entropy(const cv::Mat& mat);
  static float f10DifferenceVariance(const cv::Mat& mat);
  static float f11DifferenceEntropy(const cv::Mat& mat);
  static float f12InformationCorrelation01(const cv::Mat& mat);
  static float f13InformationCorrelation02(const cv::Mat& mat);
  static float f15_Directionality(const cv::Mat& mat);
};
}  // namespace ssig
#endif  // !_SSIG_DESCRIPTORS_HARALICK_HPP_
