package com.qs.face.entity;

import com.qs.face.entity.FaceFeaturePoint;
import com.qs.face.entity.FaceFeatureRectangle;

/*
 * 人脸特征对象
 */
public class FaceModel {
	  /// <summary>
    /// 人脸特征矩形框
    /// </summary>
    public FaceFeatureRectangle rtFace;
    /// <summary>
    /// 左眼位置
    /// </summary>
    public FaceFeaturePoint ptLeftEye;
    /// <summary>
    /// 友眼位置
    /// </summary>
    public FaceFeaturePoint ptRightEye;
    /// <summary>
    /// 嘴巴
    /// </summary>
    public FaceFeaturePoint ptNose;
    /// <summary>
    /// 左嘴位置
    /// </summary>
    public FaceFeaturePoint ptMouthLeft;
    /// <summary>
    /// 友嘴位置
    /// </summary>
    public FaceFeaturePoint ptMouthRight;
    /// <summary>
    /// 人脸特征码
    /// </summary>
    public byte[] ptFeature;
}
