#include "visual_novel_web_maker_http_server.h"

// #include "WebSocketServer.h"
#include "EventLoop.h"
#include "htime.h"
#include "hssl.h"
#include "hlog.h"
#include <regex>
#include <wsjcpp_core.h>

using namespace hv;


VisualNovelWebMakerHttpServer::VisualNovelWebMakerHttpServer() {
    TAG = "VisualNovelWebMakerHttpServer";

    {
        logger_t* pLogger = hv_default_logger();
        // logger_set_max_filesize(pLogger, 102400);
        std::string sLogDirPath = "./hv_logs";
        if (!WsjcppCore::dirExists(sLogDirPath)) {
            WsjcppCore::makeDir(sLogDirPath);
        }
        std::string sLogFilePath = sLogDirPath + "/http_" + WsjcppCore::getCurrentTimeForFilename() + ".log";
        logger_set_file(pLogger, sLogFilePath.c_str());
    }

    m_sGamePathPrefix = "/game/";

    // m_sApiPathPrefix = "/api/v1/";
    // m_sTeamLogoPrefix = "/team-logo/";
    // m_nTeamLogoPrefixLength = m_sTeamLogoPrefix.size();

    // m_jsonGame["game_name"] = m_pConfig->gameName();
    // m_jsonGame["game_start"] = WsjcppCore::formatTimeUTC(m_pConfig->gameStartUTCInSec()) + " (UTC)";
    // m_jsonGame["game_end"] = WsjcppCore::formatTimeUTC(m_pConfig->gameEndUTCInSec()) + " (UTC)";
    // m_jsonGame["game_has_coffee_break"] = m_pConfig->gameHasCoffeeBreak();
    // m_jsonGame["game_coffee_break_start"] = WsjcppCore::formatTimeUTC(m_pConfig->gameCoffeeBreakStartUTCInSec()) + " (UTC)";
    // m_jsonGame["game_coffee_break_end"] = WsjcppCore::formatTimeUTC(m_pConfig->gameCoffeeBreakEndUTCInSec()) + " (UTC)";
    // m_jsonGame["teams"] = nlohmann::json::array();
    // m_jsonGame["services"] = nlohmann::json::array();

    // for (unsigned int i = 0; i < m_pConfig->servicesConf().size(); i++) {
    //     Ctf01dServiceDef serviceConf = m_pConfig->servicesConf()[i];
    //     if (serviceConf.isEnabled()) {
    //         nlohmann::json serviceInfo;
    //         serviceInfo["id"] = serviceConf.id();
    //         serviceInfo["name"] = serviceConf.name();
    //         serviceInfo["round_time_in_sec"] = serviceConf.timeSleepBetweenRunScriptsInSec();
    //         m_jsonGame["services"].push_back(serviceInfo);
    //     }
    // }

    // for (unsigned int i = 0; i < m_pConfig->teamsConf().size(); i++) {
    //     Ctf01dTeamDef teamConf = m_pConfig->teamsConf()[i];
    //     nlohmann::json teamInfo;
    //     teamInfo["id"] = teamConf.getId();
    //     teamInfo["name"] = teamConf.getName();
    //     teamInfo["ip_address"] = teamConf.ipAddress();
    //     teamInfo["logo"] = "./team-logo/" + teamConf.getId();
    //     teamInfo["logo_last_write_time"] = teamConf.getLogoLastWriteTime();

    //     m_jsonGame["teams"].push_back(teamInfo);
    //     m_jsonTeams["teams"].push_back(teamInfo);
    // }

    // m_sCacheResponseGameJson = m_jsonGame.dump();
    // m_sCacheResponseTeamsJson = m_jsonTeams.dump();
    // // m_sCacheResponseServicesJson =

    m_pHttpService = new HttpService();

    // static files
    m_pHttpService->document_root = ".";

    m_pHttpService->GET("*", std::bind(&VisualNovelWebMakerHttpServer::httpWebFolder, this, std::placeholders::_1, std::placeholders::_2));
    // // m_pHttpService->GET("/admin*", std::bind(&VisualNovelWebMakerHttpServer::httpAdmin, this, std::placeholders::_1, std::placeholders::_2));


    // // m_pHttpService->GET("/get", [](HttpRequest* req, HttpResponse* resp) {
    // //     resp->json["origin"] = req->client_addr.ip;
    // //     resp->json["url"] = req->url;
    // //     resp->json["args"] = req->query_params;
    // //     resp->json["headers"] = req->headers;
    // //     return 200;
    // // });
}

