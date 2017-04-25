#include "util.h"

std::string std_getenv(const std::string &name)
{
    auto value = getenv(name.c_str());
    return value != nullptr ? std::string(value) : std::string();
}

std::string random_string()
{
    auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[rand() % max_index];
    };
    std::stringstream rand_str;
    for (int i = 0; i < 16; i++)
    {
        rand_str << randchar();
    }
    return rand_str.str();
}

ssl_context_ptr tls_init(websocketpp::connection_hdl)
{

    ssl_context_ptr ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);

    try {

        ctx->set_options(boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::no_sslv3 |
            boost::asio::ssl::context::single_dh_use);

        ctx->set_verify_mode(boost::asio::ssl::verify_none);
        //ctx->set_verify_callback(bind(&verify_certificate, hostname, ::_1, ::_2));

        // Here we load the CA certificates of all CA's that this client trusts.
        //ctx->load_verify_file("ca-chain.cert.pem");

    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return ctx;
}