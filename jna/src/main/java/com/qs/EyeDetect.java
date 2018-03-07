package com.qs;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.objdetect.CascadeClassifier;

public class EyeDetect {
    private static final CascadeClassifier EYE_DETECTOR;
    static {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        EYE_DETECTOR = new CascadeClassifier("haarcascades/haarcascade_eye.xml");
    }

    /**
     * 判断是否带眼镜，非人脸照片也会显示带了眼镜
     * @param face 传入人脸照片
     * @return
     */
    public static boolean isGlasses(Mat face) {

        MatOfRect rect = new MatOfRect();
        Mat dest = face.clone();
        EYE_DETECTOR.detectMultiScale(dest, rect);

        System.out.println(rect.toArray().length);
        return rect.toArray().length < 2;
    }
    
    

    public static void main(String[] args) {
        try {
            Mat src = Imgcodecs.imread("2.jpg");
            if (src.empty()) {
                throw new Exception("no file");
            }
            System.out.println(isGlasses(src));
        } catch (Exception e) {
            System.out.println("例外：" + e);
        }
    }
}
