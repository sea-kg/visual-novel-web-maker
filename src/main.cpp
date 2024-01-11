#include <wsjcpp_core.h>

#include <visual_novel_web_maker_http_server.h>
#include "WebSocketServer.h"  // libhv

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    std::string m_sCurrentDir = WsjcppCore::getCurrentDirectory();
    std::string m_sGameTemplateDir = m_sCurrentDir + "/game-template";
    std::string m_sGameDir = m_sCurrentDir + "/game";

    // previous logs in current directory
    if (!WsjcppCore::dirExists("logs")) {
        WsjcppCore::makeDir("logs");
    }
    WsjcppLog::setPrefixLogFile("visual-novel-web-maker");
    WsjcppLog::setLogDirectory("logs");

    if (!WsjcppCore::dirExists(m_sGameDir)) {
        WsjcppCore::recoursiveCopyFiles(m_sGameTemplateDir, m_sGameDir);
    }

    // websocket_server_t server;
    // server.service = pRouter;
    // server.port = 12345;
    // // server.ws = pWs;
    // websocket_server_run(&server);

    VisualNovelWebMakerHttpServer httpServer;
    hv::HttpService *pRouter = httpServer.getService();
    hv::HttpServer server(pRouter);
    server.setPort(1120);
    server.setThreadNum(3);
    server.run();
}
