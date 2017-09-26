echo start compile demo bin file 
/usr/bin/g++ com_qs_face_QsFace.cpp -shared -fPIC -o libwisfacejni.so \
-I"${JAVA_HOME}/include" -I"${JAVA_HOME}/include/linux" \
-L/root/wisfacesdk/app -Wl,-rpath=/root/wisfacesdk/app -lwisface \
-L/usr/lib/x86_64-linux-gnu/ -Wl,-rpath=/usr/lib/x86_64-linux-gnu -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_objdetect \
-Wl,-rpath,/usr/local/lib -fopenmp -L./ -Wl,-rpath=./
