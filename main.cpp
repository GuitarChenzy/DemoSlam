#include <iostream>
#include <opencv2/opencv.hpp>
#include "SLAM.h"

using namespace std;

const string config_path = "../config/config.yaml";

void loadImages(const cv::FileStorage fsSettings, vector<string> &imageLeftPath,
                vector<string> &imageRightPath, vector<double> &imageTimestampsPath);

int main()
{
    cout << "loading config......" << endl;
    cv::FileStorage fsSettings(config_path.c_str(), cv::FileStorage::READ);
    if (!fsSettings.isOpened())
    {
        cerr << "Failed to open settings file at: " << config_path << endl;
        exit(-1);
    }
    vector<string> imageLeftPath, imageRightPath;
    string image_path;
    vector<double> imageTimestamps;
    loadImages(fsSettings, imageLeftPath, imageRightPath, imageTimestamps);
    cout << imageTimestamps.size() << "\n"
         << imageLeftPath.size() << "\t" << imageLeftPath[0] << "\nloading end" << endl;

    const int nImages = imageLeftPath.size();
    cv::Mat leftImage, rightImage;
    SLAM slam;
    slam.initial(fsSettings);
    for (int i =  0; i < 10; i++)
    {
        leftImage = cv::imread(imageLeftPath[i]);
        rightImage = cv::imread(imageRightPath[i]);
        double frameTime = imageTimestamps[i];
        
    }

    return 0;
}

void loadImages(const cv::FileStorage fsSettings, vector<string> &imageLeftPath, vector<string> &imageRightPath, vector<double> &imageTimestamps)
{
    string timesPath = fsSettings["Times.path"];
    string sequencePath = fsSettings["Image.path"];
    ifstream fTimes;
    fTimes.open(timesPath.c_str());
    while (!fTimes.eof())
    {
        string s;
        getline(fTimes, s);
        if (!s.empty())
        {
            stringstream ss;
            ss << s;
            double t;
            ss >> t;
            imageTimestamps.push_back(t);
        }
    }

    string strPrefixLeft = sequencePath + "/image_0/";
    string strPrefixRight = sequencePath + "/image_1/";

    const int nTimes = imageTimestamps.size();
    imageLeftPath.resize(nTimes);
    imageRightPath.resize(nTimes);

    for (int i = 0; i < nTimes; i++)
    {
        stringstream ss;
        ss << setfill('0') << setw(6) << i;
        imageLeftPath[i] = strPrefixLeft + ss.str() + ".png";
        imageRightPath[i] = strPrefixRight + ss.str() + ".png";
    }
}