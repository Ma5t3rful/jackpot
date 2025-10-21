#include "telegram_interface.hpp"
#include <string>
#include <cpr/cpr.h>
#include "shuffler.hpp"
#include <ranges>


auto main (int,char**) -> int
{
    TelegramInterface interface("7544314108:AAGWKrA4xrzSecOwueFg87R6Ap9efGYujCQ",[](const auto& message,auto* sender){
        if(not message.contains("text"))return;
        if(std::string(message["text"]).starts_with("/start"))
        {
            sender->async_send_message(
              "💠 Hello I'm Shuffler. 🔀\n"
              "📋 Send me a list and I will shuffle it for you.\n"
              "👁️‍🗨️ Be sure to REPLY to one of my messages so I can answer your request.\n"
              "✨ Let's get started by giving me a list like this:\nBen Smith\nJohn Griffin\nDonald Williams\netc.",
              message["chat"]["id"]
            );
            return;
        }
        else if(not message.contains("reply_to_message") or message["reply_to_message"]["from"]["id"]!=7544314108)return;

        /*std::println("poll signal received: \"{}\" from: {} with id={}",
            std::string(message["text"]),
            std::string(message["from"]["first_name"]),
            static_cast<long long>(message["chat"]["id"]));*/
        const auto shuffled_ent = shuffler::shuffle(std::string(message["text"]));
        const auto shuffled_names = shuffled_ent | std::views::transform([](const auto& str){return str+'\n';}) | std::views::join | std::ranges::to<std::string>();
        sender->async_send_message(shuffled_names,message["chat"]["id"]);
    });
    interface.mainloop();
}
