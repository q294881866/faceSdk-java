#include "com_qs_face_QsFace.h"
#include <iostream>
#include "WisFaceSDK.h"
#include <com_face_base.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>  //包含了Linux C 中的函数getcwd()
#define FILEPATH_MAX (80)

using namespace std;
using namespace cv;
using namespace Qiansou;
#define FACE_FEATURE_SIZE 1024

CFaceWis *ptWis=NULL;
int fatigueDetectCount=5;
int eyeThresold=6;
int mouthThresold=10;

int MuothSum=0;
int EyeSum=0;

char* jstringTostring(JNIEnv* env, jstring jstr)
{
       char* rtn = NULL;
       jclass clsstring = env->FindClass("java/lang/String");
       jstring strencode = env->NewStringUTF("utf-8");
       jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
       jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
       jsize alen = env->GetArrayLength(barr);
       jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
       if (alen > 0)
       {
                 rtn = (char*)malloc(alen + 1);
                 memcpy(rtn, ba, alen);
                 rtn[alen] = 0;
       }
       env->ReleaseByteArrayElements(barr, ba, 0);
       return rtn;
}

//char* to jstring
jstring stoJstring(JNIEnv* env, const char* pat)
{
       jclass strClass = env->FindClass("Ljava/lang/String;");
       jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
       jbyteArray bytes = env->NewByteArray(strlen(pat));
       env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
       jstring encoding = env->NewStringUTF("utf-8");
       return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}

bool EyeOrEye(FaceModel faceModel)
{

    if(faceModel.landmark.size()>24){
    int x = faceModel.landmark[24].y-faceModel.landmark[20].y;
    printf("24x=%d\n",faceModel.landmark[24].y);
    if (x <= eyeThresold)
        return true;
    }
    return false;
}

bool Mouth(FaceModel faceModel)
{

    if(faceModel.landmark.size()>47){
        int x = faceModel.landmark[47].y - faceModel.landmark[44].y;
        if (x >= mouthThresold)
        {
            return true;
        }
    }

    return false;
}

JNIEXPORT jint JNICALL Java_com_qs_face_QsFace_createEngine(JNIEnv *jEnv, jobject jObj)
{
    if(ptWis==NULL){
        ptWis=new CFaceWis();
        return ptWis->Initial();
    }else{
        return -1;
    }
}

JNIEXPORT void JNICALL Java_com_qs_face_QsFace_freeEngine
  (JNIEnv *jEnv, jobject jObj, jint engine)
{
	return;
}

JNIEXPORT jfloat JNICALL Java_com_qs_face_QsFace_compare2Feature
  (JNIEnv *jEnv, jobject jObj, jint enginge, jbyteArray feature1, jbyteArray feature2)
{
	jfloat result=-1;
	if(feature1==NULL||feature2==NULL)
		return result;
    if(ptWis!=NULL)
//	if(enginge!=0)
	{
		jsize feature1Len  = jEnv->GetArrayLength(feature1);
        jbyte *jbarrayFeature1 = (jbyte *)malloc(feature1Len * sizeof(jbyte));
		jEnv->GetByteArrayRegion(feature1,0,feature1Len,jbarrayFeature1);

		jsize feature2Len  = jEnv->GetArrayLength(feature2);
        jbyte *jbarrayFeature2 = (jbyte *)malloc(feature2Len * sizeof(jbyte));
		jEnv->GetByteArrayRegion(feature2,0,feature2Len,jbarrayFeature2);

        result=ptWis->Compare2Feature((float*)jbarrayFeature1,(float*)jbarrayFeature2);
		free(jbarrayFeature1);
		free(jbarrayFeature2);
	}
	return result;
}


