package QS.Test;

import java.awt.*;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferByte;
import java.io.*;

import com.qs.face.QsFace;
import com.qs.face.Util;
import com.qs.face.entity.FaceFeaturePoint;
import com.qs.face.entity.FaceModel;
import com.qs.face.entity.FacePointsFeatureResult;

import javax.imageio.ImageIO;
import javax.swing.*;

public class Test {

	
    public static void main(String[] args) {
    	
    	//要将sdk跟目录加入系统环境变量path
        //初始化
        QsFace qsFace = new QsFace();
        qsFace.init();
   
        //直接比較兩個照片的相似度
        
    	String image1 = "E:\\tmp\\faces\\sdk\\1.jpg";
    	String image2 = "E:\\tmp\\faces\\sdk\\2.jpg";
        float score = Compare2Image(qsFace,image1,image2);
        System.out.println("score = "+score);
        
        //人臉的特征提取
        //FaceModel[] faces1 = new FaceModel[10];//10代表一次最多获取10个人的人脸特征，例如当一个照片中有多个人脸时，可以使用这个参数
        FaceModel[] faces1 = DetectAndExtfeature(qsFace, image1);
        FaceModel[] faces2 = DetectAndExtfeature(qsFace, image2);
        
        //人脸特征比对
        if(faces1 != null && faces2 != null){
	        score = Compare2Feature(qsFace, faces1[0], faces2[0]);
	        System.out.println("score = "+score);
        }
        qsFace.freeEngine();
    }
    
    public static float Compare2Image(QsFace qsFace,String image1 ,String image2)  {
    
    	float score = qsFace.compare2Image(image1, image2);
    	//System.out.println(score);
    	return score;
    }
    
    public static float Compare2Feature(QsFace qsFace,FaceModel model1, FaceModel model2 ){
    	float score = qsFace.face2Face(model1.ptFeature, model2.ptFeature);
    	return score;
    }
    
    public static FaceModel[] DetectAndExtfeature(QsFace qsFace,String imageFilePath){
    	  Image image=null;
          try {
              File pathToFile = new File(imageFilePath);
              image= ImageIO.read(pathToFile);
          } catch (IOException ex) {
              ex.printStackTrace();
              return null;
          }
          BufferedImage bufferedImage=Util.toBufferedImage(image);
          if (bufferedImage==null)
              return null;
          byte[] bgr24=((DataBufferByte) bufferedImage.getRaster().getDataBuffer()).getData();
          int widht = bufferedImage.getWidth();
          int height = bufferedImage.getHeight();
          int widthStept = widht*3;
          //===================================================特征值检测，检测出的特征值用于相似度比对============================================
          FaceModel[] models = qsFace.detectFacesFeature(bgr24, widht, height, widthStept, 10);
          return models;
    }

 
}
