/*
 *  FRESteamWorks.cpp
 *  This file is part of FRESteamWorks.
 *
 *  Created by David ´Oldes´ Oliva on 3/29/12.
 *  Contributors: Ventero <http://github.com/Ventero>
 *  Copyright (c) 2012 Amanita Design. All rights reserved.
 *  Copyright (c) 2012-2013 Level Up Labs, LLC. All rights reserved.
 */

#include "FRESteamWorks.h"

#include <cstdlib>

#include "FREConverters.h"

// Used to dispatch event back to AIR
FREContext AIRContext;

// Global access to Steam object
ANESteam* g_Steam = NULL;

#ifdef DEBUG
#include <iostream>
void steamWarningMessageHook(int severity, const char* msg) {
	std::cerr << "Severity " << severity << ": " << msg << std::endl;
}
#else
void steamWarningMessageHook(int severity, const char* msg) {
	// ignore silently
}
#endif

void ANESteam::DispatchEvent(char* code, char* level) {
	// ignore unsuccessful dispatches
	FREDispatchStatusEventAsync(AIRContext, (const uint8_t*)code, (const uint8_t*)level);
}

// Utility macros
#define ARG_CHECK(arg_num, default) \
	if (!g_Steam || argc != arg_num) return default

#define SET_PROP(object, property, value) \
	FRESetObjectProperty(object, (const uint8_t*)property, value, NULL)

#define AIR_FUNC(name) \
	FREObject name(FREContext ctx, void* funcData, uint32 argc, FREObject argv[])


/*
 * prototype declarations
 */
#define X(a) AIR_FUNC(a);
#include "functions.h"
#undef X

/*
 * general functions
 */

AIR_FUNC(AIRSteam_Init) {
	// check if already initialized
	if (g_Steam) return FREBool(true);

	g_Steam = new ANESteam();
	if (!g_Steam->Initialize()) {
		delete g_Steam;
		g_Steam = nullptr;
		return FREBool(false);
	}

#ifdef DEBUG
	g_Steam->SetWarningMessageHook(steamWarningMessageHook);
#endif

	return FREBool(true);
}

AIR_FUNC(AIRSteam_RunCallbacks) {
	SteamAPI_RunCallbacks();
	return FREBool(true);
}

AIR_FUNC(AIRSteam_GetUserID) {
	if (!g_Steam) return FREUint64(0);

	return FREUint64(g_Steam->GetUserID().ConvertToUint64());
}

AIR_FUNC(AIRSteam_GetAppID) {
	if (!g_Steam) return FREUint(0);

	return FREUint(g_Steam->GetAppID());
}

AIR_FUNC(AIRSteam_GetAvailableGameLanguages) {
	if (!g_Steam) return FREString("");

	return FREString(g_Steam->GetAvailableGameLanguages());
}

AIR_FUNC(AIRSteam_GetCurrentGameLanguage) {
	if (!g_Steam) return FREString("");

	return FREString(g_Steam->GetCurrentGameLanguage());
}

AIR_FUNC(AIRSteam_GetPersonaName) {
	if (!g_Steam) return FREString("");

	return FREString(g_Steam->GetPersonaName());
}

AIR_FUNC(AIRSteam_GetIPCountry) {
	if (!g_Steam) return FREString("");

	return FREString(g_Steam->GetIPCountry());
}

AIR_FUNC(AIRSteam_IsSteamInBigPictureMode) {
	if (!g_Steam) return FREBool(false);

	return FREBool(g_Steam->IsSteamInBigPictureMode());
}

AIR_FUNC(AIRSteam_IsSteamRunningOnSteamDeck) {
	if (!g_Steam) return FREBool(false);

	return FREBool(g_Steam->IsSteamRunningOnSteamDeck());
}

AIR_FUNC(AIRSteam_GetEarliestPurchaseUnixTime) {
	if (!g_Steam) return FREUint(0);
	if (argc != 1) return FREUint(0);

	uint32 appID = 0;
	if (!FREGetUint32(argv[0], &appID)) return FREUint(0);

	return FREUint(g_Steam->GetEarliestPurchaseUnixTime(appID));
}

AIR_FUNC(AIRSteam_GetServerRealTime) {
	return FREUint(g_Steam->GetServerRealTime());
}

AIR_FUNC(AIRSteam_GetSecondsSinceAppActive) {
	return FREUint(g_Steam->GetSecondsSinceAppActive());
}

AIR_FUNC(AIRSteam_RestartAppIfNecessary) {
	if (argc != 1) return FREBool(false);
	uint32 appID = 0;
	if (!FREGetUint32(argv[0], &appID)) return FREBool(false);

	bool result = SteamAPI_RestartAppIfNecessary(appID);
	return FREBool(result);
}

/*
 * stats / achievements
 */

AIR_FUNC(AIRSteam_RequestStats) {
	bool ret = false;
	if (g_Steam) ret = g_Steam->RequestStats();

	SteamAPI_RunCallbacks();
	return FREBool(ret);
}

AIR_FUNC(AIRSteam_SetAchievement) {
	ARG_CHECK(1, FREBool(false));

	bool ret = false;
	std::string name;
	if (FREGetString(argv[0], name))
		ret = g_Steam->SetAchievement(name);

	SteamAPI_RunCallbacks();
	return FREBool(ret);
}

AIR_FUNC(AIRSteam_ClearAchievement) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->ClearAchievement(name));
}

AIR_FUNC(AIRSteam_IsAchievement) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->IsAchievement(name));
}

AIR_FUNC(AIRSteam_IsAchievementEarned) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->IsAchievementEarned(name));
}

AIR_FUNC(AIRSteam_GetAchievementAchievedPercent) {
	ARG_CHECK(1, FREDouble(0.0));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREDouble(0.0);

	float value = 0.0f;
	g_Steam->GetAchievementAchievedPercent(name, &value);
	return FREDouble(value);
}

AIR_FUNC(AIRSteam_GetAchievementDisplayAttribute) {
	ARG_CHECK(2, FREString(""));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREString("");

	std::string attribute;
	if (!FREGetString(argv[1], attribute)) return FREString("");

	return FREString(g_Steam->GetAchievementDisplayAttribute(name, attribute));
}

AIR_FUNC(AIRSteam_GetAchievementName) {
	ARG_CHECK(1, FREString(""));

	uint32 index;
	if (!FREGetUint32(argv[0], &index)) return FREString("");

	return FREString(g_Steam->GetAchievementName(index));
}

AIR_FUNC(AIRSteam_GetNumAchievements) {
	ARG_CHECK(0, 0);
	return FREInt(g_Steam->GetNumAchievements());
}


AIR_FUNC(AIRSteam_GetAchievementIcon) {
	ARG_CHECK(1, nullptr);

	std::string name;
	if (!FREGetString(argv[0], name)) return nullptr;

	Image image = g_Steam->GetAchievementIcon(name);
	if (image.data.size() == 0) return nullptr;
	return FREBitmapDataFromImageRGBA(image.width, image.height, image.argb_data());
}



AIR_FUNC(AIRSteam_IndicateAchievementProgress) {
	ARG_CHECK(3, FREBool(false));

	std::string name;
	uint32 current_progress = 0;
	uint32 max_progress = 0;

	if (!FREGetString(argv[0], name) ||
		!FREGetUint32(argv[1], &current_progress) ||
		!FREGetUint32(argv[2], &max_progress))
		return FREBool(false);

	return FREBool(g_Steam->IndicateAchievementProgress(name,
		current_progress, max_progress));
}



AIR_FUNC(AIRSteam_GetStatInt) {
	ARG_CHECK(1, FREInt(0));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREInt(0);

	int32 value = 0;
	g_Steam->GetStat(name, &value);
	return FREInt(value);
}

AIR_FUNC(AIRSteam_GetStatFloat) {
	ARG_CHECK(1, FREDouble(0.0));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREDouble(0.0);

	float value = 0.0f;
	g_Steam->GetStat(name, &value);
	return FREDouble(value);
}

