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


#include "util.hpp"

#include <algorithm>
#include <functional>
#include <cctype>
#include <vector>
#include <locale>
#include <string>

namespace ssig {

  cv::Mat Util::convertToImg(const cv::Mat& m) {
    std::vector<cv::Mat> channels(3);
    cv::split(m, channels);
    for (auto& channel : channels) {
      double minVal = 0, maxVal = 0;
      cv::minMaxIdx(channel, &minVal, &maxVal);
      channel = channel - minVal;
      channel = channel / (maxVal - minVal);
    }
    cv::Mat ans;
    cv::merge(channels, ans);
    ans.convertTo(ans, CV_8UC3, 255);
    return ans;
  }

std::string Util::ltrim(std::string str) {
  str.erase(str.begin(),
            std::find_if(str.begin(), str.end(),
                         std::not1(std::ptr_fun<int, int>(std::isspace))));
  return str;
}

std::string Util::rtrim(std::string str) {
  str.erase(std::find_if(str.rbegin(), str.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace)))
                .base(),
            str.end());
  return str;
}

std::string Util::trim(std::string str) { return ltrim(rtrim(str)); }

void Util::reorder(const cv::Mat& collection, cv::Mat_<int>& ordering,
                   cv::Mat& out) {
  out = cv::Mat::zeros(collection.rows, collection.cols, collection.type());
  for (int i = 0; i < ordering.rows; ++i) {
    collection.row(ordering[i][0]).copyTo(out.row(i));
  }
}

}  // namespace ssig
