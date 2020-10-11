# Introduction

This repository hosts the source code for my ESP32-based smart doorlock. It works alongside a web API and a MQTT broker.

# MQTT commands

Some actions can be done remotely on the doorlock by sending a JSON object on the topic devices/doorlock/commands.

JSON format :

	{
		"command" : "",
		"arguments" : {
			"" : "",
			...
		}
	}

Available commands :
* delete_user (with user_id argument)
* reboot
* lock
* unlock

# Output

The doorlock publishes the id of any user successfully authenticated in the topic /devices/doorlock/entries.