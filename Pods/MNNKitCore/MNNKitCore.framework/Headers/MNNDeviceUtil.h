//
//  MNNDeviceUtil.h
//  MNNKitCore
//
//  Created by MNN on 2019/12/23.
//  Copyright © 2019, Alibaba Group Holding Limited
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MNNDeviceUtil : NSObject

+ (NSString *)platform;

+ (NSString*)cpuType;

+ (NSString*)getAppName;
    
@end

NS_ASSUME_NONNULL_END
