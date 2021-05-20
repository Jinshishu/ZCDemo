//
//  CommonDef.h
//  MNNKitCore
//
//  Created by MNN on 2019/12/3.
//  Copyright © 2019, Alibaba Group Holding Limited
//


#import <Foundation/Foundation.h>

#ifndef CommonDef_h
#define CommonDef_h


typedef NS_ENUM(NSUInteger, MNNCVImageFormat) {
    
    RGBA = 0,
    RGB = 1,
    BGR = 2,
    GRAY = 3,
    BGRA = 4,
    YUV_NV21 = 11,
};

typedef NS_ENUM(NSUInteger, MNNFlipType) {
    
    FLIP_NONE = 0,
    FLIP_X = 1,
    FLIP_Y = 2,
    FLIP_XY = 3,
};


#endif /* CommonDef_h */