AIR_FUNC(AIRSteam_SetStatInt) {
	ARG_CHECK(2, FREBool(false));

	std::string name;
	int32 value;
	if (!FREGetString(argv[0], name) ||
	    !FREGetInt32(argv[1], &value)) return FREBool(false);

	return FREBool(g_Steam->SetStat(name, value));
}

AIR_FUNC(AIRSteam_SetStatFloat) {
	ARG_CHECK(2, FREBool(false));

	std::string name;
	double value;
	if (!FREGetString(argv[0], name) ||
	    !FREGetDouble(argv[1], &value)) return FREBool(false);

	return FREBool(g_Steam->SetStat(name, (float)value));
}

AIR_FUNC(AIRSteam_StoreStats) {
	if (!g_Steam) return FREBool(false);

	return FREBool(g_Steam->StoreStats());
}

AIR_FUNC(AIRSteam_ResetAllStats) {
	ARG_CHECK(1, FREBool(false));

	bool achievementsToo;
	if (!FREGetBool(argv[0], &achievementsToo)) return FREBool(false);

	return FREBool(g_Steam->ResetAllStats(achievementsToo));
}

AIR_FUNC(AIRSteam_RequestGlobalStats) {
	ARG_CHECK(1, FREBool(false));

	int days;
	if (!FREGetInt32(argv[0], &days))
		return FREBool(false);

	return FREBool(g_Steam->RequestGlobalStats(days));
}

// Since Flash doesn't have support for 64bit integer types, we just always
// return a double (aka Number).
AIR_FUNC(AIRSteam_GetGlobalStatInt) {
	ARG_CHECK(1, FREDouble(0.0));

	std::string name;
	if (!FREGetString(argv[0], name))
		return FREDouble(0.0);

	int64 value = 0;
	g_Steam->GetGlobalStat(name, &value);

	return FREDouble(static_cast<double>(value));
}

AIR_FUNC(AIRSteam_GetGlobalStatFloat) {
	ARG_CHECK(1, FREDouble(0.0));

	std::string name;
	if (!FREGetString(argv[0], name))
		return FREDouble(0.0);

	double value = 0;
	g_Steam->GetGlobalStat(name, &value);

	return FREDouble(static_cast<double>(value));
}

// See above for why double is used.
AIR_FUNC(AIRSteam_GetGlobalStatHistoryInt) {
	ARG_CHECK(2, FREArray(0));

	std::string name;
	uint32 days;
	if (!FREGetString(argv[0], name) ||
		  !FREGetUint32(argv[1], &days))
		return FREArray(0);

	auto history = g_Steam->GetGlobalStatHistoryInt(name, days);
	uint32 size = static_cast<uint32>(history.size());

	FREObject array = FREArray(size);
	for (uint32 i = 0; i < size; ++i) {
		double value = static_cast<double>(history.at(i));
		FRESetArrayElementAt(array, i, FREDouble(value));
	}

	return array;
}

AIR_FUNC(AIRSteam_GetGlobalStatHistoryFloat) {
	ARG_CHECK(2, FREArray(0));

	std::string name;
	uint32 days;
	if (!FREGetString(argv[0], name) ||
		  !FREGetUint32(argv[1], &days))
		return FREArray(0);

	auto history = g_Steam->GetGlobalStatHistoryFloat(name, days);
	uint32 size = static_cast<uint32>(history.size());

	FREObject array = FREArray(size);
	for (uint32 i = 0; i < size; ++i) {
		double value = history.at(i);
		FRESetArrayElementAt(array, i, FREDouble(value));
	}

	return array;
}

/*
 * leaderboards
 */
AIR_FUNC(AIRSteam_FindLeaderboard) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->FindLeaderboard(name));
}

AIR_FUNC(AIRSteam_FindOrCreateLeaderboard) {
	ARG_CHECK(3, FREBool(false));

	std::string name;
	uint32 sort, display;
	if (!FREGetString(argv[0], name) ||
		  !FREGetUint32(argv[1], &sort) ||
		  !FREGetUint32(argv[2], &display)) return FREBool(false);

	return FREBool(g_Steam->FindOrCreateLeaderboard(name,
		ELeaderboardSortMethod(sort), ELeaderboardDisplayType(display)));
}

AIR_FUNC(AIRSteam_FindLeaderboardResult) {
	ARG_CHECK(0, FREUint64(0));

	return FREUint64(g_Steam->FindLeaderboardResult());
}

AIR_FUNC(AIRSteam_GetLeaderboardName) {
	ARG_CHECK(1, FREString(""));

	SteamLeaderboard_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREString("");

	return FREString(g_Steam->GetLeaderboardName(handle));
}

AIR_FUNC(AIRSteam_GetLeaderboardEntryCount) {
	ARG_CHECK(1, FREInt(0));

	SteamLeaderboard_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREInt(0);

	return FREInt(g_Steam->GetLeaderboardEntryCount(handle));
}

AIR_FUNC(AIRSteam_GetLeaderboardSortMethod) {
	ARG_CHECK(1, FREUint(0));

	SteamLeaderboard_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREInt(0);

	return FREUint(g_Steam->GetLeaderboardSortMethod(handle));
}

AIR_FUNC(AIRSteam_GetLeaderboardDisplayType) {
	ARG_CHECK(1, FREUint(0));

	SteamLeaderboard_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREInt(0);

	return FREUint(g_Steam->GetLeaderboardDisplayType(handle));
}

AIR_FUNC(AIRSteam_UploadLeaderboardScore) {
	ARG_CHECK(4, FREBool(false));

	SteamLeaderboard_t handle;
	uint32 method;
	int32 score;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetUint32(argv[1], &method) ||
	    !FREGetInt32(argv[2], &score)) return FREBool(false);

	std::vector<int32> details = getArray<int32>(argv[3], FREGetInt32);
	int num_details = static_cast<int>(details.size());

	return FREBool(g_Steam->UploadLeaderboardScore(handle,
		ELeaderboardUploadScoreMethod(method), score,
		details.data(), num_details));
}

AIR_FUNC(AIRSteam_UploadLeaderboardScoreResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.UploadLeaderboardScoreResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);

	auto details = g_Steam->UploadLeaderboardScoreResult();
	if (!details) return result;

	SET_PROP(result, "success",            FREBool(details->m_bSuccess != 0));
	SET_PROP(result, "leaderboardHandle",  FREUint64(details->m_hSteamLeaderboard));
	SET_PROP(result, "score",              FREInt(details->m_nScore));
	SET_PROP(result, "scoreChanged",       FREBool(details->m_bScoreChanged != 0));
	SET_PROP(result, "newGlobalRank",      FREInt(details->m_nGlobalRankNew));
	SET_PROP(result, "previousGlobalRank", FREInt(details->m_nGlobalRankPrevious));

	return result;
}

AIR_FUNC(AIRSteam_DownloadLeaderboardEntries) {
	ARG_CHECK(4, FREBool(false));

	SteamLeaderboard_t handle;
	uint32 request;
	int rangeStart, rangeEnd;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetUint32(argv[1], &request) ||
	    !FREGetInt32(argv[2], &rangeStart) ||
	    !FREGetInt32(argv[3], &rangeEnd)) return FREBool(false);

	return FREBool(g_Steam->DownloadLeaderboardEntries(handle,
		ELeaderboardDataRequest(request), rangeStart, rangeEnd));
}

AIR_FUNC(AIRSteam_DownloadLeaderboardEntriesResult) {
	ARG_CHECK(1, FREArray(0));

	int32 numDetails;
	if (!FREGetInt32(argv[0], &numDetails)) return FREArray(0);

	auto entries = g_Steam->DownloadLeaderboardEntriesResult(numDetails);
	if (entries.empty()) return FREArray(0);

	uint32 size = static_cast<uint32>(entries.size());
	FREObject array = FREArray(size);
	for (uint32 i = 0; i < size; ++i) {
		FREObject el;
		FRENewObject((const uint8_t*)"com.amanitadesign.steam.LeaderboardEntry", 0, NULL, &el, NULL);

		LeaderboardEntry_t *e = &entries[i].entry;
		SET_PROP(el, "userID",     FREUint64(e->m_steamIDUser.ConvertToUint64()));
		SET_PROP(el, "globalRank", FREInt(e->m_nGlobalRank));
		SET_PROP(el, "score",      FREInt(e->m_nScore));
		SET_PROP(el, "numDetails", FREInt(e->m_cDetails));

		int32 dets = e->m_cDetails;
		if (numDetails < dets) dets = numDetails;
		FREObject details = FREArray(dets);
		for (int d = 0; d < dets; ++d) {
			FRESetArrayElementAt(details, d, FREInt(entries[i].details[d]));
		}
		SET_PROP(el, "details", details);

		FRESetArrayElementAt(array, i, el);
	}

	return array;
}

