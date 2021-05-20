//
//  MNNFaceDetector.h
//  MNNFaceDetection
//
//  Created by MNN on 2019/12/2.
//  Copyright © 2019, Alibaba Group Holding Limited
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MNNKitCore/CommonDef.h>

static NSString *MNNKitErrorDomain = @"MNNKitErrorDomain";


/// Face detect config used to configure which face actions the detector cares about.
/// It's usually used in the video detection mode: `MNN_FACE_DETECT_MODE_VIDEO`.
typedef NS_ENUM(NSUInteger, MNNFaceDetectConfig) {
    EYE_BLINK         = 1<<1,     ///< Eye blink
    MOUTH_AH          = 1<<2,     ///< Mouth open
    HEAD_YAW          = 1<<3,     ///< Yaw.
    HEAD_PITCH        = 1<<4,     ///< Pitch.
    BROW_JUMP         = 1<<5,     ///< Brow jump.
};

/// Face detect mode.
/// In `MNN_FACE_DETECT_MODE_VIDEO`, the detection is run by default every 20 frames while the rest of the frames are only used for tracking.
/// In `MNN_FACE_DETECT_MODE_IMAGE`, each frame will trigger the detection.
typedef NS_ENUM(NSUInteger, MNNFaceDetectMode) {
    MNN_FACE_DETECT_MODE_VIDEO = 0x10000000,  ///< video detect mode
    MNN_FACE_DETECT_MODE_IMAGE = 0x20000000,  ///< image detect mode

};
/// Configuration used to create the FaceDetector.
@interface MNNFaceDetectorCreateConfig : NSObject

/// The face detect mode of the detector.
@property (nonatomic, assign) MNNFaceDetectMode detectMode;

@end

/// Face detection result object.
@interface MNNFaceDetectionReport : NSObject

/// Bounding box for the face.
@property (nonatomic, assign) CGRect rect;

/// A unique identification for the face. A new faceId will be generated if face tracking is lost and then detected successfully again.
@property (nonatomic, assign) NSInteger faceId;

/// 106 feature points on the face.
@property (nonatomic, assign) CGPoint *keyPoints;

/// Visibility of feature points, 1.0 if visible, 0.0 if invisible
@property (nonatomic, assign) CGFloat *visibilities;

/// Confidence score for the input frame to be a face.
@property (nonatomic, assign) CGFloat score;

/// Yaw angle, left -, right +. See  https://github.com/alibaba/MNNKit/blob/master/doc/face_ypr.png
@property (nonatomic, assign) CGFloat yaw;

/// Pitch angle, up -, down +. See https://github.com/alibaba/MNNKit/blob/master/doc/face_ypr.png
@property (nonatomic, assign) CGFloat pitch;

/// Roll angle, left -, right +. See https://github.com/alibaba/MNNKit/blob/master/doc/face_ypr.png
@property (nonatomic, assign) CGFloat roll;

/// Face actions in the video mode. Could be a combination of the enums in `MNNFaceDetectConfig`.
@property (nonatomic, assign) unsigned long faceAction;

@end


/// Face Detector Class.
/// See [here] (https://github.com/alibaba/MNNKit/blob/master/doc/FaceDetection_CN.md) for its detailed usage.
@interface MNNFaceDetector : NSObject

/// Creates a face detector instance asynchronously. The instance is passed in the callback in the main thread.
/// @param config config parameter for the creation,  such as video detection or image detection mode.
/// @param block call back after creation, error is `nil` if the call back is successful.
+ (void)createInstanceAsync:(MNNFaceDetectorCreateConfig*)config callback:(void(^)(NSError *error, MNNFaceDetector *faceDetector))block;

/// Creates a face detector instance asynchronously. The instance is passed in the  callback in the designated thread specified by `callbackQueue`.
/// @param config config parameter for the creation,  such as video detection or image detection mode.
/// @param block call back after creation, error is `nil` if the call back is successful.
/// @param callbackQueue call back in this designated thread, in main thread if `nil`.
+ (void)createInstanceAsync:(MNNFaceDetectorCreateConfig*)config callback:(void(^)(NSError *error, MNNFaceDetector *faceDetector))block callbackQueue:(dispatch_queue_t)callbackQueue;


/// Detects faces in the system camera input.
/// @param pixelBuffer input data in CVPixelBufferRef format
/// @param detectConfig detect config, see also `MNNFaceDetectConfig` enum.
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image. The face would be in the upright orientation after the rotation.
/// @param outAngle output angle, the coordinate of raw output feature points will rotate `outAngle` degree in in the image coordinate system. Generally in order to reach the same direction with the rendering coordinate system, then feature points will be easily rendered.
/// @param flipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message, `nil` if the inference is successful
- (NSArray<MNNFaceDetectionReport *> *)inferenceWithPixelBuffer:(CVPixelBufferRef)pixelBuffer config:(MNNFaceDetectConfig)detectConfig
                                                          angle:(float)inAngle outAngle:(float)outAngle flipType:(MNNFlipType)flipType
                                                          error:(NSError *__autoreleasing *)error;

/// Detects faces in the input image.
/// @param image image in UIImage format
/// @param detectConfig detect config, see also `MNNFaceDetectConfig` enum
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image. The face would be in the upright orientation after the rotation.
/// @param outAngle output angle, the coordinate of raw output feature points will rotate outAngle degree in in the image coordinate system. Generally in order to reach the same direction with the rendering coordinate system, then feature points will be easily rendered.
/// @param flipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message,  `nil` if the inference is successful
- (NSArray<MNNFaceDetectionReport *> *)inferenceWithImage:(UIImage*)image config:(MNNFaceDetectConfig)detectConfig
                                                    angle:(float)inAngle outAngle:(float)outAngle flipType:(MNNFlipType)flipType
                                                    error:(NSError *__autoreleasing *)error;

/// Detects faces in the common data format input.
/// @param data input data, int unsigned char format
/// @param w data width
/// @param h data height
/// @param format data format
/// @param detectConfig detect config, see also MNNFaceDetectConfig enum
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image. The face would be in the upright orientation after the rotation.
/// @param outAngle output angle, the coordinate of raw output feature points will rotate outAngle degree in in the image coordinate system. Generally in order to reach the same direction with the rendering coordinate system, then feature points will be easily rendered.
/// @param flipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message,  `nil` if the inference is successful.
- (NSArray<MNNFaceDetectionReport *> *)inferenceWithData:(unsigned char*)data width:(float)w height:(float)h format:(MNNCVImageFormat)format
                                                  config:(MNNFaceDetectConfig)detectConfig
                                                   angle:(float)inAngle outAngle:(float)outAngle flipType:(MNNFlipType)flipType
                                                   error:(NSError *__autoreleasing *)error;

@end

