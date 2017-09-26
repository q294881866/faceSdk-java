package com.qs.face.entity;

import com.qs.face.entity.FaceFeaturePoint;

/**
 * Created by mazhi on 2017/6/3.
 */
public class FacePointModel {

    /// <summary>
    /// 翻滚角
    /// </summary>
    public float roll;

    /// <summary>
    /// 偏转角
    /// </summary>
    public float yaw;

    /// <summary>
    /// 俯仰角
    /// </summary>
    public float pitch;

    /// <summary>
    /// 人脸特征点
    /// </summary>
    public FaceFeaturePoint[] points;

}