hv::HttpService *VisualNovelWebMakerHttpServer::getService() {
    return m_pHttpService;
}

int VisualNovelWebMakerHttpServer::httpApiV1GetPaths(HttpRequest* req, HttpResponse* resp) {
    return resp->Json(m_pHttpService->Paths());
}

int VisualNovelWebMakerHttpServer::httpAdmin(HttpRequest* req, HttpResponse* resp) {
    std::string str = req->path + " match /admin*";
    return resp->String(str);
}

int VisualNovelWebMakerHttpServer::httpWebFolder(HttpRequest* req, HttpResponse* resp) {
    std::string sOriginalRequestPath = req->path;
    WsjcppLog::info(TAG, "sOriginalRequestPath = " + sOriginalRequestPath);
    std::string sRequestPath;

    // remove get params from path
    std::size_t nFoundGetParams = sOriginalRequestPath.rfind("?");
    if (nFoundGetParams != std::string::npos) {
        sRequestPath = sOriginalRequestPath.substr(0, nFoundGetParams);
    } else {
        sRequestPath = sOriginalRequestPath;
    }

    sRequestPath = WsjcppCore::doNormalizePath(sRequestPath);

    // // WsjcppLog::info(TAG, "sRequestPath = " + sRequestPath);
    // if (sRequestPath == "/flag") {
    //     return this->httpApiV1Flag(req, resp);
    // }

    // if (sRequestPath.rfind(m_sTeamLogoPrefix, 0) == 0) {
    //     std::string sTeamId = sRequestPath.substr(m_nTeamLogoPrefixLength, sRequestPath.length() - m_nTeamLogoPrefixLength);
    //     Ctf01dTeamLogo *pLogo = m_pTeamLogos->findTeamLogo(sTeamId);
    //     if (pLogo == nullptr) {
    //         return 404;
    //     }
    //     resp->SetContentTypeByFilename(pLogo->sFilename.c_str());
    //     return resp->Data(
    //         pLogo->pBuffer,
    //         pLogo->nBufferSize,
    //         true, // nocopy
    //         resp->content_type
    //     );
    // }

    // if (sRequestPath.rfind(m_sApiPathPrefix, 0) == 0) {
    //     if (sRequestPath == "/api/v1/game") {
    //         resp->SetContentTypeByFilename("game.json");
    //         std::cout << m_sCacheResponseGameJson << std::endl;
    //         return resp->Data(
    //             (void *)(m_sCacheResponseGameJson.c_str()),
    //             m_sCacheResponseGameJson.length(),
    //             true,
    //             resp->content_type
    //         );
    //     } else if (sRequestPath == "/api/v1/scoreboard") {
    //         return this->httpApiV1Scoreboard(req, resp);
    //     } else if (sRequestPath == "/api/v1/teams") {
    //         resp->SetContentTypeByFilename("teams.json");
    //         return resp->Data(
    //             (void *)(m_sCacheResponseTeamsJson.c_str()),
    //             m_sCacheResponseTeamsJson.length(),
    //             true,
    //             resp->content_type
    //         );
    //     }
    //     return this->httpApiV1GetPaths(req, resp);
    // }

    if (sRequestPath == "/game") {
        // TODO redirect to /game/
        sRequestPath = "/game/index.html";
    }
    if (sRequestPath == "/game/") {
        sRequestPath = "/game/index.html";
    }

    if (sRequestPath.rfind(m_sGamePathPrefix, 0) == 0) {
        WsjcppLog::info(TAG, "Request path: " + sRequestPath);
        sRequestPath.erase(0, m_sGamePathPrefix.size());
        WsjcppLog::info(TAG, "Request path2: " + sRequestPath);
        std::string sFilePath = WsjcppCore::doNormalizePath("./game-template/" + sRequestPath);
        WsjcppLog::info(TAG, "sFilePath: " + sFilePath);
        if (WsjcppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
            return resp->File(sFilePath.c_str());
        }
        return 403;
    }

    if (sRequestPath == "/") {
        sRequestPath = "/index.html";
    }

    // TODO
    WsjcppLog::info(TAG, "Request path: " + sRequestPath);
    std::string sFilePath = WsjcppCore::doNormalizePath("./html/" + sRequestPath);
    if (WsjcppCore::fileExists(sFilePath)) { // TODO check the file exists not dir
        return resp->File(sFilePath.c_str());
    }

    return 404; // Not found
}


