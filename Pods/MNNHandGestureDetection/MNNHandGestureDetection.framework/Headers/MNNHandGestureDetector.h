//
//  MNNHandGestureDetector.h
//  MNNHandGestureDetection
//
//  Created by MNN 2019/12/25.
//  Copyright © 2019, Alibaba Group Holding Limited
//

#import <UIKit/UIKit.h>

#import <MNNKitCore/CoreCommon.h>
#import <MNNKitCore/MNNMonitor.h>

static NSString *MNNKitErrorDomain = @"MNNKitErrorDomain";

/// Hand gesture detect mode. In `MNN_HAND_DETECT_MODE_VIDEO`, the detection is run by default every 20 frames while the rest of the frames are only used for tracking. In `MNN_HAND_DETECT_MODE_IMAGE`, each frame will trigger the detection.
typedef NS_ENUM(NSUInteger, MNNHandDetectMode) {
    MNN_HAND_DETECT_MODE_VIDEO = 0,///< video detect mode
    MNN_HAND_DETECT_MODE_IMAGE = 1,///< image detect mode
};

/// Configuration used to create the hand gesture detector.
@interface MNNHandGestureDetectorCreateConfig : NSObject
/// detect mode, video or image
@property (nonatomic, assign) MNNHandDetectMode detectMode;
@end

/// Hand gesture result type. See [here] (https://github.com/alibaba/MNNKit/blob/master/doc/hand_gesture.png) for an illustration.
typedef NS_ENUM(NSUInteger, MNNHandGestureType) {
    MNN_HAND_GESTURE_TYPE_FINGER_HEART = 0, ///< finger heart
    MNN_HAND_GESTURE_TYPE_HAND_OPEN = 1,    ///< hand open
    MNN_HAND_GESTURE_TYPE_INDEX_FINGER = 2, ///< index finger
    MNN_HAND_GESTURE_TYPE_FIST = 3,         ///< fist
    MNN_HAND_GESTURE_TYPE_THUMB_UP = 4,     ///< thumb up
    MNN_HAND_GESTURE_TYPE_OTHER = 5,        ///< other
};


/// Hand gesture detection result.
@interface MNNHandGestureDetectionReport : NSObject

/// Hand gesture result type.
@property (nonatomic, assign) MNNHandGestureType type;

/// Confidence score for the detected gesture.
@property (nonatomic, assign) float score;

/// Aunique identification for the hand. A new handID will be generated if hand track is lost but detected successfully again.
@property (nonatomic, assign) int handID;

/// Bounding box for the hand.
@property (nonatomic, assign) CGRect rect;

@end


/// Hand Gesture Detector Class. See [here](https://github.com/alibaba/MNNKit/blob/master/doc/HandGestureDetection_CN.md) for detailed usage.
@interface MNNHandGestureDetector : NSObject

/// Creates a hand gesture detector instance asynchronously. The instance is passed in the callback in the main thread.
/// @param config  config parameter for the creation, such as video detection or image detection
/// @param block call back after creation, error is `nil` if the call back is successful.
+ (void)createInstanceAsync:(MNNHandGestureDetectorCreateConfig*)config callback:(void(^)(NSError *error, MNNHandGestureDetector *handgestureDetector))block;

/// Creates a hand gesture detector instance asynchronously. The instance is passed in the callback in the designated thread specified by `callbackQueue`.
/// @param config config parameter for the creation, such as video detection or image detection
/// @param block call back after creation, error is `nil` if the call back is successful.
/// @param callbackQueue call back in this designated thread, in main thread if `nil`.
+ (void)createInstanceAsync:(MNNHandGestureDetectorCreateConfig*)config callback:(void(^)(NSError *error, MNNHandGestureDetector *handgestureDetector))block callbackQueue:(dispatch_queue_t)callbackQueue;

/// Detects a hand gesture from the system camera input.
/// @param pixelBuffer input data in CVPixelBufferRef format
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image, the hand would be in the upright orientation after rotation.
/// @param outAngle output angle, the coordinate of raw output feature points will rotate `outAngle` degree in the image coordinate system. Generally in order to reach the same direction with the rendering coordinate system, then feature points will be easily rendered.
/// @param flipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message, `nil` if the inference is successful
- (NSArray<MNNHandGestureDetectionReport *> *)inferenceWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                                                                 angle:(float)inAngle
                                                              outAngle:(float)outAngle
                                                              flipType:(MNNFlipType)flipType
                                                                 error:(NSError *__autoreleasing *)error;

/// Detects a hand gesture from the input image.
/// @param image image in UIImage format
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image, the hand would be in the upright orientation after rotation.
/// @param outAngle output angle, the coordinate of raw output feature points will rotate `outAngle` degree in in the image coordinate system. Generally in order to reach the same direction with the rendering coordinate system, then feature points will be easily rendered.
/// @param flipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message, `nil` if the inference is successful
- (NSArray<MNNHandGestureDetectionReport *> *)inferenceWithImage:(UIImage*)image
                                                           angle:(float)inAngle outAngle:(float)outAngle
                                                        flipType:(MNNFlipType)flipType
                                                           error:(NSError *__autoreleasing *)error;

/// Detects a hand gesture from the common data format input.
/// @param data input data, in unsigned char format
/// @param w data width
/// @param h data height
/// @param format data format
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image, the hand would be in the upright orientation after rotated.
/// @param outAngle output angle, the coordinate of raw output feature points will rotate `outAngle` degree in in the image coordinate system. Generally in order to reach the same direction with the rendering coordinate system, then feature points will be easily rendered.
/// @param flipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error  error message, `nil` if the inference is successful
- (NSArray<MNNHandGestureDetectionReport *> *)inferenceWithData:(unsigned char*)data width:(float)w
                                                         height:(float)h format:(MNNCVImageFormat)format
                                                          angle:(float)inAngle outAngle:(float)outAngle
                                                       flipType:(MNNFlipType)flipType
                                                          error:(NSError *__autoreleasing *)error;

@end