/*
 * cloud storage
 */

AIR_FUNC(AIRSteam_GetFileCount) {
	if (!g_Steam) return FREInt(0);

	return FREInt(g_Steam->GetFileCount());
}

AIR_FUNC(AIRSteam_GetFileSize) {
	ARG_CHECK(1, FREInt(0));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREInt(0);

	return FREInt(g_Steam->GetFileSize(name));
}

AIR_FUNC(AIRSteam_FileExists) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->FileExists(name));
}

AIR_FUNC(AIRSteam_FileWrite) {
	ARG_CHECK(2, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[1], &byteArray) != FRE_OK)
		return FREBool(false);

	bool ret = g_Steam->FileWrite(name, byteArray.bytes, byteArray.length);
	FREReleaseByteArray(argv[1]);

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_FileRead) {
	ARG_CHECK(2, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	char* data = NULL;
	int32 size = g_Steam->FileRead(name, &data);

	// ensure the bytearray is big enough
	SET_PROP(argv[1], "length", FREUint(size));

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[1], &byteArray) != FRE_OK) {
		delete[] data;
		return FREBool(false);
	}

	memcpy(byteArray.bytes, data, size);

	FREReleaseByteArray(argv[1]);
	delete[] data;

	return FREBool(true);
}

AIR_FUNC(AIRSteam_FileDelete) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->FileDelete(name));
}

AIR_FUNC(AIRSteam_FileShare) {
	ARG_CHECK(1, FREBool(false));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREBool(false);

	return FREBool(g_Steam->FileShare(name));
}

AIR_FUNC(AIRSteam_FileShareResult) {
	ARG_CHECK(0, FREUint64(k_UGCHandleInvalid));

	return FREUint64(g_Steam->FileShareResult());
}

AIR_FUNC(AIRSteam_IsCloudEnabledForApp) {
	if (!g_Steam) return FREBool(false);

	return FREBool(g_Steam->IsCloudEnabledForApp());
}

AIR_FUNC(AIRSteam_SetCloudEnabledForApp) {
	ARG_CHECK(1, FREBool(false));

	bool enabled;
	if (!FREGetBool(argv[0], &enabled)) return FREBool(false);

	return FREBool(g_Steam->SetCloudEnabledForApp(enabled));
}

AIR_FUNC(AIRSteam_GetQuota) {
	if (!g_Steam) return FREArray(0);

	uint64 total, avail;
	if (!g_Steam->GetQuota(&total, &avail)) return FREArray(0);

	FREObject array = FREArray(2);
	FRESetArrayElementAt(array, 0, FREUint64(total));
	FRESetArrayElementAt(array, 1, FREUint64(avail));

	return array;
}

/*
 * ugc / workshop
 */

AIR_FUNC(AIRSteam_UGCDownload) {
	ARG_CHECK(2, FREBool(false));
	ARG_CHECK(2, FREBool(false));

	UGCHandle_t handle;
	uint32 priority;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetUint32(argv[1], &priority)) return FREBool(false);

	return FREBool(g_Steam->UGCDownload(handle, priority));
}

AIR_FUNC(AIRSteam_UGCRead) {
	ARG_CHECK(4, FREBool(false));

	UGCHandle_t handle;
	int32 _size;
	uint32 offset;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetInt32(argv[1], &_size) ||
	    !FREGetUint32(argv[2], &offset)) return FREBool(false);

	if (_size <= 0) return FREBool(false);
	uint32 size = _size;

	// ensure the bytearray is big enough
	SET_PROP(argv[3], "length", FREUint(size));

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[3], &byteArray) != FRE_OK)
		return FREBool(false);

	char* data = NULL;
	int32 result = g_Steam->UGCRead(handle, size, offset, &data);

	bool ret = false;
	if (result > 0 && static_cast<uint32>(result) <= byteArray.length) {
		ret = true;
		memcpy(byteArray.bytes, data, result);
	}

	FREReleaseByteArray(argv[3]);
	delete[] data;

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_GetUGCDownloadProgress) {
	ARG_CHECK(1, FREArray(0));

	UGCHandle_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREArray(0);

	int32 downloaded, expected;
	if (!g_Steam->GetUGCDownloadProgress(handle, &downloaded, &expected))
		return FREArray(0);

	FREObject array = FREArray(2);
	FRESetArrayElementAt(array, 0, FREInt(downloaded));
	FRESetArrayElementAt(array, 1, FREInt(expected));

	return array;
}

AIR_FUNC(AIRSteam_GetUGCDownloadResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.DownloadUGCResult", 0, NULL, &result, NULL);

	ARG_CHECK(1, result);

	UGCHandle_t handle;
	if (!FREGetUint64(argv[0], &handle)) return result;

	auto details = g_Steam->GetUGCDownloadResult(handle);
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "fileHandle", FREUint64(details->m_hFile));
	SET_PROP(result, "appID", FREUint(details->m_nAppID));
	SET_PROP(result, "size", FREInt(details->m_nSizeInBytes));
	SET_PROP(result, "fileName", FREString(details->m_pchFileName));
	SET_PROP(result, "owner", FREUint64(details->m_ulSteamIDOwner));

	return result;
}

AIR_FUNC(AIRSteam_PublishWorkshopFile) {
	ARG_CHECK(8, FREBool(false));

	std::string name, preview, title, description;
	uint32 appId, visibility, fileType;
	if (!FREGetString(argv[0], name) ||
	    !FREGetString(argv[1], preview) ||
	    !FREGetUint32(argv[2], &appId) ||
	    !FREGetString(argv[3], title) ||
	    !FREGetString(argv[4], description) ||
	    !FREGetUint32(argv[5], &visibility) ||
	    !FREGetUint32(argv[7], &fileType)) return FREBool(false);

	std::vector<std::string> tags = extractStringArray(argv[6]);
	SteamParamStringArray_t tagArray;
	createParamStringArray(tags, &tagArray);

	bool ret = g_Steam->PublishWorkshopFile(name, preview, appId, title, description,
		ERemoteStoragePublishedFileVisibility(visibility), &tagArray,
		EWorkshopFileType(fileType));

	delete[] tagArray.m_ppStrings;

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_PublishWorkshopFileResult) {
	ARG_CHECK(0, FREUint64(0));

	return FREUint64(g_Steam->PublishWorkshopFileResult());
}

AIR_FUNC(AIRSteam_DeletePublishedFile) {
	ARG_CHECK(1, FREBool(false));

	PublishedFileId_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREBool(false);

	return FREBool(g_Steam->DeletePublishedFile(handle));
}

AIR_FUNC(AIRSteam_GetPublishedFileDetails) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileId_t handle;
	uint32 maxAge;
	if (!FREGetUint64(argv[0], &handle) ||
	   !FREGetUint32(argv[1], &maxAge)) return FREBool(false);

	return FREBool(g_Steam->GetPublishedFileDetails(handle, maxAge));
}

