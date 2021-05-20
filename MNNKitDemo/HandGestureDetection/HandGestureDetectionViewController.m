//
//  HandGestureDetectionViewController.m
//  MNNKitDemo
//
//  Created by tsia on 2019/12/25.
//  Copyright © 2019 tsia. All rights reserved.
//

#import "HandGestureDetectionViewController.h"
#import <AVFoundation/AVFoundation.h>
#import <CoreMotion/CoreMotion.h>
#import <MNNHandGestureDetection/MNNHandGestureDetector.h>
#import "HandGestureDetectView.h"
#import "HandGestureDetectionImageViewController.h"
#import "libyuv.h"
#import "aw_alloc.h"

#define ScreenWidth                         [[UIScreen mainScreen] bounds].size.width
#define ScreenHeight                        [[UIScreen mainScreen] bounds].size.height

@interface HandGestureDetectionViewController ()

@property (strong, nonatomic) UILabel *lbCostTime;

@property (strong, nonatomic) MNNHandGestureDetector *handGestureDetector;

@end

@implementation HandGestureDetectionViewController

#pragma mark - life cycle

- (void)viewDidLoad {
    [super viewDidLoad];
    
    // 耗时ms
    _lbCostTime = [[UILabel alloc]initWithFrame:CGRectMake(10, 74, 100, 40)];
    _lbCostTime.textColor = [UIColor redColor];
    _lbCostTime.textColor = [UIColor greenColor];
    [self.view addSubview:_lbCostTime];
}

#pragma mark - mnn hand gesture
- (void)createKitInstance {
    if (self.handGestureDetector) {
        self.handGestureDetector = nil;
    }
    
    MNNHandGestureDetectorCreateConfig *config = [[MNNHandGestureDetectorCreateConfig alloc] init];
    config.detectMode = MNN_HAND_DETECT_MODE_VIDEO;
    [MNNHandGestureDetector createInstanceAsync:config callback:^(NSError *error, MNNHandGestureDetector *handgestureDetector) {
        
        self.handGestureDetector = handgestureDetector;
    }];
}

#pragma mark - ui
- (VideoBaseDetectView*)createDetectView {
    HandGestureDetectView *detectView = [[HandGestureDetectView alloc] init];
    return detectView;
}

#pragma mark - action
- (void)onImageMode:(id)sender {
    [self.navigationController pushViewController:[HandGestureDetectionImageViewController new] animated:YES];
}

#pragma mark - AVCaptureVideoDataOutputSampleBufferDelegate
- (void)captureOutput:(AVCaptureOutput *)output didOutputSampleBuffer:(nonnull CMSampleBufferRef)sampleBuffer fromConnection:(nonnull AVCaptureConnection *)connection {
    if (!self.handGestureDetector) {
        return;
    }
    
    NSDictionary *angleDic = [self calculateInAndOutAngle];
    float inAngle = [angleDic[@"inAngle"] floatValue];
    float outAngle = [angleDic[@"outAngle"] floatValue];
    
//    NSLog(@"inAngle = %.2f , outAngle = %.2f \n sampleBuffer = %@",inAngle,outAngle,sampleBuffer);
    NSError *error = nil;
    NSTimeInterval startTime = [[NSDate date] timeIntervalSince1970];
    NSArray<MNNHandGestureDetectionReport *> *detectResult = [self.handGestureDetector inferenceWithPixelBuffer:CMSampleBufferGetImageBuffer(sampleBuffer) angle:inAngle outAngle:outAngle flipType:FLIP_NONE error:&error];
    NSLog(@"\n--detectResult = %@",detectResult);
    NSTimeInterval timeElapsed = [[NSDate date] timeIntervalSince1970] - startTime;
    
    if (error) {
        NSLog(@"%@", error.localizedDescription);
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        if (detectResult!=nil && detectResult.count>0) {
            self->_lbCostTime.text = [NSString stringWithFormat:@"%.2fms", timeElapsed*1000];
        } else {
            self->_lbCostTime.text = @"0.00ms";
        }
    });
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        if ([self.detectView isKindOfClass:NSClassFromString(@"HandGestureDetectView")]) {
            HandGestureDetectView *handDetectView = (HandGestureDetectView*)self.detectView;
            handDetectView.detectResult = detectResult;
        }
    });
    
}

