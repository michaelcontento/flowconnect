@interface NativeCodeLauncher : NSObject

// GameCenter
+(void)loginGameCenter;
+(void)openRanking;
+(void)postHighScore:(const char *)idName score:(NSNumber *)score;
+(void)openAchievement;
+(void)postAchievement:(const char *)idName percent:(NSNumber *)percentComplete;

@end