AIR_FUNC(AIRSteam_GetPublishedFileDetailsResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.FileDetailsResult", 0, NULL, &result, NULL);

	ARG_CHECK(1, result);

	PublishedFileId_t file;
	if (!FREGetUint64(argv[0], &file)) return result;

	auto details = g_Steam->GetPublishedFileDetailsResult(file);
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "file", FREUint64(details->m_nPublishedFileId));
	SET_PROP(result, "creatorAppID", FREUint(details->m_nCreatorAppID));
	SET_PROP(result, "consumerAppID", FREUint(details->m_nConsumerAppID));
	SET_PROP(result, "title", FREString(details->m_rgchTitle));
	SET_PROP(result, "description", FREString(details->m_rgchDescription));
	SET_PROP(result, "fileHandle", FREUint64(details->m_hFile));
	SET_PROP(result, "previewFileHandle", FREUint64(details->m_hPreviewFile));
	SET_PROP(result, "owner", FREUint64(details->m_ulSteamIDOwner));
	SET_PROP(result, "timeCreated", FREUint(details->m_rtimeCreated));
	SET_PROP(result, "timeUpdated", FREUint(details->m_rtimeUpdated));
	SET_PROP(result, "visibility", FREInt(details->m_eVisibility));
	SET_PROP(result, "banned", FREBool(details->m_bBanned));
	SET_PROP(result, "tags", FREString(details->m_rgchTags));
	SET_PROP(result, "tagsTruncated", FREBool(details->m_bTagsTruncated));
	SET_PROP(result, "fileName", FREString(details->m_pchFileName));
	SET_PROP(result, "fileSize", FREInt(details->m_nFileSize));
	SET_PROP(result, "previewFileSize", FREInt(details->m_nPreviewFileSize));
	SET_PROP(result, "url", FREString(details->m_rgchURL));
	SET_PROP(result, "fileType", FREInt(details->m_eFileType));

	return result;
}

AIR_FUNC(AIRSteam_EnumerateUserPublishedFiles) {
	ARG_CHECK(1, FREBool(false));

	uint32 startIndex;
	if (!FREGetUint32(argv[0], &startIndex)) return FREBool(false);

	return FREBool(g_Steam->EnumerateUserPublishedFiles(startIndex));
}

AIR_FUNC(AIRSteam_EnumerateUserPublishedFilesResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.UserFilesResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->EnumerateUserPublishedFilesResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "resultsReturned", FREInt(details->m_nResultsReturned));
	SET_PROP(result, "totalResults", FREInt(details->m_nTotalResultCount));

	FREObject ids = FREArray(details->m_nResultsReturned);
	for (int32 i = 0; i < details->m_nResultsReturned; ++i) {
		FRESetArrayElementAt(ids, i, FREUint64(details->m_rgPublishedFileId[i]));
	}
	SET_PROP(result, "publishedFileId", ids);

	return result;
}

AIR_FUNC(AIRSteam_EnumeratePublishedWorkshopFiles) {
	ARG_CHECK(6, FREBool(false));

	uint32 type, start, count, days;
	if (!FREGetUint32(argv[0], &type) ||
	    !FREGetUint32(argv[1], &start) ||
	    !FREGetUint32(argv[2], &count) ||
	    !FREGetUint32(argv[3], &days)) return FREBool(false);

	std::vector<std::string> tags = extractStringArray(argv[4]);
	std::vector<std::string> userTags = extractStringArray(argv[5]);
	SteamParamStringArray_t tagArray, userTagArray;
	createParamStringArray(tags, &tagArray);
	createParamStringArray(userTags, &userTagArray);

	bool ret = g_Steam->EnumeratePublishedWorkshopFiles(
		EWorkshopEnumerationType(type), start, count, days, &tagArray, &userTagArray);

	delete[] tagArray.m_ppStrings;
	delete[] userTagArray.m_ppStrings;

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_EnumeratePublishedWorkshopFilesResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.WorkshopFilesResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->EnumeratePublishedWorkshopFilesResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "resultsReturned", FREInt(details->m_nResultsReturned));
	SET_PROP(result, "totalResults", FREInt(details->m_nTotalResultCount));

	FREObject ids = FREArray(details->m_nResultsReturned);
	FREObject scores = FREArray(details->m_nResultsReturned);
	for (int32 i = 0; i < details->m_nResultsReturned; ++i) {
		FRESetArrayElementAt(ids, i, FREUint64(details->m_rgPublishedFileId[i]));
		FRESetArrayElementAt(scores, i, FREDouble(details->m_rgScore[i]));
	}
	SET_PROP(result, "publishedFileId", ids);
	SET_PROP(result, "score", scores);

	return result;
}

AIR_FUNC(AIRSteam_EnumerateUserSubscribedFiles) {
	ARG_CHECK(1, FREBool(false));

	uint32 startIndex;
	if (!FREGetUint32(argv[0], &startIndex)) return FREBool(false);

	return FREBool(g_Steam->EnumerateUserSubscribedFiles(startIndex));
}

AIR_FUNC(AIRSteam_EnumerateUserSubscribedFilesResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.SubscribedFilesResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->EnumerateUserSubscribedFilesResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "resultsReturned", FREInt(details->m_nResultsReturned));
	SET_PROP(result, "totalResults", FREInt(details->m_nTotalResultCount));

	FREObject ids = FREArray(details->m_nResultsReturned);
	FREObject timesSubscribed = FREArray(details->m_nResultsReturned);
	for (int32 i = 0; i < details->m_nResultsReturned; ++i) {
		FRESetArrayElementAt(ids, i, FREUint64(details->m_rgPublishedFileId[i]));
		FRESetArrayElementAt(timesSubscribed, i, FREUint(details->m_rgRTimeSubscribed[i]));
	}
	SET_PROP(result, "publishedFileId", ids);
	SET_PROP(result, "timeSubscribed", timesSubscribed);

	return result;
}

AIR_FUNC(AIRSteam_EnumerateUserSharedWorkshopFiles) {
	ARG_CHECK(4, FREBool(false));

	uint64 steamID;
	uint32 start;
	if (!FREGetUint64(argv[0], &steamID) ||
	    !FREGetUint32(argv[1], &start)) return FREBool(false);

	std::vector<std::string> required = extractStringArray(argv[2]);
	std::vector<std::string> excluded = extractStringArray(argv[3]);
	SteamParamStringArray_t requiredArray, excludedArray;
	createParamStringArray(required, &requiredArray);
	createParamStringArray(excluded, &excludedArray);

	bool ret = g_Steam->EnumerateUserSharedWorkshopFiles(CSteamID(steamID), start,
		&requiredArray, &excludedArray);

	delete[] requiredArray.m_ppStrings;
	delete[] excludedArray.m_ppStrings;

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_EnumerateUserSharedWorkshopFilesResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.UserFilesResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->EnumerateUserSharedWorkshopFilesResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "resultsReturned", FREInt(details->m_nResultsReturned));
	SET_PROP(result, "totalResults", FREInt(details->m_nTotalResultCount));

	FREObject ids = FREArray(details->m_nResultsReturned);
	for (int32 i = 0; i < details->m_nResultsReturned; ++i) {
		FRESetArrayElementAt(ids, i, FREUint64(details->m_rgPublishedFileId[i]));
	}
	SET_PROP(result, "publishedFileId", ids);

	return result;
}

AIR_FUNC(AIRSteam_EnumeratePublishedFilesByUserAction) {
	ARG_CHECK(2, FREBool(false));

	uint32 action, startIndex;
	if (!FREGetUint32(argv[0], &action) ||
	    !FREGetUint32(argv[1], &startIndex)) return FREBool(false);

	return FREBool(g_Steam->EnumeratePublishedFilesByUserAction(
		EWorkshopFileAction(action), startIndex));
}

AIR_FUNC(AIRSteam_EnumeratePublishedFilesByUserActionResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.FilesByUserActionResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->EnumeratePublishedFilesByUserActionResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "action", FREInt(details->m_eAction));
	SET_PROP(result, "resultsReturned", FREInt(details->m_nResultsReturned));
	SET_PROP(result, "totalResults", FREInt(details->m_nTotalResultCount));

	FREObject ids = FREArray(details->m_nResultsReturned);
	FREObject timeUpdated = FREArray(details->m_nResultsReturned);
	for (int32 i = 0; i < details->m_nResultsReturned; ++i) {
		FRESetArrayElementAt(ids, i, FREUint64(details->m_rgPublishedFileId[i]));
		FRESetArrayElementAt(timeUpdated, i, FREUint(details->m_rgRTimeUpdated[i]));
	}

	SET_PROP(result, "publishedFileId", ids);
	SET_PROP(result, "timeUpdated", timeUpdated);

	return result;
}

