# Introduction

This repository hosts the source code for my ESP32-based smart doorlock. It works alongside a web API and a MQTT broker.

# MQTT commands

Some actions can be done remotely on the doorlock by sending a JSON object on the appropriate topic.

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
