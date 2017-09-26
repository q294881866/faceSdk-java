package QS.Test;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.*;

import com.qs.face.QsFace;
import com.qs.face.entity.FaceFeaturePoint;
import com.qs.face.entity.FaceModel;
import com.qs.face.entity.FacePointsFeatureResult;

import javax.imageio.ImageIO;
import javax.swing.*;

public class Test {

    public static void demo()
    {
        QsFace qsFace = new QsFace();
        qsFace.init();
        int maxFaceCount=1;
        try {
            qsFace.detectFacesFeature("1.jpg",maxFaceCount);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void demo2()
    {

    }

    /**
     * @param args
     */
    public static void main(String[] args) {
        System.out.print(System.getProperty("java.library.path"));
        QsFace qsFace = new QsFace();
        if (args.length==0){
            System.out.print("输入参数错误");
            return;
        }
        //初始化
        qsFace.init();
        String imageFilePath=args[0];
        Image image=null;
        try {
            File pathToFile = new File(imageFilePath);
            image= ImageIO.read(pathToFile);
        } catch (IOException ex) {
            ex.printStackTrace();
            return;
        }
        //===================================================2张图片比对相似度============================================
        float sim=qsFace.compare2Image(imageFilePath,imageFilePath);
        //===================================================2张图片比对相似度============================================
        BufferedImage bufferedImage=toBufferedImage(image);
        if (bufferedImage==null)
            return;
        byte[] bgr24=((DataBufferByte) bufferedImage.getRaster().getDataBuffer()).getData();
                int widht = bufferedImage.getWidth();
        int height = bufferedImage.getHeight();

        int widthStept = widht*3;

        //===================================================特征值检测，检测出的特征值用于相似度比对============================================
        FaceModel[] models = qsFace.detectFacesFeature(bgr24, widht, height, widthStept, 10);
//        FaceModel[] models2 = qsFace.detectFacesFeature(bgr242, widht2, height2, widthStept2, 10);
//        sim=qsFace.face2Face(models[0].ptFeature,models2[0].ptFeature);
        //===================================================特征值检测，检测出的特征值用于相似度比对=============================================


        //===================================================防伪，活体检测====================================
        //防伪检测是通过判断一定时间内的连续输入照片来判断是否是活体,
        //在这一定时间内：1表示检测成功 、0表示需要持续检测（控制检测超时要在外层应用进行判断）、 -1表示检测失败
        int antiFakeResult=qsFace.antiFake(bgr24,widht,height,widthStept,1);
        System.out.print("antifake=");
        System.out.print(antiFakeResult);
     //===================================================防伪检测====================================

        if (models != null) {

           for (int i=0;i<models.length;i++){
               //===================================================姿态检测====================================
               FacePointsFeatureResult pointResult = qsFace.calFacePoints(bgr24, widht, height, widthStept, models[0]);
               if (pointResult != null) {

                   System.out.print(pointResult.scole);
                   System.out.print("\n");
                   for (FaceFeaturePoint item : pointResult.facePoints.points) {
//                       System.out.print(item.x);
//                       System.out.print(";");
//                       System.out.print(item.y);
//                       System.out.print("\n");
                   }
               }
               //===================================================姿态检测===================================
           }

        } else {
            System.out.print("can not detected face\n");
        }
        //===================================================疲劳检测============================================
        //疲劳检测,s
        com.face.base.entity.FatigueDetectConfig config=new com.face.base.entity.FatigueDetectConfig();
        //设置疲劳检测灵敏度，数值越低越灵敏，
        config.fatigueDetectCount=10;
        qsFace.setFatigueConfig(config);
        com.face.base.entity.FatigueDeterminationResult fatiresult=qsFace.fatigueDetect(bgr24,widht,height,widthStept);
        if (fatiresult!=null){
            System.out.print("fatiresult...........=\n");
            System.out.print(fatiresult.fatigueDeterminationResult);
            if (fatiresult.rtFace!=null){
                System.out.print(fatiresult.rtFace.bottom);
            }else {
                System.out.print("没有人脸矩形\n");
            }

        }
        //===================================================疲劳检测============================================
    }


    public static BufferedImage toBufferedImage(Image image) {
        if (image instanceof BufferedImage) {
            return (BufferedImage) image;
        }

        // This code ensures that all the pixels in the image are loaded
        image = new ImageIcon(image).getImage();

        // Determine if the image has transparent pixels; for this method's
        // implementation, see e661 Determining If an Image Has Transparent Pixels
        //boolean hasAlpha = hasAlpha(image);

        // Create a buffered image with a format that's compatible with the screen
        BufferedImage bimage = null;
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        try {
            // Determine the type of transparency of the new buffered image
            int transparency = Transparency.OPAQUE;
       /* if (hasAlpha) {
         transparency = Transparency.BITMASK;
         }*/

            // Create the buffered image
            GraphicsDevice gs = ge.getDefaultScreenDevice();
            GraphicsConfiguration gc = gs.getDefaultConfiguration();
            bimage = gc.createCompatibleImage(
                    image.getWidth(null), image.getHeight(null), transparency);
        } catch (HeadlessException e) {
            // The system does not have a screen
        }

        if (bimage == null) {
            // Create a buffered image using the default color model
            int type = BufferedImage.TYPE_INT_RGB;
            //int type = BufferedImage.TYPE_3BYTE_BGR;//by wang
        /*if (hasAlpha) {
         type = BufferedImage.TYPE_INT_ARGB;
         }*/
            bimage = new BufferedImage(image.getWidth(null), image.getHeight(null), type);
        }

        // Copy image to buffered image
        Graphics g = bimage.createGraphics();

        // Paint the image onto the buffered image
        g.drawImage(image, 0, 0, null);
        g.dispose();

        return bimage;
    }


}
