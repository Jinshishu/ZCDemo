//
//  CoreCommon.h
//  MNN
//
//  Created by MNN on 2019/12/4.
//  Copyright © 2019, Alibaba Group Holding Limited
//

#import <Foundation/Foundation.h>
#import "CommonDef.h"
#import <CoreVideo/CoreVideo.h>

NS_ASSUME_NONNULL_BEGIN

@interface CoreCommon : NSObject

unsigned char* MNNPixelBufferRefPaser(CVPixelBufferRef pixelBufferRef, int *w, int *h, MNNCVImageFormat *format, int *image_stride);

@end

NS_ASSUME_NONNULL_END
