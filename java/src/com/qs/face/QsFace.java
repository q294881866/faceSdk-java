package com.qs.face;

import com.qs.face.entity.FaceModel;
import com.qs.face.entity.FacePointsFeatureResult;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

/**
 * Created by mazhi on 2017/6/9.
 */
public class QsFace {

	static String JNI_LIB_PATH = "WisFaceEngineWrap";

	static {
		System.loadLibrary(JNI_LIB_PATH);// 后面使用C/C++编写的JAVA能直接调用的库
	}

    public QsFace() {
//        setFatigueDetectCountAPI(fatigueDetectConfig.fatigueDetectCount,
//                fatigueDetectConfig.eyeThresold,fatigueDetectConfig.mouthThresold);
    }

//    public QsFace(com.face.base.entity.FatigueDetectConfig fatigueDetectConfig)
//    {
//        this.fatigueDetectConfig=fatigueDetectConfig;
//
//    }

//    private com.face.base.entity.FatigueDetectConfig fatigueDetectConfig=new
//            com.face.base.entity.FatigueDetectConfig();

    private native int createEngine();

	private native void freeEngine(int faceEngine);

	private native FaceModel[] process(int faceEngine, byte[] imgBgr24, int width, int height, int widthStep,
			int maxFaceCount);

	private native float compare2Feature(int faceEngine, byte[] feature1, byte[] feature2);

	private native float compare2Image(int faceEngine, String image1, String image2);

    private static native FacePointsFeatureResult calculateFacePoints(int faceEngine, byte[] imgBgr24, int width,
			int height, int widthStep, int x, int y, int rectWidth, int rectHeight);

    private static native com.face.base.entity.FatigueDeterminationResult fatigueDetectAPI(int faceEngine,
            byte[] imgBgr24,int width,int height,int widthStep);

    private static native void setFatigueDetectCountAPI(int fatigueDetectCountArgs,
                                                       int eyeThresoldArgs,int mouthThresoldArgs);


    /// <summary>
    /// 人脸防伪检测
    /// </summary>
    /// <param name="imgBgr24">imgBgr24</param>
    /// <param name="width">图片的width</param>
    /// <param name="height">图片的height</param>
    /// <param name="widthStep">图片的widthStep</param>
    /// <param name="FacePoseType">动作类型int（1~4）,1:眨眼 2：吐舌头 3：左右转脸 4：上下点头</param>
    /// <returns>返回检测状态：1表示检测成功 、0表示需要持续检测（控制检测超时要在外层应用进行判断）、 -1表示检测失败</returns>
	public static native int antiFakeDetect(int faceEngine,byte[] imgBgr24, int width,
                                            int height, int widthStep,int facePoseType);

	protected int faceEngine;

    public static final int antiErrorResult=-1;

	/**
	 * 初始化引擎，进行人脸识别首先要初始化人脸检测引擎
	 * 
	 * @return true success, other error
	 */
	public boolean init() {
		faceEngine = createEngine();
		if (faceEngine>0)
			return true;
		return false;
	}

	public float compare2Image(String image1, String image2) {
		return this.compare2Image(this.faceEngine, image1, image2);
	}

	/*
	 * 获取图片的特征值 image 图片缓冲区 imageSize 图片缓冲区大小 返回人脸特征值数组
	 */
	public FaceModel[] detectFacesFeature(byte[] imgBgr24, int width, int height, int widthStep, int maxFaceCount) {
		FaceModel[] models = process(faceEngine, imgBgr24, width, height, widthStep, maxFaceCount);
		return models;
	}

	public FaceModel[] detectFacesFeature(Image image, int maxFaceCount) {
		if (image != null) {
			BufferedImage bufferedImage = null;
			bufferedImage = Util.toBufferedImage(image);
			if (bufferedImage != null) {
				return detectFacesFeature(bufferedImage, maxFaceCount);
			}
		}
		return null;

	}

	public FaceModel[] detectFacesFeature(BufferedImage bufferedImage, int maxFaceCount) {
		if (bufferedImage != null) {
			return detectFacesFeature(Util.getBgrDataFromBufferedImage(bufferedImage), bufferedImage.getWidth(),
					bufferedImage.getHeight(), bufferedImage.getWidth() * 3, maxFaceCount);
		}
		return null;
	}

	public FaceModel[] detectFacesFeature(String imagePath, int maxFaceCount) throws IOException {
		if (imagePath != null) {
			return detectFacesFeature(Util.toBufferedImage(imagePath), maxFaceCount);
		}
		return null;
	}

    public int antiFake(byte[] imgBgr24, int width, int height, int widthStep, int facePoseType)
    {
        return antiFakeDetect(faceEngine, imgBgr24, width, height, widthStep, facePoseType);
    }

    public int antiFake(Image image,int facePostType)
    {
        if (image!=null)
        {
            return antiFake(Util.toBufferedImage(image),facePostType);
        }
        return antiErrorResult;
    }

    public int antiFake(BufferedImage bufferedImage,int facePoseType)
    {
        if (bufferedImage!=null)
            return antiFake(Util.getBgrDataFromBufferedImage(bufferedImage),bufferedImage.getWidth(),
                    bufferedImage.getHeight(),bufferedImage.getWidth()*3,facePoseType);
        return antiErrorResult;
    }

    public com.face.base.entity.FatigueDeterminationResult fatigueDetect(
           byte[] imgBgr24,int width,int height,int widthStep)
    {
        if (imgBgr24!=null){
            return fatigueDetectAPI(faceEngine, imgBgr24, width, height, widthStep);
        }
        return null;
    }

    public void setFatigueConfig(com.face.base.entity.FatigueDetectConfig config){
        if (config!=null){
            setFatigueDetectCountAPI(config.fatigueDetectCount,
                    config.eyeThresold,config.mouthThresold);
        }
    }

	/*
	 * 获取人脸相似度 feature1 第一张人脸的人脸特征码 feature2 第二张人脸的人脸特征码
	 */
	public float face2Face(byte[] feature1, byte[] feature2) {
		float degree = compare2Feature(faceEngine, feature1, feature2);
		return degree;
	}

	/*
	 * 释放人脸引擎
	 */
	public void freeEngine() {
		freeEngine(faceEngine);
	}

	public FacePointsFeatureResult calFacePoints(byte[] imgBgr24, int width, int height, int widthStep,
			FaceModel faceModel) {
		if (faceModel != null && faceModel.rtFace != null) {
			int x = faceModel.rtFace.left;
			int y = faceModel.rtFace.top;
			int rectWidth = Math.abs(faceModel.rtFace.left - faceModel.rtFace.right);
			int rectHeight = Math.abs(faceModel.rtFace.top - faceModel.rtFace.bottom);
			return calculateFacePoints(faceEngine, imgBgr24, width, height, widthStep, x, y, rectWidth, rectHeight);
		}
		return null;

	}
}
