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


#include "algorithm.hpp"
// c++
#include <string>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace ssig {

Algorithm::Algorithm() {}

Algorithm::~Algorithm() {}

Algorithm::Algorithm(const Algorithm& rhs) {}

Algorithm& Algorithm::operator=(const Algorithm& rhs) {
  return *this;
}

void Algorithm::load(const std::string& filename,
                     const std::string& nodename) {
  cv::FileStorage fileStorage;
  fileStorage.open(filename, cv::FileStorage::READ);
  cv::FileNode node;
  if (!nodename.empty())
    node = fileStorage[nodename];
  else
    node = fileStorage.root();
  this->read(node);
  fileStorage.release();
}

void Algorithm::save(const std::string& filename,
                     const std::string& nodename) const {
  cv::FileStorage fileStorage;
  fileStorage.open(filename, cv::FileStorage::WRITE);
  if (!nodename.empty()) {
    fileStorage << nodename << "{";

    write(fileStorage);

    fileStorage << "}";
  } else {
    write(fileStorage);
  }
  fileStorage.release();
}

void Algorithm::verboseLog(FILE* file, const char* format, ...) const {
  if (mVerbose) {
    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);
  }
}

void Algorithm::vVerboseLog(
  FILE* file,
  const char* format, va_list args) const {
  if (mVerbose) {
    vfprintf(file, format, args);
  }
}

void Algorithm::verboseLog(const char* format, ...) const {
  va_list args;
  va_start(args, format);
  vVerboseLog(stdout, format, args);
  va_end(args);
}

void Algorithm::setUseOpenCl(bool state) {
  mOpenClEnabled = state;
}

bool Algorithm::getVerbose() const {
  return mVerbose;
}

void Algorithm::setVerbose(const bool verbose) {
  mVerbose = verbose;
}
}  // namespace ssig
