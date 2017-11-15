package com.qs;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;

import com.sun.jna.Structure;


/**
 * 实现{@link #getFieldOrder()},业务使用需要注意是否符合需求
 * 
 * @author ppf@jiumao.org
 * @date 2017年11月8日
 */
public class DefaultStruct extends Structure {

    /**
     * 只获取 public 字段
     */
    @Override
    protected List<?> getFieldOrder() {
        Field[] fs = this.getClass().getFields();
        List<String> ls = new ArrayList<String>(fs.length);
        for (Field f : fs) {
            if (!f.getName().startsWith("ALIGN")) {
                ls.add(f.getName());
            }
        }
        return ls;
    }

}
