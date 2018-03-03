#include "stdafx.h"
#include "com_qs_face_QsFace.h"
#include <iostream>
#include "com_face_base.h"
#include <WisFaceEngineWrap.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <opencv/cxcore.h>
#define FILEPATH_MAX (80)
//
//#pragma comment(lib, "vfw32.lib" ) 
//#pragma comment(lib, "IlmImf.lib" )   
//#pragma comment(lib, "libjasper.lib" )  
//#pragma comment(lib, "libjpeg.lib" )  
//#pragma comment(lib, "libpng.lib" )  
//#pragma comment(lib, "libtiff.lib" ) 
//#pragma comment(lib, "zlib.lib" ) 
//#pragma comment(lib, "opencv_core249.lib" )   
//#pragma comment(lib, "opencv_objdetect249.lib" )  
//#pragma comment(lib, "opencv_highgui249.lib" )  
//#pragma comment(lib, "opencv_imgproc249.lib" ) 

using namespace std;
using namespace cv;

#define FACE_FEATURE_SIZE 1024

QsHANDLE ptWis;



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



JNIEXPORT jint JNICALL Java_com_qs_face_QsFace_createEngine(JNIEnv *jEnv, jobject jObj)
{
    if(ptWis==NULL){
        ptWis=qs_Wis_Create();
        return 0;
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
	{
		jsize feature1Len  = jEnv->GetArrayLength(feature1);
        jbyte *jbarrayFeature1 = (jbyte *)malloc(feature1Len * sizeof(jbyte));
		jEnv->GetByteArrayRegion(feature1,0,feature1Len,jbarrayFeature1);

		jsize feature2Len  = jEnv->GetArrayLength(feature2);
        jbyte *jbarrayFeature2 = (jbyte *)malloc(feature2Len * sizeof(jbyte));
		jEnv->GetByteArrayRegion(feature2,0,feature2Len,jbarrayFeature2);

        result=qs_Wis_Compare2Feature(ptWis,(BYTE*)jbarrayFeature1,(BYTE*)jbarrayFeature2);
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

	{

		jbyte* imageBuffer=jEnv->GetByteArrayElements(buffer,0);
        cv::Mat mat(height,width,CV_8UC3,imageBuffer,widthstep);

		QsFace *faceModels=new QsFace[maxFaceCount];

		int faceCount=qs_Wis_DetectFaces_Reinforce(ptWis,mat.data,mat.cols,
			mat.rows,mat.step,faceModels,maxFaceCount);
        printf("faceCount=%d,\n",faceCount);
		if(faceCount>0)
		{
			for(int i=0;i<faceCount;i++)
			{
				qs_Wis_ExtractFeature(ptWis,mat.data,mat.cols,mat.rows,mat.step,&faceModels[i]);
			}
		}

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
				int leftV=faceModels[i].rect.left;
				int rightV=faceModels[i].rect.right;
				int topV=faceModels[i].rect.top;
				int bottomV=faceModels[i].rect.bottom;

				(jEnv)->SetIntField(rtFaceObj,left,leftV);
				(jEnv)->SetIntField(rtFaceObj,top,topV);
				(jEnv)->SetIntField(rtFaceObj,right,rightV);
				(jEnv)->SetIntField(rtFaceObj,bottom,bottomV);

				//(jEnv)->SetIntField(ptLeftEyeObj,x,faceModels[i].ptLeftEye.x);
				//(jEnv)->SetIntField(ptLeftEyeObj,y,faceModels[i].ptLeftEye.y);

				//(jEnv)->SetIntField(ptRightEyeObj,x,faceModels[i].ptRightEye.x);
				//(jEnv)->SetIntField(ptRightEyeObj,y,faceModels[i].ptRightEye.y);

				//(jEnv)->SetIntField(ptNoseObj,x,faceModels[i].ptNose.x);
				//(jEnv)->SetIntField(ptNoseObj,y,faceModels[i].ptNose.y);

				//(jEnv)->SetIntField(ptMouthLeftObj,x,faceModels[i].ptMouthLeft.x);
				//(jEnv)->SetIntField(ptMouthLeftObj,y,faceModels[i].ptMouthLeft.y);

				//(jEnv)->SetIntField(ptMouthRightObj,x,faceModels[i].ptMouthRight.x);
				//(jEnv)->SetIntField(ptMouthRightObj,y,faceModels[i].ptMouthRight.y);

				(jEnv)->SetObjectField(model,rtFace,rtFaceObj);
				(jEnv)->SetObjectField(model,ptLeftEye,ptLeftEyeObj);
				(jEnv)->SetObjectField(model,ptRightEye,ptRightEyeObj);
				(jEnv)->SetObjectField(model,ptNose,ptNoseObj);
				(jEnv)->SetObjectField(model,ptMouthLeft,ptMouthLeftObj);
				(jEnv)->SetObjectField(model,ptMouthRight,ptMouthRightObj);

				jbyteArray  jbarray = (jEnv)->NewByteArray(FACE_FEATURE_SIZE);
				(jEnv)->SetByteArrayRegion(jbarray,0,FACE_FEATURE_SIZE,(const jbyte*)faceModels[i].feature);

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
        result=qs_Wis_Compare2Image(ptWis,c_image1,c_image2);
        jEnv->ReleaseStringUTFChars(img1, c_image1);
        jEnv->ReleaseStringUTFChars(img2, c_image2);
    }
   return result;
}



