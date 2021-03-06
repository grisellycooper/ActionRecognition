#ifndef HARALICK_H
#define HARALICK_H

#include "common.h"

using namespace std;

class Haralick {
    cv::Mat compute(const cv::Mat & mat) {
        cv::Mat output = cv::Mat::zeros(1, 15, CV_32F);
        output.at<float>(0, 0) = f1ASM(mat);

        output.at<float>(0, 1) = f2Contrast(mat);
        output.at<float>(0, 2) = f3Correlation(mat);
        output.at<float>(0, 3) = f4Variance(mat);
        output.at<float>(0, 4) = f5IDM(mat);
        output.at<float>(0, 5) = f6SumAverage(mat);
        output.at<float>(0, 6) = f7SumVariance(mat);
        output.at<float>(0, 7) = f8SumEntropy(mat);
        output.at<float>(0, 8) = f9Entropy(mat);
        output.at<float>(0, 9) = f10DifferenceVariance(mat);
        output.at<float>(0, 10) = f11DifferenceEntropy(mat);
        output.at<float>(0, 11) = f12InformationCorrelation01(mat);
        output.at<float>(0, 12) = f13InformationCorrelation02(mat);
        output.at<float>(0, 13) = 0.0f;
        output.at<float>(0, 14) = f15_Directionality(mat);

        for (int i = 0; i < 15; i++)
            if (isnan(output.at<float>(0, i))) output.at<float>(0, i) = 0.0f;

        return output;


        return output;
    }

    static float f1ASM(const cv::Mat& mat) {
        float sum = 0.0;

        for (auto i = 0; i < mat.rows; ++i)
            for (auto j = 0; j < mat.cols; ++j)
            sum += mat.at<float>(i, j) * mat.at<float>(i, j);

        return sum;
    }

    static float f2Contrast(const cv::Mat& mat) {
        float sum = 0.0, bigsum = 0.0;

        for (auto k = 0; k < mat.rows; ++k) {
            for (auto i = 0; i < mat.cols; ++i) {
                for (auto j = 0; j < mat.rows; ++j) {
                    if ((i - j) == k || (j - i) == k) {
                        sum += mat.at<float>(i, j);
                    }
                }
            }
            bigsum += k * k * sum;
            sum = 0;
        }
        return bigsum;
    }

    static float f3Correlation(const cv::Mat& mat) {
        std::vector<float> px(mat.rows, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                px[i] += mat.at<float>(i, j);
            }
        }

