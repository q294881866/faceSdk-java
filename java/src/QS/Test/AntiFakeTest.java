package QS.Test;

import com.qs.face.QsFace;
import com.qs.face.Util;

import java.io.IOException;

/**
 * Created by mazhi on 2017/6/10.
 * hautmz@163.com
 */
public class AntiFakeTest {
    public static void main(String[] args){
        QsFace qsFace=new QsFace();
        qsFace.init();
        String imagePath="1.jpg";
        //动作类型int（1~4）,1:眨眼 2：吐舌头 3：左右转脸 4：上下点头（暂不能用4）
        int facePoseType=1;
        try {
            //返回检测状态：1表示检测成功 、0表示需要持续检测（控制检测超时要在外层应用进行判断）、 -1表示检测失败
            int antiResult=qsFace.antiFake(Util.toBufferedImage(imagePath),facePoseType);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
