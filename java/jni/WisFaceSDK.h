#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "opencv2/opencv.hpp"

typedef struct WisFaceHadleStruct* WisFaceHandle;

using namespace std;
using namespace cv;

#define FEATURE_SIZE 256



typedef struct
{
    cv::Rect rtFace;
    cv::Point leftEye;
    cv::Point ptLeftEye;	// 左侧眼球坐标
    cv::Point ptRightEye;	// 右侧眼球坐标
    cv::Point ptNose;		// 鼻尖坐标
    cv::Point ptMouthLeft;	// 左侧嘴角坐标
    cv::Point ptMouthRight;	// 右侧嘴角坐标
    std::vector< cv::Point > landmark; // landmark

    float yaw;              // 偏航角 angle
    float roll;             // 翻滚角 angle
    float pitch;            // 俯仰角 angle
    float conf;             // confidence

    float ptFeature[FEATURE_SIZE];
} FaceModel;

namespace Qiansou {

class /*__declspec(dllimport)*/ CFaceWis
{
public:
    CFaceWis();
    ~CFaceWis();


    //Initial FaceEngine
    //return 0 success
    int Initial();

    /**
    * detect mutil faces(Max size is maxFaceNum)
    * @image, gray Mat
    * @faces, face results
    * @maxFaceNum, max faces will be detected and return
    */
    int DetectFace(cv::Mat image, FaceModel *faces, int maxFaceNum);

    /**
    * extract face feature (sigle face)
    * @grayImage,gray mat
    * @face, face postion in image
    * @prFeature, feature result,in|out param, prFeature.length = 256 * sizeof(float)
    */
    int ExtractFeature(cv::Mat grayImage, FaceModel &face);


   /**
    * Normalize face
    * @image, mat
    * @face, face model include face postion in image
    * @faceImage, normal crop face
    */
    int Normalize(cv::Mat image, FaceModel face, cv::Mat &faceImage);

    /**
    * Calculate similarity between 2 face feature
    * @fea1, float array, size 256
    * @fea2, float array, size 256
    * return similarity score
    */
    float Compare2Feature(float* fea1, float*fea2);

   /**
    * calculate two photo image similarity score
    * @imgFile1, photo file1
    * @imgFile2, photo file2
    * return similarity score
    */
    float Compare2Image(const char* imgFile1, const char* imgFile2);


    //fpm 人脸特征点
    int GetFacePoints(cv::Mat grayimage, FaceModel  &face);


    //detect and extract face and feature only for sigle face model
    int FaceProcess(cv::Mat image, FaceModel  &face);

public:
    /// <summary>
    /// 点头
    /// </summary>
    /// <param name="facePoints">人脸特征点</param>
    /// <returns>是否成功</returns>
    bool UpOrDown(FaceModel face);
    /// <summary>
    /// 左右转脸
    /// </summary>
    /// <param name="facePoints">特征点</param>
    /// <returns>是否成功</returns>
    bool LeftOrRright(FaceModel face);
    /// <summary>
    /// 眨眼
    /// </summary>
    /// <param name="facePoints">特征点</param>
    /// <returns>是否成功</returns>
    bool EyeOrEye(FaceModel face);
    /// <summary>
    /// 吐舌头
    /// </summary>
    /// <param name="facePoints">特征点</param>
    /// <returns>是否成功</returns>
    bool Mouth(FaceModel face);

    /// <summary>
    /// 人脸防伪检测
    /// </summary>
    /// <param name="engine">qs_Wis_FaceAntiFakeCreate()返回的人脸防伪引擎</param>
    /// <param name="image">照片imgRgb24</param>
    /// <param name="faces">返回的人脸模型包含人脸位置</param>
    /// <param name="FacePoseType">动作类型int（1~4）,1:眨眼 2：吐舌头 3：左右转脸 4：上下点头</param>
    /// <returns>返回检测状态：1表示检测成功 、0表示需要持续检测（控制检测超时要在外层应用进行判断）、 -1表示检测失败</returns>
    int AntiFakeDetect(cv::Mat img, FaceModel &face, int facePoseType);

public:

    void Dispose();
    void InitGlog();
private:
    int _Extract(float* prImage, float* prFeature);
    void _SetInputBlob(float* prImage);
    void _GetImageData(cv::Mat img, float* prImage);
private:
    void* _net;
    WisFaceHandle _detector;
    long _pointsEngine;
    void* _faceNomalizer;
    float *_prImageData;

private:
    // 最大幅度
    float _rangeMax;
    // 最小幅度
    float _rangeMin;
    //人脸姿态类型
    int _facePoseType;

};

}




