package com.qs.test;

import java.awt.image.BufferedImage;

import com.qs.face.QsFace;
import com.qs.face.Util;
import com.qs.face.entity.FaceModel;

/**
 * jni api demo
 * @author Bevis-Pei<ppf@jiumao.org> 
 * @date 2018/03/03
 */
public class Test {


    @org.junit.Test
    public void imgCompareTest() throws Exception {
        // 初始化算法引擎
        long begin = System.currentTimeMillis();
        QsFace qsFace = new QsFace();
        qsFace.init();

        String image1 = "1.jpg";
        String image2 = "2.jpg";
        float score = qsFace.compare2Image(image1, image2);
        System.err.println(System.currentTimeMillis() - begin);
        System.out.println("相似度： " + score);
    }

    /**
     * 标准API，基于数据流
     */
    @org.junit.Test
    public void bufCompareTest() throws Exception {
        // 初始化算法引擎
        long begin = System.currentTimeMillis();
        QsFace qsFace = new QsFace();
        qsFace.init();

        // 人臉的特征提取
        // FaceModel[] faces1 = new FaceModel[10];//10代表一次最多获取10个人的人脸特征，例如当一个照片中有多个人脸时，可以使用这个参数
        BufferedImage img1 = Util.toBufferedImage("1.jpg");
        FaceModel[] faces1 = qsFace.detectFacesFeature(Util.toBytes(img1), img1.getWidth(), img1.getHeight(), img1.getWidth() * 3, 10);
        BufferedImage img2 = Util.toBufferedImage("2.jpg");

        FaceModel[] faces2 = qsFace.detectFacesFeature(Util.toBytes(img2), img2.getWidth(), img2.getHeight(), img2.getWidth() * 3, 10);

        // 特征值用于相似度比对
        if (faces1 != null && faces2 != null) {
            float score = qsFace.face2Face(faces1[0].ptFeature, faces2[0].ptFeature);
            System.out.println("相似度： " + score);
        }
        System.err.println(System.currentTimeMillis() - begin);
        // 释放算法model
        qsFace.freeEngine();
    }



}
