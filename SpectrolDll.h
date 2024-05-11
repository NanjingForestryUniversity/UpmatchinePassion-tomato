#ifndef RTSPECTROL_H
#define RTSPECTROL_H

#ifndef Q_DECL_IMPORT
#define Q_DECL_IMPORT   __declspec(dllimport)
#endif
#ifndef Q_DECL_EXPORT
#define Q_DECL_EXPORT   __declspec(dllexport)
#endif
#ifdef RTSPECTROL_LIBRARY
    #define RT_API extern "C" Q_DECL_EXPORT
#else
    #define RT_API extern "C" Q_DECL_IMPORT
#endif
RT_API bool __stdcall RTGetCameraName( wchar_t* wcCameraName );
RT_API bool __stdcall RTSetCameraCaptureStyle(int iStyle);
RT_API int __stdcall RTOpenCamera();
RT_API bool __stdcall RTCloseCamera();
RT_API bool __stdcall RTStartAutoExposetime();

RT_API bool __stdcall RTSetExposure(float expostime);
RT_API bool __stdcall RTGetExposure(float& expostime);
RT_API bool __stdcall RTSetCameraGain(float fgain);
RT_API bool __stdcall RTGetCameraGain(float& fgain);
RT_API bool __stdcall RTGetImageProperty(int& iWidth, int& iHeight);
RT_API bool __stdcall RT_SetBool(const wchar_t* Feature,bool flag);
RT_API bool __stdcall RTSetCameraBinning(int iBinX,int iBinY);
RT_API bool __stdcall RTGetCameraBinning(int& iBinX,int& iBinY);
RT_API bool __stdcall RTGetCameraFrameRate(float& fRate);
RT_API bool __stdcall RTSetCameraFrameRate(float fRate);

RT_API int __stdcall RTGetCameraDepth();
RT_API bool __stdcall RTApplyCameraParam();


RT_API bool __stdcall RTCameraSnapshot(const char* cstrPath,int iTimes);

RT_API long __stdcall RTAddRGBCallback( void (__stdcall *VideoRGBFilter)(
	void *pContext,unsigned char *pRData,unsigned char *pGData,
	unsigned char *pBData, unsigned long dataLength),
	void *pCBContext );

RT_API int __stdcall RTRemoveRGBCallback( long callbackId );

RT_API bool __stdcall RTStartSingleCapture(bool bMotor = false,bool bSave = false,
	const char* savePath = "", bool bFlip = false,bool bRef = false, bool bcorrect = false);

RT_API bool __stdcall RTSetBWPath(const char* bwPath);

RT_API bool __stdcall RTStopSingleCapture();

RT_API bool __stdcall RTGetMinAndMaxWavelength(float& fmin,float& fmax);
RT_API bool __stdcall RTGetRGBselect(int& iRed,int& iGreen, int& iBlue);
RT_API bool __stdcall RTSetRGBselect(int iRed,int iGreen, int iBlue);

RT_API const double* __stdcall RTGetCurentCalibrationData();

RT_API int __stdcall RTGetLastError(wchar_t* errobuf,int len);

RT_API bool __stdcall RTIsCameraWorking();

RT_API bool __stdcall RTGetRealFrameRate(float& frame);

RT_API bool __stdcall RTCaptureAllDarkFrame();

RT_API bool __stdcall RTGetLastRealFrameRate(float& frame);

RT_API long __stdcall RTAddStreamingCallback(
	void(__stdcall *VideoFilter)(void *pContext, unsigned char *pData, unsigned long dataLength),
	void *pCBContext);

RT_API int __stdcall RTRemoveStreamingCallback(long callbackId);

RT_API int __stdcall RTAddMessageCallback(	void(__stdcall *VideoFilter)(void *pContext,const wchar_t *pData, unsigned long dataLength),
	void *pCBContext);

RT_API int __stdcall RTBWCorrection(const char* cOriFile, const char* cOutputFile, const char* cDCFile, const char* cWCFile, const char* cWhiteDarkFile,int correctsize = 0);


#endif
