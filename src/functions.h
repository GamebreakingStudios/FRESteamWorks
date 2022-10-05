/* automatically generated by generateAPI.rb */
/* START GENERATED CODE */
/********************************/
/* General Steamworks functions */
/********************************/
X(AIRSteam_Init) /* = 0 */
X(AIRSteam_RunCallbacks) /* = 1 */
X(AIRSteam_GetUserID) /* = 2 */
X(AIRSteam_GetAppID) /* = 3 */
X(AIRSteam_GetAvailableGameLanguages) /* = 4 */
X(AIRSteam_GetCurrentGameLanguage) /* = 5 */
X(AIRSteam_GetPersonaName) /* = 6 */
X(AIRSteam_RestartAppIfNecessary) /* = 7 */
X(AIRSteam_GetIPCountry) /* = 8 */
X(AIRSteam_IsSteamInBigPictureMode) /* = 9 */
/************************/
/* Stats / Achievements */
/************************/
X(AIRSteam_RequestStats) /* = 10 */
X(AIRSteam_SetAchievement) /* = 11 */
X(AIRSteam_ClearAchievement) /* = 12 */
X(AIRSteam_IsAchievement) /* = 13 */
X(AIRSteam_IsAchievementEarned) /* = 14 */
X(AIRSteam_GetAchievementAchievedPercent) /* = 15 */
X(AIRSteam_GetAchievementDisplayAttribute) /* = 16 */
X(AIRSteam_GetAchievementIcon) /* = 17 */
X(AIRSteam_GetAchievementName) /* = 18 */
X(AIRSteam_GetNumAchievements) /* = 19 */
X(AIRSteam_IndicateAchievementProgress) /* = 20 */
X(AIRSteam_GetStatInt) /* = 21 */
X(AIRSteam_GetStatFloat) /* = 22 */
X(AIRSteam_SetStatInt) /* = 23 */
X(AIRSteam_SetStatFloat) /* = 24 */
X(AIRSteam_StoreStats) /* = 25 */
X(AIRSteam_ResetAllStats) /* = 26 */
X(AIRSteam_RequestGlobalStats) /* = 27 */
X(AIRSteam_GetGlobalStatInt) /* = 28 */
X(AIRSteam_GetGlobalStatFloat) /* = 29 */
X(AIRSteam_GetGlobalStatHistoryInt) /* = 30 */
X(AIRSteam_GetGlobalStatHistoryFloat) /* = 31 */
/****************/
/* Leaderboards */
/****************/
X(AIRSteam_FindLeaderboard) /* = 32 */
X(AIRSteam_FindOrCreateLeaderboard) /* = 33 */
X(AIRSteam_FindLeaderboardResult) /* = 34 */
X(AIRSteam_GetLeaderboardName) /* = 35 */
X(AIRSteam_GetLeaderboardEntryCount) /* = 36 */
X(AIRSteam_GetLeaderboardSortMethod) /* = 37 */
X(AIRSteam_GetLeaderboardDisplayType) /* = 38 */
X(AIRSteam_UploadLeaderboardScore) /* = 39 */
X(AIRSteam_UploadLeaderboardScoreResult) /* = 40 */
X(AIRSteam_DownloadLeaderboardEntries) /* = 41 */
X(AIRSteam_DownloadLeaderboardEntriesResult) /* = 42 */
/**************************/
/* Cloud / Remote Storage */
/**************************/
X(AIRSteam_GetFileCount) /* = 43 */
X(AIRSteam_GetFileSize) /* = 44 */
X(AIRSteam_FileExists) /* = 45 */
X(AIRSteam_FileWrite) /* = 46 */
X(AIRSteam_FileRead) /* = 47 */
X(AIRSteam_FileDelete) /* = 48 */
X(AIRSteam_FileShare) /* = 49 */
X(AIRSteam_FileShareResult) /* = 50 */
X(AIRSteam_IsCloudEnabledForApp) /* = 51 */
X(AIRSteam_SetCloudEnabledForApp) /* = 52 */
X(AIRSteam_GetQuota) /* = 53 */
/******************/
/* UGC / Workshop */
/******************/
X(AIRSteam_UGCDownload) /* = 54 */
X(AIRSteam_UGCRead) /* = 55 */
X(AIRSteam_GetUGCDownloadProgress) /* = 56 */
X(AIRSteam_GetUGCDownloadResult) /* = 57 */
X(AIRSteam_PublishWorkshopFile) /* = 58 */
X(AIRSteam_PublishWorkshopFileResult) /* = 59 */
X(AIRSteam_DeletePublishedFile) /* = 60 */
X(AIRSteam_GetPublishedFileDetails) /* = 61 */
X(AIRSteam_GetPublishedFileDetailsResult) /* = 62 */
X(AIRSteam_EnumerateUserPublishedFiles) /* = 63 */
X(AIRSteam_EnumerateUserPublishedFilesResult) /* = 64 */
X(AIRSteam_EnumeratePublishedWorkshopFiles) /* = 65 */
X(AIRSteam_EnumeratePublishedWorkshopFilesResult) /* = 66 */
X(AIRSteam_EnumerateUserSubscribedFiles) /* = 67 */
X(AIRSteam_EnumerateUserSubscribedFilesResult) /* = 68 */
X(AIRSteam_EnumerateUserSharedWorkshopFiles) /* = 69 */
X(AIRSteam_EnumerateUserSharedWorkshopFilesResult) /* = 70 */
X(AIRSteam_EnumeratePublishedFilesByUserAction) /* = 71 */
X(AIRSteam_EnumeratePublishedFilesByUserActionResult) /* = 72 */
X(AIRSteam_SubscribePublishedFile) /* = 73 */
X(AIRSteam_UnsubscribePublishedFile) /* = 74 */
X(AIRSteam_CreatePublishedFileUpdateRequest) /* = 75 */
X(AIRSteam_UpdatePublishedFileFile) /* = 76 */
X(AIRSteam_UpdatePublishedFilePreviewFile) /* = 77 */
X(AIRSteam_UpdatePublishedFileTitle) /* = 78 */
X(AIRSteam_UpdatePublishedFileDescription) /* = 79 */
X(AIRSteam_UpdatePublishedFileSetChangeDescription) /* = 80 */
X(AIRSteam_UpdatePublishedFileVisibility) /* = 81 */
X(AIRSteam_UpdatePublishedFileTags) /* = 82 */
X(AIRSteam_CommitPublishedFileUpdate) /* = 83 */
X(AIRSteam_GetPublishedItemVoteDetails) /* = 84 */
X(AIRSteam_GetPublishedItemVoteDetailsResult) /* = 85 */
X(AIRSteam_GetUserPublishedItemVoteDetails) /* = 86 */
X(AIRSteam_GetUserPublishedItemVoteDetailsResult) /* = 87 */
X(AIRSteam_UpdateUserPublishedItemVote) /* = 88 */
X(AIRSteam_SetUserPublishedFileAction) /* = 89 */
/***********/
/* Friends */
/***********/
X(AIRSteam_GetFriendCount) /* = 90 */
X(AIRSteam_GetFriendByIndex) /* = 91 */
X(AIRSteam_GetFriendPersonaName) /* = 92 */
X(AIRSteam_GetSmallFriendAvatar) /* = 93 */
X(AIRSteam_GetMediumFriendAvatar) /* = 94 */
X(AIRSteam_GetLargeFriendAvatar) /* = 95 */
X(AIRSteam_SetRichPresence) /* = 96 */
X(AIRSteam_ClearRichPresence) /* = 97 */
/******************************/
/* Authentication & Ownership */
/******************************/
X(AIRSteam_GetAuthSessionTicket) /* = 98 */
X(AIRSteam_GetAuthSessionTicketResult) /* = 99 */
X(AIRSteam_BeginAuthSession) /* = 100 */
X(AIRSteam_EndAuthSession) /* = 101 */
X(AIRSteam_CancelAuthTicket) /* = 102 */
X(AIRSteam_UserHasLicenseForApp) /* = 103 */
X(AIRSteam_RequestEncryptedAppTicket) /* = 104 */
X(AIRSteam_GetEncryptedAppTicket) /* = 105 */
/***********/
/* Overlay */
/***********/
X(AIRSteam_ActivateGameOverlay) /* = 106 */
X(AIRSteam_ActivateGameOverlayToUser) /* = 107 */
X(AIRSteam_ActivateGameOverlayToWebPage) /* = 108 */
X(AIRSteam_ActivateGameOverlayToStore) /* = 109 */
X(AIRSteam_ActivateGameOverlayInviteDialog) /* = 110 */
X(AIRSteam_IsOverlayEnabled) /* = 111 */
X(AIRSteam_SetOverlayNotificationPosition) /* = 112 */
X(AIRSteam_SetOverlayNotificationInset) /* = 113 */
X(AIRSteam_OverlayNeedsPresent) /* = 114 */
/***********************/
/* DLC / subscriptions */
/***********************/
X(AIRSteam_IsSubscribedApp) /* = 115 */
X(AIRSteam_IsDLCInstalled) /* = 116 */
X(AIRSteam_GetDLCCount) /* = 117 */
X(AIRSteam_InstallDLC) /* = 118 */
X(AIRSteam_UninstallDLC) /* = 119 */
X(AIRSteam_DLCInstalledResult) /* = 120 */
/********************/
/* Microtransaction */
/********************/
X(AIRSteam_MicroTxnResult) /* = 121 */
/**********************************/
/* Other non-Steamworks functions */
/**********************************/
X(AIRSteam_GetEnv) /* = 122 */
X(AIRSteam_SetEnv) /* = 123 */
/********************/
/* Input */
/********************/
X(AIRSteam_InputInit)                       /* = 124 */
X(AIRSteam_GetControllerForGamepadIndex)    /* = 125 */
X(AIRSteam_ShowBindingPanel)                /* = 126 */
X(AIRSteam_GetActionSetHandle)              /* = 127 */
X(AIRSteam_GetDigitalActionHandle)          /* = 128 */
X(AIRSteam_GetAnalogActionHandle)           /* = 129 */
X(AIRSteam_GetDigitalActionData)            /* = 130 */
X(AIRSteam_GetAnalogActionData)             /* = 131 */
X(AIRSteam_RunFrame)                        /* = 132 */
X(AIRSteam_GetConnectedControllers)         /* = 133 */
X(AIRSteam_ActivateActionSet)               /* = 134 */
X(AIRSteam_GetHandleAllControllers)         /* = 135 */
X(AIRSteam_GetDigitalActionOrigins)         /* = 136 */
X(AIRSteam_GetAnalogActionOrigins)          /* = 137 */
X(AIRSteam_GetGlyphSVGForActionOrigin)      /* = 138 */
X(AIRSteam_GetGlyphPNGForActionOrigin)      /* = 139 */
X(AIRSteam_GetStringForActionOrigin)        /* = 140 */
X(AIRSteam_ShowGamepadTextInput)            /* = 141 */
X(AIRSteam_ShowFloatingGamepadTextInput)    /* = 142 */
X(AIRSteam_SteamInputShutDown)              /* = 143 */
X(AIRSteam_DismissFloatingGamepadTextInput) /* = 144 */
/* END GENERATED CODE */
