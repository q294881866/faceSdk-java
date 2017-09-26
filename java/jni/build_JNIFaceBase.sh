echo start compile demo bin file 
/usr/bin/g++ com_face_base.cpp -shared -fPIC -o libjnifaceentity.so \
-I"${JAVA_HOME}/include" -I"${JAVA_HOME}/include/linux" \

