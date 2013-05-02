@interface GameCenterLauncher : NSObject {
}

+(GameCenterLauncher*)shared;

-(void)login;

-(void)openAchievement;
-(void)postAchievement:(const char*)idName percent:(NSNumber*)percentComplete;
-(void)clearAllAchivements;

-(void)openLeaderboard;
-(void)postScore:(const char*)idName score:(NSNumber*)score;
-(void)clearAllScores;

@end
