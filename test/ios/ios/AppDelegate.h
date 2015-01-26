//
//  AppDelegate.h
//  ios
//
//  Created by Saint Hsu on 23/1/15.
//  Copyright (c) 2015 Saint Hsu. All rights reserved.
//

#import <UIKit/UIKit.h>

@class GameViewController;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property(nonatomic, readonly) GameViewController* viewController;

@end

