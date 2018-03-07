package com.qs;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Structure;


/**
 * 图片对比
 * 
 * @author ppf
 * @date 2017年11月15日
 */
public class QsFaceEngine {

    private static final QsFaceLibrary INSTANCE =
            (QsFaceLibrary) Native.loadLibrary("WisFaceEngineWrapV4", QsFaceLibrary.class);


    /**
     * 创建人脸识别引擎
     * 
     * @return 返回人脸引擎句柄engine if >0 sucess, or falied
     */
    public static long create() {
        return INSTANCE.qs_Wis_Create(0);
    }


    /**
     * 人脸检测
     * 
     * @param engine qs_Wis_Create()返回的人脸引擎句柄
     * @param image 照片imgBgr24
     * @param width 图像宽度
     * @param height 图像高度
     * @param widthstep widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）
     * @param faces 返回的人脸位置,size = 数量maxCount
     * @param maxCount 最大检测人脸个数
     * @return 检测到的人脸个数
     */
    public static int detectFaces(long handler, byte[] imgBuff, int width, int height, int widthstep,
            QsFace[] faces, int maxCount) {
        return INSTANCE.qs_Wis_DetectFaces(handler, imgBuff, width, height, widthstep, faces, maxCount);
    }


    /**
     * 人脸检测
     * 
     * @param engine {@link #qs_Wis_Create(int)}返回的人脸引擎句柄
     * @param image 照片imgBgr24
     * @param width 图像宽度
     * @param height 图像高度
     * @param widthstep widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）
     * @param faceRects 返回的人脸位置,size = 数量maxCount
     * @param maxCount 最大检测人脸个数
     * @return 检测到的人脸个数
     */
    public static int detectFacesV2(long handler, byte[] imgBuff, int width, int height, int widthstep,
            QsFace[] faceRects, int maxCount) {
        return INSTANCE.qs_Wis_DetectFacesV2(handler, imgBuff, width, height, widthstep, faceRects, maxCount);
    }


    /**
     * 人脸检测,加强版，速度比{@link #qs_Wis_DetectFaces(long, byte[], int, int, int, QsFace[], int)}慢
     * 
     * @param handler qs_Wis_Create()返回的人脸引擎句柄
     * @param imgBuff 照片imgBgr24
     * @param width 图像宽度
     * @param height 图像高度
     * @param widthstep widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）
     * @param faces 返回的人脸位置,size = 数量maxCount
     * @param maxCount 最大检测人脸个数
     * @return 检测到的人脸个数
     */
    public static int detectFacesReinforce(long handler, byte[] imgBuff, int width, int height, int widthstep,
            QsFace[] faces, int maxCount) {
        return INSTANCE.qs_Wis_DetectFaces_Reinforce(handler, imgBuff, width, height, widthstep, faces,
            maxCount);
    }


    /**
     * 
     * 特征提取
     * 
     * @param handler qs_Wis_Create()返回的人脸引擎句柄
     * @param imgBuff 照片imgBgr24
     * @param width 图像宽度
     * @param height 图像高度
     * @param widthstep widthstep是存储一行图像所占的字节（相邻两行起点指针的差值）
     * @param face
     *            人脸结构体，调用特征提取之前就已经有了rect即人脸框，执行特征提取成功后，该人脸的特征值会自动存储于face.feature中
     * @return 0 success, 其他失败
     */
    public static int extractFeature(long handler, byte[] imgBuff, int width, int height, int widthstep,
            QsFace.ByReference face) {
        return INSTANCE.qs_Wis_ExtractFeature(handler, imgBuff, width, height, widthstep, face);
    }


    /**
     * 特征比对
     * 
     * @param engine qs_Wis_Create()返回的人脸引擎句柄
     * @param ptFeature1 第一个人脸特征
     * @param ptFeature2 第一个人脸特征
     * @return 相似度
     */
    public static float compare2Feature(long handler, byte[] feature1, byte[] feature2) {
        return INSTANCE.qs_Wis_Compare2Feature(handler, feature1, feature2);
    }


