/*
QiansouFaceSDKV4,http://qiansou.cn
Copyright (c) 2015-2017 Qiansou
support QQ : 2843028512
*/

#pragma once
#include <stdio.h>
#include "windows.h"

#define QsHANDLE long long
#define FEATURE_SIZE 512
#ifndef BYTE
#define BYTE unsigned char
#endif // !BYTE


typedef struct {
	int 	left;			// 矩形框左上角x坐标
	int 	top;			// 矩形框左上角y坐标
	int 	right;			// 矩形框右下角x坐标
	int 	bottom;			// 矩形框右下角y坐标
} QsRect;

typedef struct {
	QsRect  rect;
	float 	confidence;		// 置信度
	float 	angle_roll;		// 横滚，将物体绕Z轴旋转（localRotationZ）
	float 	angle_yaw;		// 航向，将物体绕Y轴旋转（localRotationY）
	float 	angle_pitch;	// 俯仰，将物体绕X轴旋转（localRotationX）
	float 	age;			// 年龄
	int   	gender;			// 性别
	BYTE    feature[FEATURE_SIZE]; 	//人脸特征 size = FEATURE_SIZE
} QsFace;


/// <summary>
/// 创建人脸识别引擎
/// </summary>
/// <param name="tag">tag = 0 </param>
/// <returns>返回人脸引擎句柄engine if >0 sucess, or falied</returns>
QsHANDLE qs_Wis_Create(int tag = 0);


/// <summary>
/// 人脸检测
/// </summary>
/// <param name="engine">qs_Wis_Create()返回的人脸引擎句柄</param>
/// <param name="image">照片imgBgr24</param>
/// <param name="width">图像宽度</param>
/// <param name="height">图像高度</param>
/// <param name="widthstep">widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）</param>
/// <param name="faces">返回的人脸位置,size = 数量maxCount</param>
/// <param name="maxCount">最大检测人脸个数</param>
/// <returns>检测到的人脸个数</returns>
int qs_Wis_DetectFaces(QsHANDLE handler, unsigned char* imgBgr24, int width, int height, int widthstep, QsFace* faces,const int maxCount);

/// <summary>
/// 人脸检测
/// </summary>
/// <param name="engine">qs_Wis_Create()返回的人脸引擎句柄</param>
/// <param name="image">照片imgBgr24</param>
/// <param name="width">图像宽度</param>
/// <param name="height">图像高度</param>
/// <param name="widthstep">widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）</param>
/// <param name="faceRects">返回的人脸位置,size = 数量maxCount</param>
/// <param name="maxCount">最大检测人脸个数</param>
/// <returns>检测到的人脸个数</returns>
int qs_Wis_DetectFacesV2(int handler, unsigned char* imgBgr24, int width, int height, int widthstep, QsFace* faceRects,const int maxCount);



/// <summary>
/// 人脸检测,加强版，速度比qs_Wis_DetectFaces慢
/// </summary>
/// <param name="engine">qs_Wis_Create()返回的人脸引擎句柄</param>
/// <param name="image">照片imgBgr24</param>
/// <param name="width">图像宽度</param>
/// <param name="height">图像高度</param>
/// <param name="widthstep">widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）</param>
/// <param name="faces">返回的人脸位置,size = 数量maxCount</param>
/// <param name="maxCount">最大检测人脸个数</param>
/// <returns>检测到的人脸个数</returns>
int qs_Wis_DetectFaces_Reinforce(QsHANDLE handler, unsigned char* imgBgr24, int width, int height, int widthstep, QsFace* faces,const int maxCount);


/// <summary>
/// 特征提取
/// </summary>
/// <param name="engine">qs_Wis_Create()返回的人脸引擎句柄</param>
/// <param name="imgBgr24">照片imgBgr24</param>
/// <param name="width">图像宽度</param>
/// <param name="height">图像高度</param>
/// <param name="widthstep">widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）</param>
/// <param name="face">人脸结构体，调用特征提取之前就已经有了rect即人脸框，执行特征提取成功后，该人脸的特征值会自动存储于face.feature中</param>
/// <returns>0 success, 其他失败</returns>
int qs_Wis_ExtractFeature(QsHANDLE handler, unsigned char* imgBgr24, int width, int height, int widthstep,  QsFace* face);

/// <summary>
/// 特征比对
/// </summary>
/// <param name="engine">qs_Wis_Create()返回的人脸引擎句柄</param>
/// <param name="ptFeature1">第一个人脸特征</param>
/// <param name="ptFeature2">第一个人脸特征</param>
/// <returns>相似度</returns>
float qs_Wis_Compare2Feature(QsHANDLE handler,BYTE* ptFeature1,BYTE* ptFeature2);

/// <summary>
/// 人脸照片比对，传入两个照片，返回两个人脸的相似度score. if score > 0.8 说明两个照片相似度很高，
/// </summary>
/// <param name="engine">qs_Wis_Create()返回的人脸引擎句柄</param>
/// <param name="imgFile1">第一个照片路径（相对or绝对均可）</param>
/// <param name="imgFile2">第二个照片路径</param>
/// <returns>相似度</returns>
float qs_Wis_Compare2Image(QsHANDLE handler, const char* imgFile1, const char* imgFile2);

