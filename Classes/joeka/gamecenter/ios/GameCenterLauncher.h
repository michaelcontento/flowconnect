@interface GameCenterLauncher : NSObject

+(void)login;

+(void)openAchievement;
+(void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete;

+(void)openLeaderboard;
+(void)postScore:(const char*)idName score:(NSNumber*)score;

+(void)showNotAuthenticatedDialog;

@end
