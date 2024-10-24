#include <dpp/dpp.h>
	 
	int main() {
	    /* Create the bot, but with our intents so we can use messages. */
	    dpp::cluster bot("token", dpp::i_default_intents | dpp::i_message_content);
	 
	    bot.on_log(dpp::utility::cout_logger());
	 
	    /* The event is fired when the bot detects a message in any server and any channel it has access to. */
	    bot.on_message_create([&bot](const dpp::message_create_t& event) {
	        /* See if the message contains the phrase we want to check for.
	         * If there's at least a single match, we reply and say it's not allowed.
	         */
	        if (event.msg.content.find("bad word") != std::string::npos) {
	            event.reply("That is not allowed here. Please, mind your language!", true);
	        }
	    });

        bot.on_ready([&bot](const dpp::ready_t& event) {
            // Create a message object
            dpp::message msg;
            msg.content = "We're online!"; // Set the content of the message
            msg.channel_id = "channelID";    // Specify the channel to send the message to

	        // Define a callback function to handle the result of the message creation
            auto callback = [](const dpp::confirmation_callback_t& callback) {
                
            };

            // Send the message to the specified channel
            bot.message_create(msg, callback);

            // Start POCSAG receive/send cycle
            while(true) {
                std::string msgToSend;
                std::getline(std::cin, msgToSend);
            }


        });
        
	    bot.start(dpp::st_wait);
	 
	    return 0;
	}