AIR_FUNC(AIRSteam_SubscribePublishedFile) {
	ARG_CHECK(1, FREBool(false));

	PublishedFileId_t file;
	if (!FREGetUint64(argv[0], &file)) return FREBool(false);

	return FREBool(g_Steam->SubscribePublishedFile(file));
}

AIR_FUNC(AIRSteam_UnsubscribePublishedFile) {
	ARG_CHECK(1, FREBool(false));

	PublishedFileId_t file;
	if (!FREGetUint64(argv[0], &file)) return FREBool(false);

	return FREBool(g_Steam->UnsubscribePublishedFile(file));
}

AIR_FUNC(AIRSteam_CreatePublishedFileUpdateRequest) {
	ARG_CHECK(1, FREUint64(k_PublishedFileUpdateHandleInvalid));

	PublishedFileId_t file;
	if (!FREGetUint64(argv[0], &file))
		return FREUint64(k_PublishedFileUpdateHandleInvalid);

	return FREUint64(g_Steam->CreatePublishedFileUpdateRequest(file));
}

AIR_FUNC(AIRSteam_UpdatePublishedFileFile) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	std::string file;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetString(argv[1], file)) return FREBool(false);

	return FREBool(g_Steam->UpdatePublishedFileFile(handle, file));
}

AIR_FUNC(AIRSteam_UpdatePublishedFilePreviewFile) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	std::string preview;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetString(argv[1], preview)) return FREBool(false);

	return FREBool(g_Steam->UpdatePublishedFilePreviewFile(handle, preview));
}

AIR_FUNC(AIRSteam_UpdatePublishedFileTitle) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	std::string title;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetString(argv[1], title)) return FREBool(false);

	return FREBool(g_Steam->UpdatePublishedFileTitle(handle, title));
}

AIR_FUNC(AIRSteam_UpdatePublishedFileDescription) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREBool(false);

	std::string description;
	if (!FREGetString(argv[1], description)) return FREBool(false);

	return FREBool(g_Steam->UpdatePublishedFileDescription(handle, description));
}

AIR_FUNC(AIRSteam_UpdatePublishedFileSetChangeDescription) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	std::string changeDesc;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetString(argv[1], changeDesc)) return FREBool(false);


	return FREBool(g_Steam->UpdatePublishedFileSetChangeDescription(handle, changeDesc));
}

AIR_FUNC(AIRSteam_UpdatePublishedFileVisibility) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	uint32 visibility;
	if (!FREGetUint64(argv[0], &handle) ||
	    !FREGetUint32(argv[1], &visibility)) return FREBool(false);

	return FREBool(g_Steam->UpdatePublishedFileVisibility(handle,
		ERemoteStoragePublishedFileVisibility(visibility)));
}

AIR_FUNC(AIRSteam_UpdatePublishedFileTags) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREBool(false);

	std::vector<std::string> tags = extractStringArray(argv[1]);
	SteamParamStringArray_t tagArray;
	createParamStringArray(tags, &tagArray);

	bool ret = g_Steam->UpdatePublishedFileTags(handle, &tagArray);

	delete[] tagArray.m_ppStrings;

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_CommitPublishedFileUpdate) {
	ARG_CHECK(1, FREBool(false));

	PublishedFileUpdateHandle_t handle;
	if (!FREGetUint64(argv[0], &handle)) return FREBool(false);

	return FREBool(g_Steam->CommitPublishedFileUpdate(handle));
}

AIR_FUNC(AIRSteam_GetPublishedItemVoteDetails) {
	ARG_CHECK(1, FREBool(false));

	PublishedFileId_t file;
	if (!FREGetUint64(argv[0], &file)) return FREBool(false);

	return FREBool(g_Steam->GetPublishedItemVoteDetails(file));
}

AIR_FUNC(AIRSteam_GetPublishedItemVoteDetailsResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.ItemVoteDetailsResult", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->GetPublishedItemVoteDetailsResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "publishedFileId", FREUint64(details->m_unPublishedFileId));
	SET_PROP(result, "votesFor", FREInt(details->m_nVotesFor));
	SET_PROP(result, "votesAgainst", FREInt(details->m_nVotesAgainst));
	SET_PROP(result, "reports", FREInt(details->m_nReports));
	SET_PROP(result, "score", FREDouble(details->m_fScore));

	return result;
}

AIR_FUNC(AIRSteam_GetUserPublishedItemVoteDetails) {
	ARG_CHECK(1, FREBool(false));

	PublishedFileId_t file;
	if (!FREGetUint64(argv[0], &file)) return FREBool(false);

	return FREBool(g_Steam->GetUserPublishedItemVoteDetails(file));
}

AIR_FUNC(AIRSteam_GetUserPublishedItemVoteDetailsResult) {
	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.UserVoteDetails", 0, NULL, &result, NULL);

	ARG_CHECK(0, result);
	auto details = g_Steam->GetUserPublishedItemVoteDetailsResult();
	if (!details) return result;

	SET_PROP(result, "result", FREInt(details->m_eResult));
	SET_PROP(result, "publishedFileId", FREUint64(details->m_nPublishedFileId));
	SET_PROP(result, "vote", FREInt(details->m_eVote));

	return result;
}

AIR_FUNC(AIRSteam_UpdateUserPublishedItemVote) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileId_t file;
	bool upvote;
	if (!FREGetUint64(argv[0], &file) ||
	    !FREGetBool(argv[1], &upvote)) return FREBool(false);


	return FREBool(g_Steam->UpdateUserPublishedItemVote(file, upvote));
}

AIR_FUNC(AIRSteam_SetUserPublishedFileAction) {
	ARG_CHECK(2, FREBool(false));

	PublishedFileId_t file;
	uint32 action;
	if (!FREGetUint64(argv[0], &file) ||
	    !FREGetUint32(argv[1], &action)) return FREBool(false);


	return FREBool(g_Steam->SetUserPublishedFileAction(file,
		EWorkshopFileAction(action)));
}

/*
 * friends
 */

AIR_FUNC(AIRSteam_GetFriendCount) {
	ARG_CHECK(1, FREInt(0));

	uint32 flags;
	if (!FREGetUint32(argv[0], &flags)) return FREInt(0);

	return FREInt(g_Steam->GetFriendCount(flags));
}

AIR_FUNC(AIRSteam_GetFriendByIndex) {
	ARG_CHECK(2, FREUint64(0));

	int32 index;
	uint32 flags;
	if (!FREGetInt32(argv[0], &index) ||
		  !FREGetUint32(argv[1], &flags)) return FREUint64(0);

	return FREUint64(g_Steam->GetFriendByIndex(index, flags).ConvertToUint64());
}

AIR_FUNC(AIRSteam_GetFriendPersonaName) {
	ARG_CHECK(1, FREString(""));

	uint64 steamId;
	if (!FREGetUint64(argv[0], &steamId)) return FREString("");

	return FREString(g_Steam->GetFriendPersonaName(CSteamID(steamId)));
}

AIR_FUNC(AIRSteam_GetSmallFriendAvatar) {
	ARG_CHECK(1, nullptr);

	uint64 steamId;
	if (!FREGetUint64(argv[0], &steamId)) return nullptr;

	Image image = g_Steam->GetSmallFriendAvatar(CSteamID(steamId));
	if (image.data.size() == 0) return nullptr;
	return FREBitmapDataFromImageRGBA(image.width, image.height, image.argb_data());
}

AIR_FUNC(AIRSteam_GetMediumFriendAvatar) {
	ARG_CHECK(1, nullptr);

	uint64 steamId;
	if (!FREGetUint64(argv[0], &steamId)) return nullptr;

	Image image = g_Steam->GetMediumFriendAvatar(CSteamID(steamId));
	if (image.data.size() == 0) return nullptr;
	return FREBitmapDataFromImageRGBA(image.width, image.height, image.argb_data());
}

