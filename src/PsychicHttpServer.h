#ifndef PsychicHttpServer_h
#define PsychicHttpServer_h

#include "PsychicCore.h"
#include "PsychicClient.h"
#include "PsychicHandler.h"

class PsychicHttpServerEndpoint;
class PsychicHandler;
class PsychicStaticFileHandler;

class PsychicHttpServer
{
  protected:
    bool _use_ssl = false;
    std::list<PsychicHttpServerEndpoint*> _endpoints;
    std::list<PsychicHandler*> _handlers;
    std::list<PsychicClient*> _clients;

    PsychicClientCallback _onOpen;
    PsychicClientCallback _onClose;

    esp_err_t _start();

  public:
    PsychicHttpServer();
    ~PsychicHttpServer();

    //esp-idf specific stuff
    httpd_handle_t server;
    httpd_config_t config;
    httpd_ssl_config_t ssl_config;

    //some limits on what we will accept
    unsigned long maxUploadSize;
    unsigned long maxRequestBodySize;

    PsychicHttpServerEndpoint *defaultEndpoint;

    static void destroy(void *ctx);

    esp_err_t listen(uint16_t port);
    esp_err_t listen(uint16_t port, const char *cert, const char *private_key);
    void stop();

    PsychicHandler& addHandler(PsychicHandler* handler);
    void removeHandler(PsychicHandler* handler);

    void addClient(PsychicClient *client);
    void removeClient(PsychicClient *client);
    PsychicClient* getClient(int socket);
    PsychicClient* getClient(httpd_req_t *req);
    bool hasClient(int socket);

    PsychicHttpServerEndpoint* on(const char* uri);
    PsychicHttpServerEndpoint* on(const char* uri, http_method method);
    PsychicHttpServerEndpoint* on(const char* uri, PsychicHttpRequestHandler onRequest);
    PsychicHttpServerEndpoint* on(const char* uri, http_method method, PsychicHttpRequestHandler onRequest);
    PsychicHttpServerEndpoint* on(const char* uri, PsychicHandler *handler);
    PsychicHttpServerEndpoint* on(const char* uri, http_method method, PsychicHandler *handler);

    static esp_err_t notFoundHandler(httpd_req_t *req, httpd_err_code_t err);
    static esp_err_t defaultNotFoundHandler(PsychicHttpServerRequest *request);
    void onNotFound(PsychicHttpRequestHandler fn);

    void onOpen(PsychicClientCallback handler);
    void onClose(PsychicClientCallback handler);
    static esp_err_t openCallback(httpd_handle_t hd, int sockfd);
    static void closeCallback(httpd_handle_t hd, int sockfd);

    PsychicStaticFileHandler* serveStatic(const char* uri, fs::FS& fs, const char* path, const char* cache_control = NULL);
};

bool ON_STA_FILTER(PsychicHttpServerRequest *request);
bool ON_AP_FILTER(PsychicHttpServerRequest *request);

#endif // PsychicHttpServer_h