        float meanX = 0.0, sumSqrX = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            meanX += px[i] * i;
            sumSqrX += px[i] * i * i;
        }

        auto meanY = meanX;
        auto stdDevX = sqrt(sumSqrX - (meanX * meanX));
        stdDevX += static_cast<float>(static_cast<float>(HARALICK_EPSILON));
        auto stdDevY = stdDevX;

        float sum = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                sum += i * j * mat.at<float>(i, j);
            }
        }
        return (sum - meanX * meanY) / (stdDevX * stdDevY);

    }

    static float f4Variance(const cv::Mat& mat) {
        float mean = 0;
        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                mean += i * mat.at<float>(i, j);
            }
        }


        float variance = 0;
        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                variance += (i + 1 - mean) * (i + 1 - mean) * mat.at<float>(i, j);
            }
        }
        return variance;
    }

    static float f5IDM(const cv::Mat& mat) {
        float idm = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                idm += mat.at<float>(i, j) / (1 + (i - j) * (i - j));
            }
        }

        return idm;
    }

    static float f6SumAverage(const cv::Mat& mat) {
        vector<float> pXY(mat.rows + mat.cols + 1, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) pXY[i + j + 2] += mat.at<float>(i, j);
        }


        float sumAvg = 0;
        for (auto i = 2; i <= mat.rows + mat.cols; ++i) {
            sumAvg += i * pXY[i];
        }

        return sumAvg;
    }

    static float f7SumVariance(const cv::Mat& mat) {
        std::vector<float> pXY(mat.rows + mat.cols + 1, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                pXY[i + j + 2] += mat.at<float>(i, j);
            }
        }

        float f6SumAvg = 0.0;
        for (auto i = 2; i <= mat.rows + mat.cols; ++i) {
            f6SumAvg += i * pXY[i];
        }

        float sumVariance = 0.0;
        for (auto i = 2; i <= mat.rows + mat.cols; ++i) {
            sumVariance += (i - f6SumAvg) * (i - f6SumAvg) * pXY[i];
        }

        return sumVariance;
    }

    static float f8SumEntropy(const cv::Mat& mat) {
        std::vector<float> pXY(mat.rows + mat.cols + 1, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                pXY[i + j + 2] += mat.at<float>(i, j);
            }
        }

        float sumEntropy = 0.0;
        for (auto i = 2; i <= mat.rows + mat.cols; ++i) {
            sumEntropy += pXY[i] * log10(pXY[i] + static_cast<float>(HARALICK_EPSILON));
        }

        return -sumEntropy;
    }

    static float f9Entropy(const cv::Mat& mat) {
        float entropy = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                entropy +=
                    mat.at<float>(i, j) *
                        log10(mat.at<float>(i, j) + static_cast<float>(HARALICK_EPSILON));
            }
        }
        return -entropy;
    }

    static float f10DifferenceVariance(const cv::Mat& mat) {
        vector<float> pXY(mat.rows + mat.cols + 1, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                pXY[abs(i - j)] += mat.at<float>(i, j);
            }
        }

        /* Now calculate the variance of Pxpy (Px-y) */
        float sum = 0.0;
        float sumSqr = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            sum += pXY[i];
            sumSqr += pXY[i] * pXY[i];
        }
        float tmp = static_cast<float>(mat.rows * mat.cols);
        float diffVar = ((tmp * sumSqr) - (sum * sum)) / (tmp * tmp);

        return diffVar;
    }

    static float f11DifferenceEntropy(const cv::Mat& mat) {
        vector<float> pXY(mat.rows + mat.cols + 1, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                pXY[abs(i - j)] += mat.at<float>(i, j);
            }
        }

        float sum = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            sum += pXY[i] * log10(pXY[i] + static_cast<float>(HARALICK_EPSILON));
        }

        return -sum;
    }

    static float f12InformationCorrelation01(const cv::Mat& mat) {
        vector<float> pX(mat.rows, 0);
        vector<float> pY(mat.cols, 0);

        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                pX[i] += mat.at<float>(i, j);
                pY[j] += mat.at<float>(i, j);
            }
        }

        float hx = 0, hy = 0, hxy = 0, hxy1 = 0, hxy2 = 0;
        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                hxy1 -= mat.at<float>(i, j) *
                        log10(pX[i] * pY[j] + static_cast<float>(HARALICK_EPSILON));
                hxy2 -= pX[i] * pY[j] *
                        log10(pX[i] * pY[j] + static_cast<float>(HARALICK_EPSILON));
                hxy -= mat.at<float>(i, j) *
                        log10(mat.at<float>(i, j) + static_cast<float>(HARALICK_EPSILON));
            }
        }

        for (auto i = 0; i < mat.rows; ++i) {
            hx -= pX[i] * log10(pX[i] + static_cast<float>(HARALICK_EPSILON));
            hy -= pY[i] * log10(pY[i] + static_cast<float>(HARALICK_EPSILON));
        }

        return ((hxy - hxy1) / (hx > hy ? hx : hy));
    }

    static float f13InformationCorrelation02(const cv::Mat& mat) {

        vector<float> pX(mat.rows, 0);
        vector<float> pY(mat.cols, 0);


        for (auto i = 0; i < mat.rows; ++i) {
            for (auto j = 0; j < mat.cols; ++j) {
                pX[i] += mat.at<float>(i, j);
                pY[j] += mat.at<float>(i, j);
            }
        }

        float hx = 0, hy = 0, hxy = 0, hxy1 = 0, hxy2 = 0;
        for (auto i = 0; i < mat.rows; ++i)
            for (auto j = 0; j < mat.cols; ++j) {
                hxy1 -= mat.at<float>(i, j) *
                        log10(pY[i] * pY[j] + static_cast<float>(HARALICK_EPSILON));
                hxy2 -= pX[i] * pY[j] *
                        log10(pX[i] * pY[j] + static_cast<float>(HARALICK_EPSILON));
                hxy -= mat.at<float>(i, j) *
                        log10(mat.at<float>(i, j) + static_cast<float>(HARALICK_EPSILON));
            }

        for (auto i = 0; i < mat.rows; ++i) {
            hx -= pX[i] * log10(pX[i] + static_cast<float>(HARALICK_EPSILON));
            hy -= pY[i] * log10(pY[i] + static_cast<float>(HARALICK_EPSILON));
        }

        return static_cast<float>(sqrt(abs(1 - exp(-2.0 * (hxy2 - hxy)))));

    }

    static float f15_Directionality(const cv::Mat& mat) {
        float sum = 0.0;
        for (auto i = 0; i < mat.rows; ++i) {
            sum += mat.at<float>(i, i);
        }

        return sum;
    }
};

#endif  // HARALICK_H
