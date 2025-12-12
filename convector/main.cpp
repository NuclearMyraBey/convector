#include "crow.h"
#include <atomic>

int main() {
    crow::SimpleApp app;

    std::atomic<int> clickCount{ 0 };

    CROW_ROUTE(app, "/")([] {
        auto page = crow::mustache::load_text("index.html");
        return crow::response(200, "text/html", page);
        });

    CROW_ROUTE(app, "/get")([&clickCount] {
        crow::json::wvalue response;
        response["count"] = clickCount.load();
        return crow::response(response);
        });

    CROW_ROUTE(app, "/increment").methods("POST"_method)([&clickCount] {
        int current = ++clickCount;
        crow::json::wvalue response;
        response["count"] = current;
        return crow::response(response);
        });

    CROW_ROUTE(app, "/reset").methods("POST"_method)([&clickCount] {
        clickCount = 0;
        crow::json::wvalue response;
        response["count"] = 0;
        response["message"] = "Counter reset";
        return crow::response(response);
        });

    app.bindaddr("127.0.0.1").port(8080).multithreaded().run();

    return 0;
}