JNIEXPORT jobjectArray JNICALL Java_com_qs_face_QsFace_process
  (JNIEnv *jEnv, jobject jObj, jint enginge, jbyteArray buffer, jint width, jint height, jint widthstep, jint maxFaceCount)
{

	jobjectArray args=0;
	if(buffer==NULL)
		return args;
    if(ptWis!=NULL)
//	if(enginge!=0)
	{
		//获取buffer中的数据
		jbyte* imageBuffer=jEnv->GetByteArrayElements(buffer,0);
        cv::Mat mat(height,width,CV_8UC3,imageBuffer,widthstep);
//        int params[3];
//
//         params[0] = CV_IMWRITE_JPEG_QUALITY;
//         params[1] = 85;//设置s压缩度
//         IplImage img =(IplImage)mat;
//         cvSaveImage("a.jpg",&img,params);
//        cvSaveImage("a.jpg",mat，params);
		FaceModel *faceModels=new FaceModel[maxFaceCount];
//		int faceCount=qs_Wis_DetectFaces(enginge,mat,faceModels,maxFaceCount);
        int faceCount=ptWis->DetectFace(mat,faceModels,maxFaceCount);
        printf("faceCount=%d,\n",faceCount);
		if(faceCount>0)
		{
			for(int i=0;i<faceCount;i++)
			{
//				qs_Wis_ExtractFeature(enginge,mat,&faceModels[i]);
                ptWis->ExtractFeature(mat,faceModels[i]);
			}
		}
		//检测完毕后释放buffer
		//(jEnv)->DeleteLocalRef(imageBuffer);
		if(faceCount>0)
		{
			jclass faceFeaturePointClass = (jEnv)->FindClass("Lcom/qs/face/entity/FaceFeaturePoint;");
			if(faceFeaturePointClass==0)
				goto complete;
			jmethodID faceFeaturePointClassInit=(jEnv)->GetMethodID(faceFeaturePointClass,"<init>","()V");
			if(faceFeaturePointClassInit==0)
				goto complete;
			jclass faceFeatureRectangleClass=(jEnv)->FindClass("Lcom/qs/face/entity/FaceFeatureRectangle;");
			if(faceFeatureRectangleClass==0)
				goto complete;
			jmethodID faceFeatureRectangleClassInit=jEnv->GetMethodID(faceFeatureRectangleClass,"<init>","()V");
			if(faceFeatureRectangleClassInit==0)
				goto complete;
			jclass faceModelClass=(jEnv)->FindClass("Lcom/qs/face/entity/FaceModel;");
			if(faceModelClass==0)
				goto complete;
			jmethodID faceModelClassInit=jEnv->GetMethodID(faceModelClass,"<init>","()V");
			if(faceModelClassInit==0)
				goto complete;
			jfieldID left=(jEnv)->GetFieldID(faceFeatureRectangleClass,"left","I");
			jfieldID top=(jEnv)->GetFieldID(faceFeatureRectangleClass,"top","I");
			jfieldID right=(jEnv)->GetFieldID(faceFeatureRectangleClass,"right","I");
			jfieldID bottom=(jEnv)->GetFieldID(faceFeatureRectangleClass,"bottom","I");
			if(left==0||top==0||right==0||bottom==0) goto complete;
			jfieldID x=(jEnv)->GetFieldID(faceFeaturePointClass,"x","I");
			if(x==0) goto complete;
			jfieldID y=(jEnv)->GetFieldID(faceFeaturePointClass,"y","I");
			if(y==0) goto complete;
			jfieldID feature=(jEnv)->GetFieldID(faceModelClass,"ptFeature","[B");
			if(feature==0) goto complete;
			jfieldID rtFace=(jEnv)->GetFieldID(faceModelClass,"rtFace","Lcom/qs/face/entity/FaceFeatureRectangle;");
			if(rtFace==0) goto complete;
			jfieldID ptLeftEye=(jEnv)->GetFieldID(faceModelClass,"ptLeftEye","Lcom/qs/face/entity/FaceFeaturePoint;");
			if(ptLeftEye==0) goto complete;
			jfieldID ptRightEye=(jEnv)->GetFieldID(faceModelClass,"ptRightEye","Lcom/qs/face/entity/FaceFeaturePoint;");
			if(ptRightEye==0) goto complete;
			jfieldID ptNose=(jEnv)->GetFieldID(faceModelClass,"ptNose","Lcom/qs/face/entity/FaceFeaturePoint;");
			if(ptNose==0) goto complete;
			jfieldID ptMouthLeft=(jEnv)->GetFieldID(faceModelClass,"ptMouthLeft","Lcom/qs/face/entity/FaceFeaturePoint;");
			if(ptMouthLeft==0) goto complete;
			jfieldID ptMouthRight=(jEnv)->GetFieldID(faceModelClass,"ptMouthRight","Lcom/qs/face/entity/FaceFeaturePoint;");
			if(ptMouthRight==0) goto complete;
			args = (jEnv)->NewObjectArray(faceCount, faceModelClass, 0);
			if(args==0) goto complete;
			for(int i=0;i<faceCount;i++)
			{
				jobject rtFaceObj=jEnv->NewObject(faceFeatureRectangleClass,faceFeatureRectangleClassInit);
				jobject ptLeftEyeObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
				jobject ptRightEyeObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
				jobject ptNoseObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
				jobject ptMouthLeftObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
				jobject ptMouthRightObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
				jobject model=jEnv->NewObject(faceModelClass,faceModelClassInit);
                int leftV=faceModels[i].rtFace.x;
                int rightV=faceModels[i].rtFace.x+faceModels[i].rtFace.width;
                int topV=faceModels[i].rtFace.y;
                int bottomV=faceModels[i].rtFace.y+faceModels[i].rtFace.height;

				(jEnv)->SetIntField(rtFaceObj,left,leftV);
				(jEnv)->SetIntField(rtFaceObj,top,topV);
				(jEnv)->SetIntField(rtFaceObj,right,rightV);
				(jEnv)->SetIntField(rtFaceObj,bottom,bottomV);

				(jEnv)->SetIntField(ptLeftEyeObj,x,faceModels[i].ptLeftEye.x);
				(jEnv)->SetIntField(ptLeftEyeObj,y,faceModels[i].ptLeftEye.y);

				(jEnv)->SetIntField(ptRightEyeObj,x,faceModels[i].ptRightEye.x);
				(jEnv)->SetIntField(ptRightEyeObj,y,faceModels[i].ptRightEye.y);

				(jEnv)->SetIntField(ptNoseObj,x,faceModels[i].ptNose.x);
				(jEnv)->SetIntField(ptNoseObj,y,faceModels[i].ptNose.y);

				(jEnv)->SetIntField(ptMouthLeftObj,x,faceModels[i].ptMouthLeft.x);
				(jEnv)->SetIntField(ptMouthLeftObj,y,faceModels[i].ptMouthLeft.y);

				(jEnv)->SetIntField(ptMouthRightObj,x,faceModels[i].ptMouthRight.x);
				(jEnv)->SetIntField(ptMouthRightObj,y,faceModels[i].ptMouthRight.y);

				(jEnv)->SetObjectField(model,rtFace,rtFaceObj);
				(jEnv)->SetObjectField(model,ptLeftEye,ptLeftEyeObj);
				(jEnv)->SetObjectField(model,ptRightEye,ptRightEyeObj);
				(jEnv)->SetObjectField(model,ptNose,ptNoseObj);
				(jEnv)->SetObjectField(model,ptMouthLeft,ptMouthLeftObj);
				(jEnv)->SetObjectField(model,ptMouthRight,ptMouthRightObj);

				jbyteArray  jbarray = (jEnv)->NewByteArray(FACE_FEATURE_SIZE);
				(jEnv)->SetByteArrayRegion(jbarray,0,FACE_FEATURE_SIZE,(const jbyte*)faceModels[i].ptFeature);

				(jEnv)->SetObjectField(model,feature,jbarray);
				 (jEnv)->SetObjectArrayElement(args,i,model);

				 jEnv->DeleteLocalRef(rtFaceObj);
				 jEnv->DeleteLocalRef(ptLeftEyeObj);
				 jEnv->DeleteLocalRef(ptRightEyeObj);
				 jEnv->DeleteLocalRef(ptNoseObj);
				 jEnv->DeleteLocalRef(model);
				 jEnv->DeleteLocalRef(jbarray);
				 jEnv->DeleteLocalRef(ptMouthLeftObj);
				 jEnv->DeleteLocalRef(ptMouthRightObj);
			}
			jEnv->DeleteLocalRef(faceFeaturePointClass);
			jEnv->DeleteLocalRef(faceFeatureRectangleClass);
			jEnv->DeleteLocalRef(faceModelClass);

//			jEnv->DeleteLocalRef(faceFeaturePointClassInit);
//			jEnv->DeleteLocalRef(faceFeatureRectangleClassInit);
//			jEnv->DeleteLocalRef(faceModelClassInit);

//			jEnv->DeleteLocalRef(left);
//			jEnv->DeleteLocalRef(top);
//			jEnv->DeleteLocalRef(right);
//			jEnv->DeleteLocalRef(bottom);
//			jEnv->DeleteLocalRef(x);
//			jEnv->DeleteLocalRef(y);
//			jEnv->DeleteLocalRef(feature);
//			jEnv->DeleteLocalRef(rtFace);
//			jEnv->DeleteLocalRef(ptLeftEye);
//
//			jEnv->DeleteLocalRef(ptRightEye);
//			jEnv->DeleteLocalRef(ptNose);
//			jEnv->DeleteLocalRef(ptMouthLeft);
//			jEnv->DeleteLocalRef(ptMouthRight);
//			jEnv->DeleteLocalRef(args);



		}
complete:
		delete[] faceModels;

	}
	return args;
}