AIR_FUNC(AIRSteam_GetLargeFriendAvatar) {
	ARG_CHECK(1, nullptr);

	uint64 steamId;
	if (!FREGetUint64(argv[0], &steamId)) return nullptr;

	Image image = g_Steam->GetLargeFriendAvatar(CSteamID(steamId));
	if (image.data.size() == 0) return nullptr;
	return FREBitmapDataFromImageRGBA(image.width, image.height, image.argb_data());
}


AIR_FUNC(AIRSteam_GetCoplayFriendCount) {
	if (!g_Steam) return FREInt(0);

	return FREInt(g_Steam->GetCoplayFriendCount());
}

AIR_FUNC(AIRSteam_GetCoplayFriend) {
	if (!g_Steam) return FREUint64(0);

	ARG_CHECK(1, FREInt(0));

	int32 index;
	if (!FREGetInt32(argv[0], &index)) return FREUint64(0);

	return FREUint64(g_Steam->GetCoplayFriend(index).ConvertToUint64());
}

AIR_FUNC(AIRSteam_SetRichPresence) {
	ARG_CHECK(2, FREBool(false));

	std::string key;
	if (!FREGetString(argv[0], key)) return FREBool(false);

	std::string value;
	if (!FREGetString(argv[1], value)) return FREBool(false);

	return FREBool(g_Steam->SetRichPresence(key, value));
}

AIR_FUNC(AIRSteam_ClearRichPresence) {
	ARG_CHECK(0, FREBool(false));
	return FREBool(g_Steam->ClearRichPresence());
}

AIR_FUNC(AIRSteam_SetPlayedWith) {
	ARG_CHECK(1, FREBool(false));

	uint64 steamId;
	if (!FREGetUint64(argv[0], &steamId)) return FREBool(false);

	return FREBool(g_Steam->SetPlayedWith(CSteamID(steamId)));
}

/*
 * authentication & ownership
 */

AIR_FUNC(AIRSteam_GetAuthSessionTicket) {
	ARG_CHECK(2, FREUint(k_HAuthTicketInvalid));

	uint64 steamId;
	if (!FREGetUint64(argv[1], &steamId)) return nullptr;

	char* data = nullptr;
	uint32 length = 0;
	HAuthTicket ret = g_Steam->GetAuthSessionTicket(&data, &length, CSteamID(steamId));

	SET_PROP(argv[0], "length", FREUint(length));

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[0], &byteArray) != FRE_OK) {
		delete[] data;
		return FREUint(k_HAuthTicketInvalid);
	}

	memcpy(byteArray.bytes, data, length);
	delete[] data;

	FREReleaseByteArray(argv[0]);

	return FREUint(ret);
}

AIR_FUNC(AIRSteam_GetAuthSessionTicketResult) {
	ARG_CHECK(0, FREUint(k_HAuthTicketInvalid));

	return FREUint(g_Steam->GetAuthSessionTicketResult());
}

AIR_FUNC(AIRSteam_BeginAuthSession) {
	FREObject result = FREInt(k_EBeginAuthSessionResultInvalidTicket);
	ARG_CHECK(2, result);

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[0], &byteArray) != FRE_OK)
		return result;

	uint64 steamId;
	if (!FREGetUint64(argv[1], &steamId)) return result;

	EBeginAuthSessionResult ret = g_Steam->BeginAuthSession(byteArray.bytes,
		byteArray.length, CSteamID(steamId));
	FREReleaseByteArray(argv[0]);

	return FREInt(ret);
}

AIR_FUNC(AIRSteam_EndAuthSession) {
	ARG_CHECK(1, FREBool(false));

	uint64 steamId;
	if (!FREGetUint64(argv[0], &steamId)) return FREBool(false);

	return FREBool(g_Steam->EndAuthSession(CSteamID(steamId)));
}

AIR_FUNC(AIRSteam_CancelAuthTicket) {
	ARG_CHECK(1, FREBool(false));

	HAuthTicket ticket;
	if (!FREGetUint32(argv[0], &ticket)) return FREBool(false);

	return FREBool(g_Steam->CancelAuthTicket(ticket));
}

AIR_FUNC(AIRSteam_UserHasLicenseForApp) {
	ARG_CHECK(2, FREInt(k_EUserHasLicenseResultNoAuth));

	uint64 steamId;
	uint32 appId;
	if (!FREGetUint64(argv[0], &steamId) ||
		  !FREGetUint32(argv[1], &appId)) return FREInt(k_EUserHasLicenseResultNoAuth);

	return FREInt(g_Steam->UserHasLicenseForApp(CSteamID(steamId), appId));
}

AIR_FUNC(AIRSteam_RequestEncryptedAppTicket) {
	ARG_CHECK(1, FREBool(false));

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[0], &byteArray) != FRE_OK)
		return FREBool(false);

	bool result = g_Steam->RequestEncryptedAppTicket(byteArray.bytes, byteArray.length);

	FREReleaseByteArray(argv[0]);

	return FREBool(result);
}

AIR_FUNC(AIRSteam_GetEncryptedAppTicket) {
	ARG_CHECK(1, FREBool(false));

	char* data = nullptr;
	uint32 length = 0;
	bool ret = g_Steam->GetEncryptedAppTicket(&data, &length);

	SET_PROP(argv[0], "length", FREUint(length));

	FREByteArray byteArray;
	if (FREAcquireByteArray(argv[0], &byteArray) != FRE_OK) {
		delete[] data;
		return FREBool(false);
	}

	memcpy(byteArray.bytes, data, length);
	delete[] data;

	FREReleaseByteArray(argv[0]);

	return FREBool(ret);
}

AIR_FUNC(AIRSteam_GetAuthTicketForWebApi) {
	ARG_CHECK(1, FREUint(k_HAuthTicketInvalid));

	std::string identity;
	if (!FREGetString(argv[0], identity))
		return FREUint(k_HAuthTicketInvalid);

	HAuthTicket ticket = g_Steam->GetAuthTicketForWebApi(identity);
    
	return FREUint(ticket);
}

AIR_FUNC(AIRSteam_GetAuthTicketForWebApiResultHandle) {
	ARG_CHECK(0, FREUint(k_HAuthTicketInvalid));

	HAuthTicket ticket = g_Steam->GetAuthTicketForWebApiResultHandle();
	return FREUint(ticket);
}

AIR_FUNC(AIRSteam_GetAuthTicketForWebApiResultHexString) {
	ARG_CHECK(0, FREString(""));

	std::string hexString = g_Steam->GetAuthTicketForWebApiResultHexString();
	return FREString(hexString);
}

/*
 * overlay
 */

AIR_FUNC(AIRSteam_ActivateGameOverlay) {
	ARG_CHECK(1, FREBool(false));

	std::string dialog;
	if (!FREGetString(argv[0], dialog)) return FREBool(false);

	return FREBool(g_Steam->ActivateGameOverlay(dialog));
}

AIR_FUNC(AIRSteam_ActivateGameOverlayToUser) {
	ARG_CHECK(2, FREBool(false));

	std::string dialog;
	uint64 steamId;
	if (!FREGetString(argv[0], dialog) ||
	    !FREGetUint64(argv[1], &steamId)) return FREBool(false);


	return FREBool(g_Steam->ActivateGameOverlayToUser(dialog, CSteamID(steamId)));
}

AIR_FUNC(AIRSteam_ActivateGameOverlayToWebPage) {
	ARG_CHECK(1, FREBool(false));

	std::string url;
	if (!FREGetString(argv[0], url)) return FREBool(false);

	return FREBool(g_Steam->ActivateGameOverlayToWebPage(url));
}

AIR_FUNC(AIRSteam_ActivateGameOverlayToStore) {
	ARG_CHECK(2, FREBool(false));

	uint32 appId, flag;
	if (!FREGetUint32(argv[0], &appId) ||
	    !FREGetUint32(argv[1], &flag)) return FREBool(false);

	return FREBool(g_Steam->ActivateGameOverlayToStore(appId,
		EOverlayToStoreFlag(flag)));
}

AIR_FUNC(AIRSteam_ActivateGameOverlayInviteDialog) {
	ARG_CHECK(1, FREBool(false));

	uint64 lobbyId;
	if (!FREGetUint64(argv[0], &lobbyId)) return FREBool(false);

	return FREBool(g_Steam->ActivateGameOverlayInviteDialog(CSteamID(lobbyId)));
}

