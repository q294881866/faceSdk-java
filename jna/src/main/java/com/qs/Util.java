package com.qs;

import javax.imageio.ImageIO;
import javax.swing.*;

import org.opencv.core.CvType;
import org.opencv.core.Mat;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.File;
import java.io.IOException;


/**
 * Created by mazhi on 2017/6/9. Modified by Gavin 2017/09/29
 */
public class Util {

    /**
     * 转换为BGR24字节流
     */
    public static byte[] toBytes(BufferedImage bufferedImage) {
        if (bufferedImage != null) {
            return ((DataBufferByte) bufferedImage.getRaster().getDataBuffer()).getData();
        }
        return null;
    }


    /**
     * 支持jpg、png图片
     * <ol>
     * <li>jpg 默认，图片24位深度
     * <li>png 图片内部会转为jpg图片
     * </ol>>
     * @param imageFilePath
     * @return 当异常发生时返回空
     * @throws IOException
     */
    public static BufferedImage toBufferedImage(String imageFilePath) throws IOException {
        File img = new File(imageFilePath);
        if(imageFilePath.endsWith(".jpg")){
            Image image = ImageIO.read(img);
            return toBufferedImage(image);
        }else {
            return pngToJpg(img);
        }
    }


    /**
     * TYPE_INT_RGB:创建一个RBG图像，24位深度，成功将32位图转化成24位
     * @param file 
     * 
     * @return
     * @throws IOException
     */
    public static BufferedImage pngToJpg(File file) throws IOException {
        BufferedImage bufferedImage = ImageIO.read(file);

        // create a blank, RGB, same width and height, and a white background
        BufferedImage newBufferedImage = new BufferedImage(bufferedImage.getWidth(),
            bufferedImage.getHeight(), BufferedImage.TYPE_3BYTE_BGR);
        newBufferedImage.createGraphics().drawImage(bufferedImage, 0, 0, Color.WHITE, null);

        return newBufferedImage;
    }


    public static BufferedImage toBufferedImage(Mat in) {
        byte[] data = new byte[in.width() * in.height() * (int) in.elemSize()];
        in.get(0, 0, data);

        int type = (in.channels() == 1) ? BufferedImage.TYPE_BYTE_GRAY : BufferedImage.TYPE_3BYTE_BGR;
        BufferedImage out = new BufferedImage(in.width(), in.height(), type);

        out.getRaster().setDataElements(0, 0, in.width(), in.height(), data);
        return out;
    }


    public static Mat toMat(BufferedImage bi) {
        Mat mat = new Mat(bi.getHeight(), bi.getWidth(), CvType.CV_8UC3);
        byte[] data = ((DataBufferByte) bi.getRaster().getDataBuffer()).getData();
        mat.put(0, 0, data);
        return mat;
    }


    public static Mat toMat(byte[] imgBuff, int width, int height) {
        Mat mat = new Mat(height, width, CvType.CV_8UC3);
        mat.put(0, 0, imgBuff);
        return mat;
    }


    public static BufferedImage toBufferedImage(Image image) {
        if (image instanceof BufferedImage) {
            return (BufferedImage) image;
        }

        // This code ensures that all the pixels in the image are loaded
        image = new ImageIcon(image).getImage();

        // Determine if the image has transparent pixels; for this method's
        // implementation, see e661 Determining If an Image Has Transparent
        // Pixels
        // boolean hasAlpha = hasAlpha(image);

        // Create a buffered image with a format that's compatible with the
        // screen
        BufferedImage bimage = null;
        GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
        try {
            // Determine the type of transparency of the new buffered image
            int transparency = Transparency.OPAQUE;
            /*
             * if (hasAlpha) { transparency = Transparency.BITMASK; }
             */

            // Create the buffered image
            GraphicsDevice gs = ge.getDefaultScreenDevice();
            GraphicsConfiguration gc = gs.getDefaultConfiguration();
            bimage = gc.createCompatibleImage(image.getWidth(null), image.getHeight(null), transparency);
        }
        catch (HeadlessException e) {
            // The system does not have a screen
        }

        if (bimage == null) {
            // Create a buffered image using the default color model
            int type = BufferedImage.TYPE_3BYTE_BGR;
            // int type = BufferedImage.TYPE_3BYTE_BGR;//by wang
            /*
             * if (hasAlpha) { type = BufferedImage.TYPE_INT_ARGB; }
             */
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
