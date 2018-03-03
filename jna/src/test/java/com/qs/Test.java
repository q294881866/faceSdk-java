package com.qs;

import static org.junit.Assert.*;

import java.awt.image.BufferedImage;

import com.qs.QsFaceEngine;
import com.qs.Util;
import com.qs.QsFaceEngine.QsFace;
import com.qs.QsFaceEngine.QsFace.ByReference;


public class Test {


    /**
     * 图片比较
     */
    @org.junit.Test
    public void imgCompareTest() throws Exception {
        long handler = QsFaceEngine.create();
        System.out.println(handler);
        long begin = System.currentTimeMillis();
        float score = QsFaceEngine.compare2Image(handler, "1.jpg", "2.jpg");
        System.err.println(System.currentTimeMillis() - begin);
        System.out.println("图片比较:" + score);
    }

    /**
     * 标准比较API，基于字节流
     * <ol>
     * <li>初始化算法model</li>
     * <li>从摄像设备读图图片字节流数据</li>
     * <li>检测数据流人脸个数</li>
     * <li>提取（循环）人脸特征</li>
     * <li>特征比较返回0-1相似度</li>
     * </ol>
     */
    @org.junit.Test
    public void faceEngineTest() throws Exception {

        long begin = System.currentTimeMillis();
        // 1.创建算法引擎，全局只需要做一次，比较耗时
        long handler = QsFaceEngine.create();
        System.out.println(handler);

        // 2.读取picture字节流image
        BufferedImage img1 = Util.toBufferedImage("1.jpg");
        BufferedImage img2 = Util.toBufferedImage("2.jpg");

        // 3.人脸检测
        QsFace[] face1 = new QsFace[1];
        QsFace[] face2 = new QsFace[1];
        int facenum1 =
                QsFaceEngine.detectFaces(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3, face1, 1);
        System.out.println("facenum1:" + facenum1);
        int facenum2 =
                QsFaceEngine.detectFaces(handler, Util.toBytes(img2), img2.getWidth(), img2.getHeight(), img2.getWidth() * 3, face2, 1);
        System.out.println("facenum2:" + facenum2);

        // 转为C/C++指针类型格式
        ByReference f1 = face1[0].asReference();
        ByReference f2 = face2[0].asReference();

        // 4.提取特征并比较：这里比较两张照片，并且都只有一张脸，多个做 for 循环
        QsFaceEngine.extractFeature(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3, f1);
        QsFaceEngine.extractFeature(handler, Util.toBytes(img2), img2.getWidth(), img2.getHeight(), img2.getWidth() * 3, f2);
        float score = QsFaceEngine.compare2Feature(handler, f1.feature, f2.feature);
        System.out.println("特征比较:" + score);
        System.err.println(System.currentTimeMillis() - begin);

    }

    /**
     * 简化标准比较API，基于字节流
     * 
     * @see faceEngineTest
     */
    @org.junit.Test
    public void apiTest() throws Exception {

        long begin = System.currentTimeMillis();
        // 1.创建算法引擎，全局只需要做一次，比较耗时
        long handler = QsFaceEngine.create();
        System.out.println(handler);

        // 2.读取picture字节流image
        BufferedImage img1 = Util.toBufferedImage("1.jpg");
        BufferedImage img2 = Util.toBufferedImage("2.jpg");

        // 3.人脸检测
        QsFace.ByReference[] face1 = QsFaceEngine.detectFaceAndFeature(//
                handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3, 1);
        QsFace.ByReference[] face2 = QsFaceEngine.detectFaceAndFeature(//
                handler, Util.toBytes(img2), img2.getWidth(), img2.getHeight(), img2.getWidth() * 3, 1);


        // 4.特征并比较
        for (int i = 0; i < face1.length; i++) {
            ByReference f1 = face1[i];
            for (int j = 0; j < face2.length; j++) {
                ByReference f2 = face2[j];
                float score = QsFaceEngine.compare2Feature(handler, f1.feature, f2.feature);
                System.err.println(System.currentTimeMillis() - begin);
                System.out.println("特征比较:" + score);
            }
        }

    }

    @org.junit.Test
    public void glassesTest() throws Exception {
        long prem = Runtime.getRuntime().freeMemory();
        long handler = QsFaceEngine.create();
        System.out.println(handler);

        BufferedImage img1 = Util.toBufferedImage("2.jpg");

        QsFace[] faces = new QsFace[1];
        int faceNum =
                QsFaceEngine.detectFaces(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3, faces, 1);
        // 判断是否戴眼镜
        if (faceNum > 0) {
            boolean res1 = QsFaceEngine.detectGlasses(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3,
                    faces[0].rect);
            System.out.println(res1);
        }

        long lastm = Runtime.getRuntime().freeMemory();
        System.out.println("memory:" + (lastm - prem));
    }

    @org.junit.Test
    public void otherAPITest() throws Exception {
        long handler = QsFaceEngine.create();
        BufferedImage img1 = Util.toBufferedImage("3.jpg");

        // 人脸检测
        QsFace[] face1 = new QsFace[1];
        int facenum =
                QsFaceEngine.detectFaces(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3, face1, 1);
        System.out.println("facenum:" + facenum);
    }


}