AIR_FUNC(AIRSteam_IsOverlayEnabled) {
	ARG_CHECK(0, FREBool(false));

	return FREBool(g_Steam->IsOverlayEnabled());
}

AIR_FUNC(AIRSteam_SetOverlayNotificationPosition) {
	ARG_CHECK(1, FREBool(false));

	uint32 pos;
	if (!FREGetUint32(argv[0], &pos))
		return FREBool(false);

	return FREBool(g_Steam->SetOverlayNotificationPosition(
		ENotificationPosition(pos)));
}

AIR_FUNC(AIRSteam_OverlayNeedsPresent) {
	ARG_CHECK(0, FREBool(false));

	return FREBool(g_Steam->OverlayNeedsPresent());
}

AIR_FUNC(AIRSteam_SetOverlayNotificationInset) {
	ARG_CHECK(2, FREBool(false));

	int hInset;
	if (!FREGetInt32(argv[0], &hInset))
		return FREBool(false);

	int vInset;
	if (!FREGetInt32(argv[1], &vInset))
		return FREBool(false);

	return FREBool(g_Steam->SetOverlayNotificationInset(hInset, vInset));
}

/*
 * DLC / subscriptions
 */

AIR_FUNC(AIRSteam_IsSubscribedApp) {
	ARG_CHECK(1, FREBool(false));

	uint32 appId;
	if (!FREGetUint32(argv[0], &appId)) return FREBool(false);

	return FREBool(g_Steam->IsSubscribedApp(appId));
}

AIR_FUNC(AIRSteam_IsDLCInstalled) {
	ARG_CHECK(1, FREBool(false));

	uint32 appId;
	if (!FREGetUint32(argv[0], &appId)) return FREBool(false);

	return FREBool(g_Steam->IsDLCInstalled(appId));
}

AIR_FUNC(AIRSteam_GetDLCCount) {
	ARG_CHECK(0, FREInt(0));

	return FREInt(g_Steam->GetDLCCount());
}

AIR_FUNC(AIRSteam_InstallDLC) {
	ARG_CHECK(1, FREBool(false));

	uint32 appId;
	if (!FREGetUint32(argv[0], &appId)) return FREBool(false);

	return FREBool(g_Steam->InstallDLC(appId));
}

AIR_FUNC(AIRSteam_UninstallDLC) {
	ARG_CHECK(1, FREBool(false));

	uint32 appId;
	if (!FREGetUint32(argv[0], &appId)) return FREBool(false);

	return FREBool(g_Steam->UninstallDLC(appId));
}

AIR_FUNC(AIRSteam_DLCInstalledResult) {
	ARG_CHECK(0, FREUint(0));

	return FREUint(g_Steam->DLCInstalledResult());
}

/*
 * Microtransaction
 */
AIR_FUNC(AIRSteam_MicroTxnResult) {
	ARG_CHECK(0, nullptr);

	MicroTxnAuthorizationResponse_t response;
	if (!g_Steam->MicroTxnResult(&response)) return nullptr;

	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.MicroTxnAuthorizationResponse", 0, NULL, &result, NULL);

	SET_PROP(result, "appID", FREInt(response.m_unAppID));
	SET_PROP(result, "orderID", FREUint64(response.m_ulOrderID));
	SET_PROP(result, "authorized", FREBool(response.m_bAuthorized != 0));

	return result;
}

/*
 * other
 */

AIR_FUNC(AIRSteam_GetEnv) {
	ARG_CHECK(1, FREString(""));

	std::string name;
	if (!FREGetString(argv[0], name)) return FREString("");

	const char* ret = ::getenv(name.c_str());
	return FREString(ret == nullptr ? "" : ret);
}

AIR_FUNC(AIRSteam_SetEnv) {
	ARG_CHECK(2, FREBool(false));

	std::string name, value;
	if (!FREGetString(argv[0], name) ||
		  !FREGetString(argv[1], value)) return FREBool(false);

#ifdef WIN32
	errno_t ret = _putenv_s(name.c_str(), value.c_str());
#else
	int ret = ::setenv(name.c_str(), value.c_str(), 1);
#endif

	return FREBool(ret == 0);
}

/*
 * input
 */
