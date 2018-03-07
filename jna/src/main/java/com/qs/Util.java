package com.qs;

import javax.imageio.ImageIO;
import javax.swing.*;

import org.opencv.core.Mat;
import org.opencv.core.MatOfByte;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.File;
import java.io.IOException;

/**
 * Created by mazhi on 2017/6/9. Modified by Gavin 2017/09/29
 */
public class Util {

    public static byte[] toBytes(BufferedImage bufferedImage) {
        if (bufferedImage != null) {
            return ((DataBufferByte) bufferedImage.getRaster().getDataBuffer()).getData();
        }
        return null;
    }

    public static BufferedImage toBufferedImage(String imageFilePath) throws IOException {
        File pathToFile = new File(imageFilePath);
        Image image = ImageIO.read(pathToFile);
        if (image != null) {
            return toBufferedImage(image);
        }
        return null;

    }

    public static Mat toMat(byte[] bytes) {
        return new MatOfByte(bytes).clone();
    }

    public static BufferedImage toBufferedImage(Image image) {
        if (image instanceof BufferedImage) {
            return (BufferedImage) image;
        }

        // This code ensures that all the pixels in the image are loaded
        image = new ImageIcon(image).getImage();

        // Determine if the image has transparent pixels; for this method's
        // implementation, see e661 Determining If an Image Has Transparent Pixels
        // boolean hasAlpha = hasAlpha(image);

        // Create a buffered image with a format that's compatible with the screen
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
        } catch (HeadlessException e) {
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
