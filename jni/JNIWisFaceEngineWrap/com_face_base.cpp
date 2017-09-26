#include "com_face_base.h"

jobject Create_FaceFeaturePoint(JNIEnv *jEnv, jobject jObj,int x,int y)
{
    if(jEnv==NULL)
        return NULL;

    jclass faceFeaturePointClass = (jEnv)->FindClass("Lcom/face/base/entity/FaceFeaturePoint;");
    if(faceFeaturePointClass==NULL)
        return NULL;
    jmethodID faceFeaturePointClassInit=(jEnv)->GetMethodID(faceFeaturePointClass,"<init>","()V");
    jfieldID xField=(jEnv)->GetFieldID(faceFeaturePointClass,"x","I");
    if(xField==0) return NULL;
    jfieldID yField=(jEnv)->GetFieldID(faceFeaturePointClass,"y","I");
    if(yField==0) return NULL;
    jobject faceFeaturePointObj=NULL;
    if(faceFeaturePointClassInit!=0)
    {
        faceFeaturePointObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
        if(faceFeaturePointObj!=NULL){
            (jEnv)->SetIntField(faceFeaturePointObj,xField,x);
            (jEnv)->SetIntField(faceFeaturePointObj,yField,y);
        }
    }

    jEnv->DeleteLocalRef(faceFeaturePointClass);
    return faceFeaturePointObj;

}

jobject Create_FaceFeatureRectangle(JNIEnv *jEnv, jobject jObj,int left,int top,int right,int bottom)
{
    if(jEnv==NULL)
        return NULL;
    jclass faceFeatureRectangleClass=(jEnv)->FindClass("Lcom/face/base/entity/FaceFeatureRectangle;");
    if(faceFeatureRectangleClass==0)
        return NULL;
    jmethodID faceFeatureRectangleClassInit=jEnv->GetMethodID(faceFeatureRectangleClass,"<init>","()V");
    jfieldID leftField=(jEnv)->GetFieldID(faceFeatureRectangleClass,"left","I");
    jfieldID topField=(jEnv)->GetFieldID(faceFeatureRectangleClass,"top","I");
    jfieldID rightField=(jEnv)->GetFieldID(faceFeatureRectangleClass,"right","I");
    jfieldID bottomField=(jEnv)->GetFieldID(faceFeatureRectangleClass,"bottom","I");

    jobject faceFeatureRectangleObj=NULL;
    if(faceFeatureRectangleClassInit!=0)
    {
        faceFeatureRectangleObj=jEnv->NewObject(faceFeatureRectangleClass,faceFeatureRectangleClassInit);
        if(faceFeatureRectangleObj!=NULL)
        {
            (jEnv)->SetIntField(faceFeatureRectangleObj,leftField,left);
            (jEnv)->SetIntField(faceFeatureRectangleObj,topField,top);
            (jEnv)->SetIntField(faceFeatureRectangleObj,rightField,right);
            (jEnv)->SetIntField(faceFeatureRectangleObj,bottomField,bottom);
        }

    }
    jEnv->DeleteLocalRef(faceFeatureRectangleClass);
    return faceFeatureRectangleObj;

}