JNIEXPORT jfloat JNICALL Java_com_qs_face_QsFace_compare2Image
  (JNIEnv *jEnv, jobject jObj, jint enginge, jstring img1, jstring img2)
{
    jfloat result=-100;
    if(ptWis!=NULL){
        const char *c_image1 = jEnv->GetStringUTFChars(img1, 0);
        const char *c_image2 = jEnv->GetStringUTFChars(img2, 0);
        result=ptWis->Compare2Image(c_image1,c_image2);
        jEnv->ReleaseStringUTFChars(img1, c_image1);
        jEnv->ReleaseStringUTFChars(img2, c_image2);
    }
   return result;
}



JNIEXPORT jobject JNICALL Java_com_qs_face_QsFace_calculateFacePoints
  (JNIEnv *jEnv, jclass jObj,  jint enginge,jbyteArray buffer, jint width, jint height, jint widthstep,
   jint x,jint y,jint rectWidth,jint rectHeight)
{
        jobject args=NULL;
        if(buffer==NULL)
            return args;
        if(ptWis!=NULL)
        {
            jbyte* imageBuffer=jEnv->GetByteArrayElements(buffer,0);

            cv::Mat mat(height,width,CV_8UC3,imageBuffer,widthstep);
            cv::Mat greyMat;
            cv::cvtColor(mat, greyMat, CV_BGR2GRAY);

//            jclass faceFeatureRectangleClass=(jEnv)->FindClass("Lcom/qs/face/entity/FaceFeatureRectangle;");
//			if(faceFeatureRectangleClass==0)
//				return args;
//            jfieldID feature=(jEnv)->GetFieldID(faceModelClass,"ptFeature","[B");
//			if(feature==0) return args;;
//			jfieldID rtFace=(jEnv)->GetFieldID(faceModelClass,"rtFace","Lcom/qs/face/entity/FaceFeatureRectangle;");
//			if(rtFace==0) return args;;
//			jfieldID ptLeftEye=(jEnv)->GetFieldID(faceModelClass,"ptLeftEye","Lcom/qs/face/entity/FaceFeaturePoint;");
//			if(ptLeftEye==0) return args;;
//			jfieldID ptRightEye=(jEnv)->GetFieldID(faceModelClass,"ptRightEye","Lcom/qs/face/entity/FaceFeaturePoint;");
//			if(ptRightEye==0) return args;;
//			jfieldID ptNose=(jEnv)->GetFieldID(faceModelClass,"ptNose","Lcom/qs/face/entity/FaceFeaturePoint;");
//			if(ptNose==0) return args;
//			jfieldID ptMouthLeft=(jEnv)->GetFieldID(faceModelClass,"ptMouthLeft","Lcom/qs/face/entity/FaceFeaturePoint;");
//			if(ptMouthLeft==0) return args;
//			jfieldID ptMouthRight=(jEnv)->GetFieldID(faceModelClass,"ptMouthRight","Lcom/qs/face/entity/FaceFeaturePoint;");
//			if(ptMouthRight==0) return args;
//			jobject rtFaceObj=(jEnv)->GetObjectField(faceModel,rtFace);
//			if(rtFaceObj==NULL)
//                return args;
//            jfieldID left=(jEnv)->GetFieldID(faceFeatureRectangleClass,"left","I");
//			jfieldID top=(jEnv)->GetFieldID(faceFeatureRectangleClass,"top","I");
//			jfieldID right=(jEnv)->GetFieldID(faceFeatureRectangleClass,"right","I");
//			jfieldID bottom=(jEnv)->GetFieldID(faceFeatureRectangleClass,"bottom","I");
//            jint leftValue=(jEnv)->GetIntField(rtFaceObj,left);
//
//            jint topValue=(jEnv)->GetIntField(rtFaceObj,top);
//            jint rightValue=(jEnv)->GetIntField(rtFaceObj,right);
//            jint bottomValue=(jEnv)->GetIntField(rtFaceObj,bottom);


//            printf("left=%d,\n",leftValue);printf("top=%d,\n",topValue);printf("right=%d,\n",rightValue);printf("bottom=%d,\n",bottomValue);
            FaceModel face;
            cv::Rect cvrect;
            cvrect.x=x;
            cvrect.y=y;
            cvrect.width=rectWidth;
            cvrect.height=rectHeight;
            face.rtFace=cvrect;

            int result=ptWis->GetFacePoints(greyMat,face);
//            if(result==0){
                jclass facePointsFeatureResultClass=(jEnv)->FindClass("Lcom/qs/face/entity/FacePointsFeatureResult;");
                jfieldID scoleFiled=(jEnv)->GetFieldID(facePointsFeatureResultClass,"scole","F");
                jfieldID facePointsFiled=(jEnv)->GetFieldID(facePointsFeatureResultClass,"facePoints","Lcom/qs/face/entity/FacePointModel;");
                jmethodID facePointsFeatureResultClassInit=(jEnv)->GetMethodID(facePointsFeatureResultClass,"<init>","()V");

                jclass facePointModelClass=(jEnv)->FindClass("Lcom/qs/face/entity/FacePointModel;");
                jmethodID facePointModelClassInit=(jEnv)->GetMethodID(facePointModelClass,"<init>","()V");
                jfieldID roolFiled=(jEnv)->GetFieldID(facePointModelClass,"roll","F");
                jfieldID yawFiled=(jEnv)->GetFieldID(facePointModelClass,"yaw","F");
                jfieldID pitchFiled=(jEnv)->GetFieldID(facePointModelClass,"pitch","F");
                jfieldID pointsFiled=(jEnv)->GetFieldID(facePointModelClass,"points","[Lcom/qs/face/entity/FaceFeaturePoint;");

                jclass faceFeaturePointClass=(jEnv)->FindClass("Lcom/qs/face/entity/FaceFeaturePoint;");
                jmethodID faceFeaturePointClassInit=(jEnv)->GetMethodID(faceFeaturePointClass,"<init>","()V");
                jfieldID xField=(jEnv)->GetFieldID(faceFeaturePointClass,"x","I");
                jfieldID yField=(jEnv)->GetFieldID(faceFeaturePointClass,"y","I");
                int pointCount=(int)face.landmark.size();
                jobjectArray pointsObj = (jEnv)->NewObjectArray(pointCount, faceFeaturePointClass, 0);


                for (int i =0; i < pointCount; i ++){
                    jobject faceFeaturePointObj=jEnv->NewObject(faceFeaturePointClass,faceFeaturePointClassInit);
                    (jEnv)->SetIntField(faceFeaturePointObj,xField,face.landmark[i].x);
                    (jEnv)->SetIntField(faceFeaturePointObj,yField,face.landmark[i].y);
                    (jEnv)->SetObjectArrayElement(pointsObj,i,faceFeaturePointObj);
                    jEnv->DeleteLocalRef(faceFeaturePointObj);
                }

                jobject facePointModelObj=jEnv->NewObject(facePointModelClass,facePointModelClassInit);
                (jEnv)->SetFloatField(facePointModelObj,roolFiled,face.roll);
                (jEnv)->SetFloatField(facePointModelObj,yawFiled,face.yaw);
                (jEnv)->SetFloatField(facePointModelObj,pitchFiled,face.pitch);
                (jEnv)->SetObjectField(facePointModelObj,pointsFiled,pointsObj);

                jobject facePointsFeatureResultObj=jEnv->NewObject(facePointsFeatureResultClass,facePointsFeatureResultClassInit);
                (jEnv)->SetFloatField(facePointsFeatureResultObj,scoleFiled,face.conf);
                (jEnv)->SetObjectField(facePointsFeatureResultObj,facePointsFiled,facePointModelObj);

                jEnv->DeleteLocalRef(facePointModelObj);
//                jEnv->DeleteLocalRef(facePointModelObj);

                jEnv->DeleteLocalRef(facePointsFeatureResultClass);
                jEnv->DeleteLocalRef(facePointModelClass);
                jEnv->DeleteLocalRef(faceFeaturePointClass);

                return facePointsFeatureResultObj;
//            }

        }
        return NULL;

}