-(void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
{
    if (!self.handGestureDetector) {
        return;
    }
    
    NSDictionary *angleDic = [self calculateInAndOutAngle];
    float inAngle = [angleDic[@"inAngle"] floatValue];
    float outAngle = [angleDic[@"outAngle"] floatValue];
    
//    NSLog(@"inAngle = %.2f , outAngle = %.2f \n sampleBuffer = %@",inAngle,outAngle,sampleBuffer);
    
    NSError *error = nil;
    NSTimeInterval startTime = [[NSDate date] timeIntervalSince1970];
    NSArray<MNNHandGestureDetectionReport *> *detectResult = [self.handGestureDetector inferenceWithPixelBuffer:[self convertVideoSmapleBufferToBGRAData:sampleBuffer] angle:inAngle outAngle:outAngle flipType:FLIP_NONE error:&error];
    
    NSTimeInterval timeElapsed = [[NSDate date] timeIntervalSince1970] - startTime;
    
    if (error) {
        NSLog(@"%@", error.localizedDescription);
        return;
    }
    
    dispatch_async(dispatch_get_main_queue(), ^{
        if (detectResult!=nil && detectResult.count>0) {
            self->_lbCostTime.text = [NSString stringWithFormat:@"%.2fms", timeElapsed*1000];
        } else {
            self->_lbCostTime.text = @"0.00ms";
        }
    });
    
    dispatch_async(dispatch_get_main_queue(), ^{
        
        if ([self.detectView isKindOfClass:NSClassFromString(@"HandGestureDetectView")]) {
            HandGestureDetectView *handDetectView = (HandGestureDetectView*)self.detectView;
            handDetectView.detectResult = detectResult;
        }
    });
    
}

-(CVPixelBufferRef)convertVideoSmapleBufferToBGRAData:(CMSampleBufferRef)videoSample{
    
    //CVPixelBufferRef是CVImageBufferRef的别名，两者操作几乎一致。
    //获取CMSampleBuffer的图像地址
    CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(videoSample);
  //VideoToolbox解码后的图像数据并不能直接给CPU访问，需先用CVPixelBufferLockBaseAddress()锁定地址才能从主存访问，否则调用CVPixelBufferGetBaseAddressOfPlane等函数则返回NULL或无效值。值得注意的是，CVPixelBufferLockBaseAddress自身的调用并不消耗多少性能，一般情况，锁定之后，往CVPixelBuffer拷贝内存才是相对耗时的操作，比如计算内存偏移。
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    //图像宽度（像素）
    size_t pixelWidth = CVPixelBufferGetWidth(pixelBuffer);
    //图像高度（像素）
    size_t pixelHeight = CVPixelBufferGetHeight(pixelBuffer);
    //获取CVImageBufferRef中的y数据
    uint8_t *y_frame = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    //获取CMVImageBufferRef中的uv数据
    uint8_t *uv_frame =(unsigned char *) CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    
    
    // 创建一个空的32BGRA格式的CVPixelBufferRef
    NSDictionary *pixelAttributes = @{(id)kCVPixelBufferIOSurfacePropertiesKey : @{}};
    CVPixelBufferRef pixelBuffer1 = NULL;
    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
                                          pixelWidth,pixelHeight,kCVPixelFormatType_32BGRA,
                                          (__bridge CFDictionaryRef)pixelAttributes,&pixelBuffer1);
    if (result != kCVReturnSuccess) {
        NSLog(@"Unable to create cvpixelbuffer %d", result);
        return NULL;
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    
    result = CVPixelBufferLockBaseAddress(pixelBuffer1, 0);
    if (result != kCVReturnSuccess) {
        CFRelease(pixelBuffer1);
        NSLog(@"Failed to lock base address: %d", result);
        return NULL;
    }
    
    // 得到新创建的CVPixelBufferRef中 rgb数据的首地址
    uint8_t *rgb_data = (uint8*)CVPixelBufferGetBaseAddress(pixelBuffer1);
    
    // 使用libyuv为rgb_data写入数据，将NV12转换为BGRA
    int ret = NV12ToARGB(y_frame, pixelWidth, uv_frame, pixelWidth, rgb_data, pixelWidth * 4, pixelWidth, pixelHeight);
    if (ret) {
        NSLog(@"Error converting NV12 VideoFrame to BGRA: %d", result);
        CFRelease(pixelBuffer1);
        return NULL;
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer1, 0);
    
    return pixelBuffer1;
}

@end