jobject Create_FaceModel(JNIEnv *jEnv, jobject jObj,jobject rtFaceObj,jobject ptLeftEyeObj,
                         jobject ptRightEyeObj,jobject ptNoseObj,jobject ptMouthLeftObj,
                         jobject ptMouthRightObj,jbyte* ptFeature,int featureSize)
{
    if(jEnv==NULL)
        return NULL;
    jclass faceModelClass=(jEnv)->FindClass("Lcom/face/base/entity/FaceModel;");
    if(faceModelClass==0)
        return NULL;
    jmethodID faceModelClassInit=jEnv->GetMethodID(faceModelClass,"<init>","()V");
    if(faceModelClassInit==0)
        return NULL;
    jobject faceModelObj=NULL;
    jfieldID featureField=(jEnv)->GetFieldID(faceModelClass,"ptFeature","[B");
    jfieldID rtFaceField=(jEnv)->GetFieldID(faceModelClass,"rtFace","Lcom/face/base/entity/FaceFeatureRectangle;");
    jfieldID ptLeftEyeField=(jEnv)->GetFieldID(faceModelClass,"ptLeftEye","Lcom/face/base/entity/FaceFeaturePoint;");
    jfieldID ptRightEyeField=(jEnv)->GetFieldID(faceModelClass,"ptRightEye","Lcom/face/base/entity/FaceFeaturePoint;");
    jfieldID ptNoseField=(jEnv)->GetFieldID(faceModelClass,"ptNose","Lcom/face/base/entity/FaceFeaturePoint;");
    jfieldID ptMouthLeftField=(jEnv)->GetFieldID(faceModelClass,"ptMouthLeft","Lcom/face/base/entity/FaceFeaturePoint;");
    jfieldID ptMouthRightField=(jEnv)->GetFieldID(faceModelClass,"ptMouthRight","Lcom/face/base/entity/FaceFeaturePoint;");
    faceModelObj = (jEnv)->NewObject(faceModelClass, faceModelClassInit);
    if(faceModelObj!=NULL){
        (jEnv)->SetObjectField(faceModelObj,rtFaceField,rtFaceObj);
        (jEnv)->SetObjectField(faceModelObj,ptLeftEyeField,ptLeftEyeObj);
        (jEnv)->SetObjectField(faceModelObj,ptRightEyeField,ptRightEyeObj);
        (jEnv)->SetObjectField(faceModelObj,ptNoseField,ptNoseObj);
        (jEnv)->SetObjectField(faceModelObj,ptMouthLeftField,ptMouthLeftObj);
        (jEnv)->SetObjectField(faceModelObj,ptMouthRightField,ptMouthRightObj);
        jbyteArray  jbarrayObj = (jEnv)->NewByteArray(featureSize);
        (jEnv)->SetByteArrayRegion(jbarrayObj,0,featureSize,ptFeature);
        (jEnv)->SetObjectField(faceModelObj,featureField,jbarrayObj);
        jEnv->DeleteLocalRef(jbarrayObj);
    }

    jEnv->DeleteLocalRef(faceModelClass);
    return faceModelObj;
}

jobject Create_FatigueDeterminationResult(JNIEnv *jEnv, jobject jObj,jobject rtFaceObj,int fatigueResult)
{
    if(jEnv==NULL)
        return NULL;
    jclass fatigueDeterminationResultClass=(jEnv)->FindClass("Lcom/face/base/entity/FatigueDeterminationResult;");
    if(fatigueDeterminationResultClass==0)
        return NULL;
    jmethodID fatigueDeterminationResultInit=jEnv->GetMethodID(fatigueDeterminationResultClass,"<init>","()V");
    if(fatigueDeterminationResultInit==0)
        return NULL;
    jfieldID rtFaceField=(jEnv)->GetFieldID(fatigueDeterminationResultClass,"rtFace","Lcom/face/base/entity/FaceFeatureRectangle;");
    jfieldID fatigueDeterminationResultField=(jEnv)->GetFieldID(fatigueDeterminationResultClass,"fatigueDeterminationResult","I");
    jobject fatigueDeterminationResultObj=NULL;
    if(rtFaceField!=0&&fatigueDeterminationResultField!=0){
        fatigueDeterminationResultObj=(jEnv)->NewObject(fatigueDeterminationResultClass,
                                                            fatigueDeterminationResultInit);
        if(fatigueDeterminationResultObj!=NULL){
            (jEnv)->SetObjectField(fatigueDeterminationResultObj,
                               rtFaceField,rtFaceObj);
            (jEnv)->SetIntField(fatigueDeterminationResultObj,fatigueDeterminationResultField,fatigueResult);
        }

    }

    jEnv->DeleteLocalRef(fatigueDeterminationResultClass);
    return fatigueDeterminationResultObj;

}