int VisualNovelWebMakerHttpServer::httpApiV1Flag(HttpRequest* req, HttpResponse* resp) {
    // auto now = std::chrono::system_clock::now().time_since_epoch();
    // int nCurrentTimeSec = std::chrono::duration_cast<std::chrono::seconds>(now).count();

    // if (nCurrentTimeSec < m_pConfig->gameStartUTCInSec()) {
    //     const std::string sErrorMsg = "Error(-8): Game not started yet";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }

    // if (m_pConfig->gameHasCoffeeBreak()
    //     && nCurrentTimeSec > m_pConfig->gameCoffeeBreakStartUTCInSec()
    //     && nCurrentTimeSec < m_pConfig->gameCoffeeBreakEndUTCInSec()
    // ) {
    //     static const std::string sErrorMsg = "Error(-8): Game on coffeebreak now";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }

    // if (nCurrentTimeSec > m_pConfig->gameEndUTCInSec()) {
    //     static const std::string sErrorMsg = "Error(-9): Game already ended";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }

    // std::string sTeamId = req->GetParam("teamid");
    // sTeamId = WsjcppCore::trim(sTeamId);
    // sTeamId = WsjcppCore::toLower(sTeamId);
    // std::string sFlag = req->GetParam("flag");
    // sFlag = WsjcppCore::trim(sFlag);
    // sFlag = WsjcppCore::toLower(sFlag);

    // if (sTeamId == "") {
    //     static const std::string sErrorMsg = "Error(-10): Not found get-parameter 'teamid' or parameter is empty";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }

    // if (sFlag == "") {
    //     static const std::string sErrorMsg = "Error(-11): Not found get-parameter 'flag' or parameter is empty";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }

    // // int nTeamNum = atoi(sTeamID.c_str());
    // // if (nTeamNum == 0) {
    // //     pRequest->response(
    // //         LightHttpRequest::RESP_BAD_REQUEST,
    // //         "text/html",
    // //         "Error(-12): 'teamid' must be number");
    // //     Log::warn(TAG, "Error(-12): 'teamid' must be number");
    // //     return true;
    // // }

    // bool bTeamFound = false;
    // for (unsigned int iteam = 0; iteam < m_pConfig->teamsConf().size(); iteam++) {
    //     Ctf01dTeamDef teamConf = m_pConfig->teamsConf()[iteam];
    //     if (teamConf.getId() == sTeamId) {
    //         bTeamFound = true;
    //     }
    // }


    // if (!bTeamFound) {
    //     static const std::string sErrorMsg = "Error(-130): this is team not found";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }



    // const static std::regex reFlagFormat("c01d[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}-[a-f0-9]{4,4}[0-9]{8,8}");
    // if (!std::regex_match(sFlag, reFlagFormat)) {
    //     static const std::string sErrorMsg = "Error(-140): flag has wrong format";
    //     WsjcppLog::err(TAG, sErrorMsg);
    //     return resp->String(sErrorMsg, 400);
    // }
    // m_pConfig->scoreboard()->incrementTries(sTeamId);

    // m_pEmployDatabase->insertFlagAttempt(sTeamId, sFlag);

    // // TODO m_pEmployFlags->insertFlagAttempt(sTeamId, sFlag);

    // Ctf01dFlag flag;
    // if (!m_pConfig->scoreboard()->findFlagLive(sFlag, flag)) {
    //     static const std::string sErrorMsg = "Error(-150): flag is too old or flag never existed or flag alredy stole.";
    //     WsjcppLog::err(TAG, sErrorMsg + ". Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
    //     return resp->String(sErrorMsg, 403);
    // }

    // long nCurrentTimeMSec = (long)nCurrentTimeSec;
    // nCurrentTimeMSec = nCurrentTimeMSec*1000;

    // if (flag.getTimeEndInMs() < nCurrentTimeMSec) {
    //     // TODO
    //     static const std::string sErrorMsg = "Error(-151): flag is too old";
    //     WsjcppLog::err(TAG, sErrorMsg + ". Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
    //     return resp->String(sErrorMsg, 403);
    // }

    // // if (flag.teamStole() == sTeamId) {
    // //     response.forbidden().sendText("Error(-160): flag already stole by your team");
    // //     WsjcppLog::err(TAG, "Error(-160): Recieved flag {" + sFlag + "} from {" + sTeamId + "} (flag already stole by your team)");
    // //     return true;
    // // }

    // if (flag.getTeamId() == sTeamId) {
    //     static const std::string sErrorMsg = "Error(-180): this is your flag";
    //     WsjcppLog::err(TAG, sErrorMsg + ". Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
    //     return resp->String(sErrorMsg, 403);
    // }

    // std::string sServiceStatus = m_pConfig->scoreboard()->serviceStatus(sTeamId, flag.getServiceId());

    // // std::cout << "sServiceStatus: " << sServiceStatus << "\n";

    // if (sServiceStatus != ServiceStatusCell::SERVICE_UP) {
    //     static const std::string sErrorMsg = "Error(-190): Your same service is dead. Try later.";
    //     WsjcppLog::err(TAG, sErrorMsg + ". Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
    //     return resp->String(sErrorMsg, 403);
    // }

    // if (m_pEmployDatabase->isAlreadyStole(flag, sTeamId)) {
    //     static const std::string sErrorMsg = "Error(-170): flag already stoled by your";
    //     WsjcppLog::err(TAG, sErrorMsg + ". Recieved flag {" + sFlag + "} from {" + sTeamId + "}");
    //     return resp->String(sErrorMsg, 403);
    // }

    // // TODO light update scoreboard
    // int nPoints = m_pConfig->scoreboard()->incrementAttackScore(flag, sTeamId);
    // std::string sPoints = std::to_string(double(nPoints) / 10.0);

    // std::string sResponse = "Accepted: Recieved flag {" + sFlag + "} from {" + sTeamId + "} (Accepted + " + sPoints + ")";
    // WsjcppLog::ok(TAG, sResponse);
    // return resp->Data((void *)(sResponse.c_str()), sResponse.size(), false, TEXT_PLAIN);
}

int VisualNovelWebMakerHttpServer::httpApiV1Scoreboard(HttpRequest* req, HttpResponse* resp) {
    // m_pTeamLogos->updateLastWriteTime();
    // nlohmann::json jsonScoreboard = m_pConfig->scoreboard()->toJson();
    // m_pTeamLogos->updateScorebordJson(jsonScoreboard);
    // std::string sScoreboardJson = jsonScoreboard.dump();
    // resp->SetContentTypeByFilename("scoreboard.json");
    // return resp->Data(
    //     (void *)(sScoreboardJson.c_str()),
    //     sScoreboardJson.length(),
    //     false, // nocopy - force copy
    //     resp->content_type
    // );
}