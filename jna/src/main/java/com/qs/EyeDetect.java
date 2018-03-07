package com.qs;

import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.MatOfRect;
import org.opencv.core.Rect;
import org.opencv.imgcodecs.Imgcodecs;
import org.opencv.objdetect.CascadeClassifier;

public class EyeDetect {
    private static final CascadeClassifier EYE_DETECTOR;
    private static final CascadeClassifier FACE_DETECTOR;
    static {
        System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
        EYE_DETECTOR = new CascadeClassifier("haarcascades/haarcascade_eye.xml");
        FACE_DETECTOR = new CascadeClassifier("haarcascades/haarcascade_frontalface_alt2.xml");
    }

    /**
     * 判断是否带眼镜，非人脸照片也会显示带了眼镜
     * 
     * @param face 传入人脸照片
     */
    public static boolean[] isGlasses(Mat image) {
        Mat dest = image.clone();
        MatOfRect faceRect = new MatOfRect();
        FACE_DETECTOR.detectMultiScale(dest, faceRect);

        Rect[] faces = faceRect.toArray();
        boolean[] res = new boolean[faces.length];
        for (int i = 0; i < faces.length; i++) {
            MatOfRect eyeRect = new MatOfRect(faces[i]);
            EYE_DETECTOR.detectMultiScale(dest, eyeRect);

            System.out.println(eyeRect.toArray().length);
            res[i] = eyeRect.toArray().length < 2;
        }

        return res;
    }



    public static void main(String[] args) throws Exception {
        Mat src = Imgcodecs.imread("2.jpg");
        if (src.empty()) {
            throw new Exception("no file");
        }
        System.out.println(isGlasses(src));
    }
}