    /**
     * 人脸照片比对，传入两个照片，返回两个人脸的相似度score. if score > 0.8 说明两个照片相似度很高，
     * 
     * @param handler
     * @param engine qs_Wis_Create() 返回的人脸引擎句柄
     * @param imgFile1 第一个照片路径（相对or绝对均可）
     * @param imgFile2 第二个照片路径
     * @return 相似度
     */
    public static float compare2Image(long handler, String image1, String image2) {
        return INSTANCE.qs_Wis_Compare2Image(handler, image1, image2);
    }

    public static final int POINTS_NUM = 51;

    public static class FacePointsModel extends DefaultStruct {
        public float roll;
        public float yaw;
        public float pitch;
        public Point[] points = new Point[POINTS_NUM];

        public static class ByValue extends FacePointsModel implements Structure.ByValue {
        }

        public static class ByReference extends FacePointsModel implements Structure.ByReference {
        }


        @Override
        public String toString() {
            return "roll :" + roll + " | yaw:" + yaw + " | pitch:" + pitch + points;
        }
    }

    public static class Point extends DefaultStruct {
        public int x, y;

        public static class ByValue extends Point implements Structure.ByValue {
        }


        @Override
        public String toString() {
            return "x:" + x + " | y" + y;
        }
    }

    public static class QsRect extends DefaultStruct {
        public int left; // 矩形框左上角x坐标
        public int top; // 矩形框左上角y坐标
        public int right; // 矩形框右下角x坐标
        public int bottom; // 矩形框右下角y坐标

        public static class ByValue extends QsRect implements Structure.ByValue {
        }


        public QsRect.ByValue asValue() {
            ByValue val = new ByValue();
            val.left = this.left;
            val.top = this.top;
            val.right = this.right;
            val.bottom = this.bottom;
            return val;
        }
    }

    /**
     * 人脸特征对象
     */
    public static class FaceModel extends DefaultStruct {
        /** 人脸特征矩形框 */
        public QsRect rtFace;
        /** 左眼位置,右眼位置, 嘴巴 ,左嘴位置 ,右嘴位置 */
        public Point ptLeftEye, ptRightEye, ptNose, ptMouthLeft, ptMouthRight;
        /** 人脸特征码 */
        public byte[] ptFeature;
    }

    public static final int FEATURE_SIZE = 512;

    public static class QsFace extends DefaultStruct {
        public QsRect rect;
        public float confidence; // 置信度
        public float angle_roll; // 横滚，将物体绕Z轴旋转（localRotationZ）
        public float angle_yaw; // 航向，将物体绕Y轴旋转（localRotationY）
        public float angle_pitch; // 俯仰，将物体绕X轴旋转（localRotationX）
        public float age; // 年龄
        public int gender; // 性别
        public byte[] feature = new byte[FEATURE_SIZE]; // 人脸特征 size

        public static class ByReference extends QsFace implements Structure.ByReference {
        }
        
        public QsFace.ByReference asReference() {
            ByReference ref = new ByReference();
            ref.rect = this.rect;
            ref.confidence = this.confidence;
            ref.angle_roll = this.angle_roll;
            ref.angle_yaw = this.angle_yaw;
            ref.angle_pitch = this.angle_pitch;
            ref.age = this.age;
            ref.gender = this.gender;
            ref.feature = this.feature;
            return ref;
        }

    }

    public interface QsFaceLibrary extends Library {

        long qs_Wis_Create(int tag);


        int qs_Wis_DetectFaces(long handler, byte[] imgBuff, int width, int height, int widthstep,
                QsFace[] faces, int maxCount);


        int qs_Wis_DetectFacesV2(long handler, byte[] imgBuff, int width, int height, int widthstep,
                QsFace[] faceRects, int maxCount);


        int qs_Wis_DetectFaces_Reinforce(long handler, byte[] imgBuff, int width, int height, int widthstep,
                QsFace[] faces, int maxCount);


        int qs_Wis_ExtractFeature(long handler, byte[] imgBuff, int width, int height, int widthstep,
                QsFace.ByReference face);


        float qs_Wis_Compare2Feature(long handler, byte[] ptFeature1, byte[] ptFeature2);


        float qs_Wis_Compare2Image(long handler, String imgFile1, String imgFile2);
    }

}
