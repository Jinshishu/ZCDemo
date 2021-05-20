//
//  MNNMonitor.h
//  MNNKitCore
//
//  Created by MNN on 2019/12/19.
//  Copyright © 2019, Alibaba Group Holding Limited
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MNNMonitor : NSObject


/// enable/disable collection of  statistical information. Enable by default.
/// @param enable true or false
+ (void)setMonitorEnable:(BOOL)enable;

+ (void)loadCommitWithBizName:(NSString*)bizName
                    packageId:(NSString*)packageId
                    modelName:(NSString*)modelName
                      success:(BOOL)isSuc
                     timeCost:(long)timeCost
                   sdkVersion:(NSMutableDictionary*)sdkVersion;

+ (void)inferenceCommitWithBizName:(NSString*)bizName
                         packageId:(NSString*)packageId
                         modelName:(NSString*)modelName
                           success:(BOOL)isSuc
                          timeCost:(long)timeCost
                        sdkVersion:(NSMutableDictionary*)sdkVersion;

@end

NS_ASSUME_NONNULL_END
