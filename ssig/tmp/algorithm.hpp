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

#ifndef _SSIG_CORE_ALGORITHM_HPP_
#define _SSIG_CORE_ALGORITHM_HPP_
// c++
#include <string>
#include <cstdarg>
// opencv
#include <opencv2/core.hpp>
// ssiglib
#include "core_defs.hpp"
#include "resource.hpp"

namespace ssig {

class Algorithm : public Resource {
 public:
  CORE_EXPORT Algorithm(void);
  CORE_EXPORT virtual ~Algorithm(void) = 0;
  CORE_EXPORT Algorithm(const Algorithm& rhs);
  CORE_EXPORT Algorithm& operator=(const Algorithm& rhs);

  CORE_EXPORT virtual void load(
    const std::string& filename,
    const std::string& nodename);

  CORE_EXPORT virtual void save(
    const std::string& filename,
    const std::string& nodename) const;

  /**
  * @brief: this function can be used to verboseLog messages
   to file when setVerbose(true) is called.
  **/
  CORE_EXPORT void verboseLog(
    FILE* file,
    const char* format,
    ...) const;
  /**
  * @brief: Overloaded method provided to verboseLog to stdout
  **/
  CORE_EXPORT void verboseLog(const char* format, ...) const;

  CORE_EXPORT void setUseOpenCl(bool state);
  CORE_EXPORT bool getVerbose() const;
  CORE_EXPORT void setVerbose(const bool verbose);

 protected:
  CORE_EXPORT virtual void read(const cv::FileNode& fn) = 0;
  CORE_EXPORT virtual void write(cv::FileStorage& fs) const = 0;
  CORE_EXPORT void vVerboseLog(
    FILE* file,
    const char* format,
    va_list args) const;
  bool mOpenClEnabled = false;
  bool mVerbose = false;
};

}  // namespace ssig
#endif  // !_SSIG_CORE_ALGORITHM_HPP_
