#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <iostream>

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class MyListener
{
public:

    http_listener listener;

    MyListener(string_t address) : listener(address)
    {
        listener.support(methods::GET, std::bind(&MyListener::handle_get, this, std::placeholders::_1));
    }

    pplx::task<void> open() { return listener.open(); }
    pplx::task<void> close() { return listener.close(); }

    void handle_get(http_request message){ 
        cout << "[GET] : "<< message.request_uri().to_string() << endl;
        json::value value; 
        value["result"] = json::value::string("Hello C++");
        message.reply(status_codes::OK,value).wait();
    }

};


int main(int argc, char** argv)
{
    string_t port = U("8080");
    if (argc == 2)
    {
        port = conversions::to_string_t(argv[1]);
    }

    string_t address = U("http://localhost:");
    address.append(port);
    address.append(U("/RESTtest"));

    MyListener myListener(address);
    myListener.open().wait();

    std::cout << "Press ENTER to exit." << std::endl;

    std::string line;
    std::getline(std::cin, line);

    myListener.close().wait();
    return 0;
}