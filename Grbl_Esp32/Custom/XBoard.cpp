#include <nvs.h>
#include <nvs_flash.h>
#include "../grbl.h"

void machine_init()
{
	pinMode(13, OUTPUT);
	digitalWrite(13, HIGH);

    grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "Machine Type: %s", machineType->getStringValue());

	// machine_settings_load();
	// switch (MACHINE_TYPE)
	// {
	// case MACHINE_COREXY: grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "Machine Type: CoreXY"); break;
	// case MACHINE_XYZ: grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "Machine Type: XYZ"); break;
	// default: grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_ERROR, "Incorrect Machine Type"); break;
	// }
	// grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "Limits Enable: X %s, Y %s, Z %s",
	// 	LIMIT_ENABLE_MASK & 0x01 ? "Enable" : "Disable",
	// 	LIMIT_ENABLE_MASK & 0x02 ? "Enable" : "Disable",
	// 	LIMIT_ENABLE_MASK & 0x04 ? "Enable" : "Disable"
	// 	);
	// grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "Limits Type: X %s, Y %s, Z %s",
	// 	LIMIT_TYPE_MASK & 0x01 ? "NO" : "NC",
	// 	LIMIT_TYPE_MASK & 0x02 ? "NO" : "NC",
	// 	LIMIT_TYPE_MASK & 0x04 ? "NO" : "NC"
	// );
}

// void machine_settings_save()
// {
// 	nvs_handle handle;
// 	nvs_open("SPINDLE_TYPE", NVS_READWRITE, &handle);
// 	nvs_set_u8(handle, "SPINDLE_TYPE", SPINDLE_TYPE);
// 	nvs_close(handle);

// 	nvs_open("MACHINE_TYPE", NVS_READWRITE, &handle);
// 	nvs_set_u8(handle, "MACHINE_TYPE", MACHINE_TYPE);
// 	nvs_close(handle);

// 	nvs_open("LIMIT_ENABLE", NVS_READWRITE, &handle);
// 	nvs_set_u8(handle, "LIMIT_ENABLE", LIMIT_ENABLE_MASK);
// 	nvs_close(handle);

// 	nvs_open("LIMIT_TYPE", NVS_READWRITE, &handle);
// 	nvs_set_u8(handle, "LIMIT_TYPE", LIMIT_TYPE_MASK);
// 	nvs_close(handle);
// }

// void machine_settings_load()
// {
// 	nvs_handle handle;
// 	// SPINDLE_TYPE
// 	if (nvs_open("SPINDLE_TYPE", NVS_READWRITE, &handle) != ESP_OK)
// 	{
// 		nvs_close(handle);
// 		machine_settings_save();
// 		grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "SPINDLE_TYPE Not Builded");
// 	}
// 	else
// 	{
// 		nvs_get_u8(handle, "SPINDLE_TYPE", &SPINDLE_TYPE);
// 		nvs_close(handle);
// 	}

// 	// MACHINE_TYPE
// 	if (nvs_open("MACHINE_TYPE", NVS_READWRITE, &handle) != ESP_OK)
// 	{
// 		nvs_close(handle);
// 		machine_settings_save();
// 		grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "MACHINE_TYPE Not Builded");
// 	}
// 	else
// 	{
// 		nvs_get_u8(handle, "MACHINE_TYPE", &MACHINE_TYPE);
// 		nvs_close(handle);
// 	}

// 	// LIMIT_ENABLE_MASK
// 	if (nvs_open("LIMIT_ENABLE", NVS_READWRITE, &handle) != ESP_OK)
// 	{
// 		nvs_close(handle);
// 		machine_settings_save();
// 		grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "LIMIT_MASK Not Builded");
// 	}
// 	else
// 	{
// 		nvs_get_u8(handle, "LIMIT_ENABLE", &LIMIT_ENABLE_MASK);
// 		nvs_close(handle);
// 	}

// 	// LIMIT_TYPE_MASK
// 	if (nvs_open("LIMIT_TYPE", NVS_READWRITE, &handle) != ESP_OK)
// 	{
// 		nvs_close(handle);
// 		machine_settings_save();
// 		grbl_msg_sendf(CLIENT_SERIAL, MSG_LEVEL_INFO, "LIMIT_TYPE Not Builded");
// 	}
// 	else
// 	{
// 		nvs_get_u8(handle, "LIMIT_TYPE", &LIMIT_TYPE_MASK);
// 		nvs_close(handle);
// 	}
// }