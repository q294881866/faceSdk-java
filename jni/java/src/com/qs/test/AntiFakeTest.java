package com.qs.test;

import java.io.IOException;

import com.qs.face.QsFace;
import com.qs.face.Util;

/**
 * 人脸指令防伪
 * Created by mazhi on 2017/6/10. hautmz@163.com
 * 
 * @update 2018/5/3 by Bevis Pei<ppf@jiumao.org> 
 */
public class AntiFakeTest {
    public static void main(String[] args) {
        QsFace qsFace = new QsFace();
        qsFace.init();
        String imagePath = "1.jpg";
        // 动作类型int（1~4）,1:眨眼 2：吐舌头 3：左右转脸 4：上下点头（暂不能用4）
        int facePoseType = 1;
        try {
            // 返回检测状态：1表示检测成功 、0表示需要持续检测（控制检测超时要在外层应用进行判断）、 -1表示检测失败
            int antiResult = qsFace.antiFake(Util.toBufferedImage(imagePath), facePoseType);
            System.out.println(antiResult);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
