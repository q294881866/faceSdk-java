
import static org.junit.Assert.*;

import java.awt.image.BufferedImage;

import org.opencv.core.Mat;
import org.opencv.imgcodecs.Imgcodecs;

import com.qs.EyeDetect;
import com.qs.QsFaceEngine;
import com.qs.Util;
import com.qs.QsFaceEngine.QsFace;
import com.qs.QsFaceEngine.QsFace.ByReference;


public class Test {
    
    @org.junit.Test
    public void faceEngineTest() throws Exception {

        long handler = QsFaceEngine.create();
        float score;
        int facenum;
        System.out.println(handler);
        score = QsFaceEngine.compare2Image(handler, "1.jpg", "2.jpg");
        System.out.println("图片比较:" + score);

        BufferedImage img1 = Util.toBufferedImage("1.jpg");
        BufferedImage img2 = Util.toBufferedImage("2.jpg");

        // 人脸检测
        QsFace[] face1 = new QsFace[1];
        QsFace[] face2 = new QsFace[1];
        facenum = QsFaceEngine.detectFaces(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(),
            img1.getWidth() * 3, face1, 1);
        System.out.println("facenum:" + facenum);
        facenum = QsFaceEngine.detectFacesV2(handler, Util.toBytes(img2), img2.getWidth(), img2.getHeight(),
            img2.getWidth() * 3, face2, 1);
        System.out.println("facenum:" + facenum);
        facenum = QsFaceEngine.detectFacesReinforce(handler, Util.toBytes(img2), img2.getWidth(),
            img2.getHeight(), img2.getWidth() * 3, face2, 1);
        System.out.println("facenum:" + facenum);

        ByReference f1 = face1[0].asReference();
        ByReference f2 = face2[0].asReference();

        // 这里比较两张照片，并且都只有一张脸，多个做 for 循环
        QsFaceEngine.extractFeature(handler, Util.toBytes(img1), img1.getWidth(), img1.getHeight(),
            img1.getWidth() * 3, f1);
        QsFaceEngine.extractFeature(handler, Util.toBytes(img2), img2.getWidth(), img2.getHeight(),
            img2.getWidth() * 3, f2);
        score = QsFaceEngine.compare2Feature(handler, f1.feature, f2.feature);
        System.out.println("特征比较:" + score);

    }
    

}