AIR_FUNC(AIRSteam_InputInit) {
	ARG_CHECK(0, FREBool(false));

	return FREBool(g_Steam->InitInput());
}
AIR_FUNC(AIRSteam_GetControllerForGamepadIndex) {
	ARG_CHECK(1, FREInt(0));

	int32 index;
	if (!FREGetInt32(argv[0], &index)) return FREUint64(0);

	return FREUint64(g_Steam->GetControllerForGamepadIndex(index));
}
AIR_FUNC(AIRSteam_ShowBindingPanel) {
	ARG_CHECK(1, FREBool(false));

	uint64 inputHandle;
	if (!FREGetUint64(argv[0], &inputHandle)) return FREBool(false);

	return FREBool(g_Steam->ShowBindingPanel(inputHandle));
}
AIR_FUNC(AIRSteam_GetActionSetHandle) {
	ARG_CHECK(1, FREBool(false));

	std::string actionSetName;
	if (!FREGetString(argv[0], actionSetName)) return FREUint64(0);

	return FREUint64(g_Steam->GetActionSetHandle(actionSetName));
}
AIR_FUNC(AIRSteam_GetDigitalActionHandle) {
	ARG_CHECK(1, FREBool(false));

	std::string actionName;
	if (!FREGetString(argv[0], actionName)) return FREUint64(0);

	return FREUint64(g_Steam->GetDigitalActionHandle(actionName));
}
AIR_FUNC(AIRSteam_GetAnalogActionHandle) {
	ARG_CHECK(1, FREBool(false));

	std::string actionName;
	if (!FREGetString(argv[0], actionName)) return FREUint64(0);

	return FREUint64(g_Steam->GetAnalogActionHandle(actionName));
}
AIR_FUNC(AIRSteam_GetDigitalActionData) {
	ARG_CHECK(2, nullptr);

	uint64 inputHandle;
	uint64 digitalActionHandle;
	
	if (!FREGetUint64(argv[0], &inputHandle)) return FREBool(false);
	if (!FREGetUint64(argv[1], &digitalActionHandle)) return FREBool(false);

	InputDigitalActionData_t response = g_Steam->GetDigitalActionData(inputHandle, digitalActionHandle);

	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.InputDigitalActionData", 0, NULL, &result, NULL);

	SET_PROP(result, "bState", FREBool(response.bState));
	SET_PROP(result, "bActive", FREBool(response.bState));

	return result;
}
AIR_FUNC(AIRSteam_GetAnalogActionData) {
	ARG_CHECK(2, nullptr);

	uint64 inputHandle;
	uint64 analogActionHandle;
	
	if (!FREGetUint64(argv[0], &inputHandle)) return FREBool(false);
	if (!FREGetUint64(argv[1], &analogActionHandle)) return FREBool(false);

	InputAnalogActionData_t response = g_Steam->GetAnalogActionData(inputHandle, analogActionHandle);

	FREObject result;
	FRENewObject((const uint8_t*)"com.amanitadesign.steam.InputAnalogActionData", 0, NULL, &result, NULL);

	SET_PROP(result, "eMode", FREInt(response.eMode));
	SET_PROP(result, "x", FREDouble(response.x));
	SET_PROP(result, "y", FREDouble(response.y));
	SET_PROP(result, "bActive", FREBool(response.bActive));

	return result;
}
AIR_FUNC(AIRSteam_RunFrame) {
	ARG_CHECK(0, FREBool(false));

	return FREBool(g_Steam->RunFrame());
}
AIR_FUNC(AIRSteam_GetConnectedControllers) {
	ARG_CHECK(0, FREArray(0));

	// See how many Steam Controllers are active. 
	ControllerHandle_t pHandles[STEAM_CONTROLLER_MAX_COUNT];
	int const nNumActive = g_Steam->GetConnectedControllers(pHandles);

	FREObject array = FREArray(nNumActive);
	for (int i = 0; i < nNumActive; ++i) {
		FRESetArrayElementAt(array, i, FREUint64(pHandles[i]));
	}
	return array;
}
AIR_FUNC(AIRSteam_ActivateActionSet) {
	ARG_CHECK(2, FREBool(false));
	
	uint64 inputHandle;
	uint64 actionSetHandle;
	if (!FREGetUint64(argv[0], &inputHandle)) return FREBool(false);
	if (!FREGetUint64(argv[1], &actionSetHandle)) return FREBool(false);
	
	g_Steam->ActivateActionSet(inputHandle, actionSetHandle);
	
	return FREBool(true);
}
AIR_FUNC(AIRSteam_GetHandleAllControllers) {
	return FREUint64(g_Steam->GetHandleAllControllers());
}
AIR_FUNC(AIRSteam_GetDigitalActionOrigins) {
	ARG_CHECK(3, FREArray(0));
	
	uint64 inputHandle;
	uint64 actionSetHandle;
	uint64 actionHandle;
	if (!FREGetUint64(argv[0], &inputHandle)) return FREArray(0);
	if (!FREGetUint64(argv[1], &actionSetHandle)) return FREArray(0);
	if (!FREGetUint64(argv[2], &actionHandle)) return FREArray(0);

	EInputActionOrigin originsOut[STEAM_INPUT_MAX_ORIGINS];
	int const numOrigins = g_Steam->GetDigitalActionOrigins(inputHandle, actionSetHandle, actionHandle, originsOut);

	FREObject array = FREArray(numOrigins);
	for (int i = 0; i < numOrigins; ++i) {
		FRESetArrayElementAt(array, i, FREUint64(originsOut[i]));
	}
	return array;
}
AIR_FUNC(AIRSteam_GetAnalogActionOrigins) {
	ARG_CHECK(3, FREArray(0));
	
	uint64 inputHandle;
	uint64 actionSetHandle;
	uint64 actionHandle;
	if (!FREGetUint64(argv[0], &inputHandle)) return FREArray(0);
	if (!FREGetUint64(argv[1], &actionSetHandle)) return FREArray(0);
	if (!FREGetUint64(argv[0], &actionHandle)) return FREArray(0);

	EInputActionOrigin originsOut[STEAM_INPUT_MAX_ORIGINS];
	int const numOrigins = g_Steam->GetAnalogActionOrigins(inputHandle, actionSetHandle, actionHandle, originsOut);

	FREObject array = FREArray(numOrigins);
	for (int i = 0; i < numOrigins; ++i) {
		FRESetArrayElementAt(array, i, FREUint64(originsOut[i]));
	}
	return array;
}
AIR_FUNC(AIRSteam_GetGlyphSVGForActionOrigin) {
	ARG_CHECK(2, FREString("error0"));
	
	uint64 eOrigin;
	uint32 unFlags;
	if (!FREGetUint64(argv[0], &eOrigin)) return FREString("error1");
	if (!FREGetUint32(argv[1], &unFlags)) return FREString("error2");
	
	return FREString(g_Steam->GetGlyphSVGForActionOrigin(EInputActionOrigin(eOrigin), unFlags));
}
AIR_FUNC(AIRSteam_GetGlyphPNGForActionOrigin) {
	ARG_CHECK(3, FREString("error0"));
	
	uint64 eOrigin;
	uint32 eSize;
	uint32 unFlags;
	if (!FREGetUint64(argv[0], &eOrigin)) return FREString("error1");
	if (!FREGetUint32(argv[1], &eSize))   return FREString("error2");
	if (!FREGetUint32(argv[2], &unFlags)) return FREString("error3");
	
	return FREString(g_Steam->GetGlyphPNGForActionOrigin(EInputActionOrigin(eOrigin), ESteamInputGlyphSize(eSize), unFlags));
}
AIR_FUNC(AIRSteam_GetStringForActionOrigin) {
	ARG_CHECK(1, FREString("error0"));
	
	uint64 eOrigin;
	if (!FREGetUint64(argv[0], &eOrigin)) return FREString("error1");
	return FREString(g_Steam->GetStringForActionOrigin(EInputActionOrigin(eOrigin)));
}
AIR_FUNC(AIRSteam_ShowGamepadTextInput) {
	ARG_CHECK(5, FREBool(false));
	
	uint32 eInputMode;
	uint32 eLineInputMode;
	std::string pchDescription;
	uint32 unCharMax;
	std::string pchExistingText;
	
	if (!FREGetUint32(argv[0], &eInputMode)) 		return FREBool(false);
	if (!FREGetUint32(argv[1], &eLineInputMode))   	return FREBool(false);
	if (!FREGetString(argv[2], pchDescription)) 	return FREBool(false);
	if (!FREGetUint32(argv[3], &unCharMax)) 		return FREBool(false);
	if (!FREGetString(argv[4], pchExistingText)) 	return FREBool(false);
	
	const char* pchDescriptionChar = ::getenv(pchDescription.c_str());
	const char* pchExistingTextChar = ::getenv(pchExistingText.c_str());
	
	return FREBool(g_Steam->ShowGamepadTextInput(EGamepadTextInputMode(eInputMode), EGamepadTextInputLineMode(eLineInputMode), pchDescriptionChar, unCharMax, pchExistingTextChar));
}
AIR_FUNC(AIRSteam_ShowFloatingGamepadTextInput) {
	ARG_CHECK(5, FREBool(false));
	
	uint32 eKeyboardMode;
	uint32 nTextFieldXPosition;
	uint32 nTextFieldYPosition;
	uint32 nTextFieldWidth;
	uint32 nTextFieldHeight;
	
	if (!FREGetUint32(argv[0], &eKeyboardMode)) 		return FREBool(false);
	if (!FREGetUint32(argv[1], &nTextFieldXPosition))   return FREBool(false);
	if (!FREGetUint32(argv[2], &nTextFieldYPosition)) 	return FREBool(false);
	if (!FREGetUint32(argv[3], &nTextFieldWidth)) 		return FREBool(false);
	if (!FREGetUint32(argv[4], &nTextFieldHeight)) 		return FREBool(false);
	
	return FREBool(g_Steam->ShowFloatingGamepadTextInput(EFloatingGamepadTextInputMode(eKeyboardMode), nTextFieldXPosition, nTextFieldYPosition, nTextFieldWidth, nTextFieldHeight));
}
AIR_FUNC(AIRSteam_SteamInputShutDown) {
	ARG_CHECK(0, FREBool(false));
	return FREBool(g_Steam->SteamInputShutDown());
}
AIR_FUNC(AIRSteam_DismissFloatingGamepadTextInput) {
	ARG_CHECK(0, FREBool(false));
	return FREBool(g_Steam->DismissFloatingGamepadTextInput());
}

	//============================


#define FRE_FUNC(fname) \
		{ (const uint8_t*) #fname, NULL, &fname }

static FRENamedFunction func[] = {
#define X(a) FRE_FUNC(a),
#include "functions.h"
#undef X
};

// A native context instance is created
void ContextInitializer(void* extData, const uint8_t* ctxType, FREContext ctx,
                        uint32* numFunctions, const FRENamedFunction** functions) {
	AIRContext = ctx;
	*functions = func;
	*numFunctions = sizeof(func) / sizeof(func[0]);
}

// A native context instance is disposed
void ContextFinalizer(FREContext ctx) {
	AIRContext = NULL;
	// Shutdown Steam
	SteamAPI_Shutdown();
	// Delete the SteamAchievements object
	delete g_Steam;
	g_Steam = NULL;
}

extern "C" {
	// Initialization function of each extension
	EXPORT void ExtInitializerFRESteamWorks(void** extDataToSet, FREContextInitializer* ctxInitializerToSet,
	                           FREContextFinalizer* ctxFinalizerToSet) {
		*extDataToSet = NULL;
		*ctxInitializerToSet = &ContextInitializer;
		*ctxFinalizerToSet = &ContextFinalizer;
	}

	// Called when extension is unloaded
	EXPORT void ExtFinalizerFRESteamWorks(void* extData) {
		return;
	}
}