JNIEXPORT int JNICALL Java_com_qs_face_QsFace_antiFakeDetect
  (JNIEnv *jEnv, jobject jObj, jint enginge, jbyteArray buffer, jint width, jint height, jint widthstep,jint facePoseType)
{
    int result=-1;
    if(buffer==NULL || ptWis==NULL)
		return result;
    jbyte* imageBuffer=jEnv->GetByteArrayElements(buffer,0);
    cv::Mat mat(height,width,CV_8UC3,imageBuffer,widthstep);
    int maxFaceCount=1;
    FaceModel *faceModels=new FaceModel[maxFaceCount];
    int faceCount=ptWis->DetectFace(mat,faceModels,maxFaceCount);
    if(faceCount>0)
    {
        result=ptWis->AntiFakeDetect(mat,faceModels[0],facePoseType);
    }
    delete[] faceModels;
    return result;
}


JNIEXPORT jobject JNICALL Java_com_qs_face_QsFace_fatigueDetectAPI
  (JNIEnv *jEnv, jobject jObj, jint enginge, jbyteArray buffer, jint width, jint height, jint widthstep)
{
    if(ptWis==NULL){
        return NULL;
    }
    jobject fatigueDeterminationResultObj=NULL;
    jbyte* imageBuffer=jEnv->GetByteArrayElements(buffer,0);
    cv::Mat mat(height,width,CV_8UC3,imageBuffer,widthstep);
    int maxFaceCount=1;
    FaceModel *faceModels=new FaceModel[maxFaceCount];
    int faceCount=ptWis->DetectFace(mat,faceModels,maxFaceCount);
    int detectiveResult=-1;
    if(faceCount>0){
        bool IsFatigue=false;
        int leftV=faceModels[0].rtFace.x;int rightV=faceModels[0].rtFace.x+faceModels[0].rtFace.width;int topV=faceModels[0].rtFace.y;
        int bottomV=faceModels[0].rtFace.y+faceModels[0].rtFace.height;
        int result=ptWis->GetFacePoints(mat,faceModels[0]);
        if (EyeOrEye(faceModels[0]) && Mouth(faceModels[0])){
            MuothSum++;
            if (MuothSum >= fatigueDetectCount)
                IsFatigue = true;
            EyeSum = 0;
        }
        else if (EyeOrEye(faceModels[0])){
            EyeSum++;
            if (EyeSum >= fatigueDetectCount)
                IsFatigue = true;
            MuothSum = 0;
        }
        else{
            EyeSum = 0;MuothSum = 0;IsFatigue = false;
        }
        if(IsFatigue)
            detectiveResult=0;
        else
            detectiveResult=1;
        jobject rtFaceObj=Create_FaceFeatureRectangle(jEnv,jObj,leftV,topV,rightV,bottomV);

        fatigueDeterminationResultObj = Create_FatigueDeterminationResult(jEnv,
                                        jObj,rtFaceObj,detectiveResult);



        jEnv->DeleteLocalRef(rtFaceObj);
//        jEnv->DeleteLocalRef(fatigueDeterminationResultObj);

    }
    delete[] faceModels;
    return fatigueDeterminationResultObj;
}

JNIEXPORT void JNICALL Java_com_qs_face_QsFace_setFatigueDetectCountAPI
  (JNIEnv *jEnv, jobject jObj, jint engine,jint fatigueDetectCountArgs,jint eyeThresoldArgs,jint mouthThresoldArgs)
  {
        fatigueDetectCount=fatigueDetectCountArgs;
        eyeThresold=eyeThresoldArgs;
        mouthThresold=mouthThresoldArgs;
  }
