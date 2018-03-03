#include <jni.h>

#ifndef COM_FACE_BASE_H_INCLUDED
#define COM_FACE_BASE_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

jobject Create_FaceFeaturePoint(JNIEnv *jEnv, jobject jObj,int x,int y);

jobject Create_FaceFeatureRectangle(JNIEnv *jEnv, jobject jObj,int left,int top,int right,int bottom);

jobject Create_FaceModel(JNIEnv *jEnv, jobject jObj,jobject rtFace,jobject ptLeftEye,
                         jobject ptRightEye,jobject ptNose,jobject ptMouthLeft,
                         jobject ptMouthRight,jbyte* ptFeature,int featureSize);

jobject Create_FatigueDeterminationResult(JNIEnv *jEnv, jobject jObj,jobject rtFace,int fatigueResult);

jobject Create_FacePointModel(JNIEnv *jEnv, jobject jObj);

jobject Create_FacePointsFeatureResult(JNIEnv *jEnv, jobject jObj);



#ifdef __cplusplus
}
#endif


#endif // COM_FACE_BASE_H_INCLUDED
