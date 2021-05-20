//
//  MNNPortraitSegmentor.h
//  MNNPortraitSegment
//
//  Created by MNN on 2019/12/26.
//  Copyright © 2019, Alibaba Group Holding Limited
//

#import <UIKit/UIKit.h>

#import <MNNKitCore/CoreCommon.h>
#import <MNNKitCore/CommonDef.h>

static NSString *MNNKitErrorDomain = @"MNNKitErrorDomain";


/// Portrait Segmentor Class. Segments a portrait from an input frame. See [here] (https://github.com/alibaba/MNNKit/blob/master/doc/PortraitSegmentation_CN.md) for the detailed usage.

@interface MNNPortraitSegmentor : NSObject

/// Creates a portrait segmentor instance asynchronously. The instance is passed in the callback in the main thread.
/// @param block call back after created, error is `nil` if the call back is successful.
+ (void)createInstanceAsync:(void(^)(NSError *error, MNNPortraitSegmentor *portraitSegmentor))block;

/// Creates a portrait segmentor instance asynchronously. The instance is passed in the callback in the designated thread specified by `callbackQueue`.
/// @param block call back after created, error is `nil` if the call back is successful.
/// @param callbackQueue call back in this designated thread, in main thread if `nil`.
+ (void)createInstanceAsync:(void(^)(NSError *error, MNNPortraitSegmentor *portraitSegmentor))block callbackQueue:(dispatch_queue_t)callbackQueue;


/// Segments the portrait from the system camera input.
/// @param pixelBuffer input data in CVPixelBufferRef format
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image. The portrait would be in the upright orientation after the rotation.
/// @param outputFlipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message,  `nil` if the inference is successful
- (NSArray<NSNumber*> *)inferenceWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                                           angle:(float)inAngle flipType:(MNNFlipType)outputFlipType
                                           error:(NSError *__autoreleasing *)error;

/// Segments the portrait from the input image.
/// @param image image in UIImage format
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image. The portrait would be in the upright orientation after rotated.
/// @param outputFlipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message, `nil` if the inference is successful
- (NSArray<NSNumber*> *)inferenceWithImage:(UIImage*)image
                                     angle:(float)inAngle
                                  flipType:(MNNFlipType)outputFlipType
                                     error:(NSError *__autoreleasing *)error;

/// Segments the portrait from common data format input.
/// @param data input data, int unsigned char format
/// @param w data width
/// @param h data height
/// @param format data format
/// @param inAngle input angle, the clock-wise rotation angle applied on the input image. The portrait would be in the upright orientation after the rotation.
/// @param outputFlipType mirror type applied on the result feature points: NONE (FLIP_NONE), X-axis flipping (FLIP_X), Y-axis flipping (FLIP_Y), Center flipping (FLIP_XY)
/// @param error error message,  `nil` if the inference is successful
- (NSArray<NSNumber*> *)inferenceWithData:(unsigned char*)data width:(float)w height:(float)h format:(MNNCVImageFormat)format
                                    angle:(float)inAngle flipType:(MNNFlipType)outputFlipType
                                    error:(NSError *__autoreleasing *)error;


@end


