#ifndef COOCCURRENCE_H
#define COOCCURRENCE_H

#include "common.h"

class CoOccurrenceMatrix
{
    cv::Mat_<float> M0, M45, M90, M135;
    int nbins;
    int distance;

  public:
    CoOccurrenceMatrix(int nbins, int distance)
    {
        this->nbins = nbins;
        this->distance = distance;

        M0.create(nbins, nbins);
        M45.create(nbins, nbins);
        M90.create(nbins, nbins);
        M135.create(nbins, nbins);
    }

    void GetMatrix0(const cv::Rect &patch, cv::Mat_<int> &img, cv::Mat &output)
    {
        float sum = 0.0f;
        int d, x, y;

        d = this->distance;

        M0 = cv::Mat::zeros(this->nbins, this->nbins, CV_32F);
        for (int row = patch.y; row < patch.height; row++)
        {
            for (int col = patch.x; col < patch.width; col++)
            {
                x = img(row, col);
                //std::cout<<"x: "<<x << std::endl;
                if (x == -1)
                    continue;

                if (col + d < patch.width)
                {
                    y = img(row, col + d);
                    //std::cout<<"y: "<<y << std::endl;
                    if (y == -1)
                        continue;

                    M0(y, x) ++;
                    M0(x, y) ++;
                    sum++;
                }
            }
        }

        cout << "sum: " << sum << endl;

        if (sum == 0.0f) {
            sum = 1.0f;
        }

        M0 = M0 / sum;

        output = M0.clone();
    }

    void GetMatrix45(const cv::Rect &patch, cv::Mat_<int> &img, cv::Mat &output)
    {
        float sum = 0.0f;
        int d, x, y;

        d = this->distance;

        M45 = cv::Mat::zeros(this->nbins, this->nbins, CV_32F);
        for (int row = patch.y; row < patch.height; row++)
        {
            for (int col = patch.x; col < patch.width; col++)
            {
                x = img(row, col);
                if (x == -1)
                    continue;

                if (row + d < patch.height && col - d >= patch.x)
                {
                    y = img(row + d, col - d);
                    if (y == -1)
                        continue;

                    M45(y, x) ++;
                    M45(x, y) ++;
                    sum++;
                }
            }
        }

        if (sum == 0.0f)
            sum = 1.0f;

        M45 = M45 / sum;

        output = M45.clone();
    }

    void GetMatrix90(const cv::Rect &patch, cv::Mat_<int> &img, cv::Mat &output)
    {
        float sum = 0.0f;
        int d, x, y;

        d = this->distance;

        M90 = cv::Mat::zeros(this->nbins, this->nbins, CV_32F);
        for (int row = patch.y; row < patch.height; row++)
        {
            for (int col = patch.x; col < patch.width; col++)
            {
                x = img(row, col);
                if (x == -1)
                    continue;

                if (row + d < patch.height)
                {
                    y = img(row + d, col);
                    if (y == -1)
                        continue;

                    M90(y, x) ++;
                    M90(x, y) ++;
                    sum++;
                }
            }
        }

        if (sum == 0.0f)
            sum = 1.0f;

        M90 = M90 / sum;

        output = M90.clone();
    }

    void GetMatrix135(const cv::Rect &patch, cv::Mat_<int> &img, cv::Mat &output)
    {
        float sum = 0.0f;
        int d, x, y;

        d = this->distance;

        M135 = cv::Mat::zeros(this->nbins, this->nbins, CV_32F);
        for (int row = patch.y; row < patch.height; row++)
        {
            for (int col = patch.x; col < patch.width; col++)
            {
                x = img(row, col);
                if (x == -1)
                    continue;

                if (row + d < patch.height && col + d < patch.width)
                {
                    y = img(row + d, col + d);
                    if (y == -1)
                        continue;

                    M135(y, x) ++;
                    M135(x, y) ++;
                    sum++;
                }
            }
        }

        if (sum == 0.0f)
            sum = 1.0f;

        M135 = M135 / sum;

        output = M135.clone();
    }

    void GetAllMatrices(const cv::Rect &patch, cv::Mat_<int> &img, std::vector<cv::Mat> &output_set)
    {
        cv::Mat single_output; // testar se vai precisar declarar o tamanho

        GetMatrix0(patch, img, single_output);
        GetMatrix45(patch, img, single_output);
        GetMatrix90(patch, img, single_output);
        GetMatrix135(patch, img, single_output);

        output_set.push_back(M0);
        output_set.push_back(M45);
        output_set.push_back(M90);
        output_set.push_back(M135);
    }
};

#endif // COOCCURRENCE_H
