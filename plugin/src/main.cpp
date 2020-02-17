/****************************************************************************
 * Copyright (c) 2019 Tomi Lähteenmäki <lihis@lihis.net>                    *
 *                                                                          *
 * This program is free software; you can redistribute it and/or modify     *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation; either version 2 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * This program is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,               *
 * MA 02110-1301, USA.                                                      *
 ****************************************************************************/

/**
 * @file main.cpp
 * @brief Register API callbacks
 * @author Tomi Lähteenmäki
 * @license This project is licensed under GNU General Public License, Version 2
 */

#include "Logger.h"
#include <functional>
#include <scs/scssdk_telemetry.h>
#include <scs/common/scssdk_telemetry_truck_common_channels.h>
#include <scs/common/scssdk_telemetry_trailer_common_channels.h>
#include <scs/common/scssdk_telemetry_job_common_channels.h>
#include <scs/amtrucks/scssdk_ats.h>
#include <scs/eurotrucks2/scssdk_eut2.h>

#ifndef _WIN32
#define __stdcall
#endif

typedef std::function<SCSAPI_VOID(const scs_value_t *const)> callback_channel_t;
typedef std::function<SCSAPI_VOID(const void *const event_info)> callback_event_t;

Logger *logger = nullptr;
callback_channel_t odometer_cb;
callback_channel_t fuel_cb;
callback_channel_t speed_cb;
callback_channel_t truckPlacement_cb;
callback_channel_t cargoDamage_cb;
callback_event_t frameEnd_cb;
callback_event_t eventPaused_cb;
callback_event_t eventStarted_cb;
callback_event_t config_cb;
callback_event_t gameplay_cb;

namespace cb {
    namespace bind {
        callback_channel_t channel(void (__stdcall Logger::*func)(const scs_value_t *const), Logger *obj) {
            return std::bind(func, obj, std::placeholders::_1);
        }

        callback_event_t event(void (__stdcall Logger::*func)(const scs_telemetry_frame_start_t *const), Logger *obj) {
            return std::bind((void (Logger::*)(const void *const))(func), obj, std::placeholders::_1);
        }

        callback_event_t event(void (__stdcall Logger::*func)(const void *const), Logger *obj) {
            return std::bind((void (Logger::*)(const void *const))(func), obj, std::placeholders::_1);
        }

        callback_event_t event(void (__stdcall Logger::*func)(const scs_telemetry_configuration_t *), Logger *obj) {
            return std::bind((void (Logger::*)(const void *const))(func), obj, std::placeholders::_1);
        }

        callback_event_t event(void (__stdcall Logger::*func)(const scs_telemetry_gameplay_event_t *), Logger *obj) {
            return std::bind((void (Logger::*)(const void *const))(func), obj, std::placeholders::_1);
        }
    }
}

SCSAPI_VOID callback_channel(const scs_string_t /*name*/, const scs_u32_t /*index*/, const scs_value_t *const value, scs_context_t context) {
    if (!context) {
        return;
    }

    auto callback = static_cast<callback_channel_t *>(context);
    (*callback)(value);
}

SCSAPI_VOID callback_event(const scs_event_t /*event*/, const void *const event_info, scs_context_t context) {
    if (!context) {
        return;
    }

    auto callback = static_cast<callback_event_t *>(context);
    (*callback)(event_info);
}

SCSAPI_RESULT scs_telemetry_init(const scs_u32_t version, const scs_telemetry_init_params_t *const params) {
    auto init_params = reinterpret_cast<const scs_telemetry_init_params_v101_t *>(params);

    if (version != SCS_TELEMETRY_VERSION_1_01) {
        return SCS_RESULT_unsupported;
    }

    Game game = Game::Unknown;
    std::string game_name = init_params->common.game_id;
    if (game_name == SCS_GAME_ID_EUT2) {
        game = Game::ETS2;
    } else if (game_name == SCS_GAME_ID_ATS) {
        game = Game::ATS;
    }

    if (game == Game::Unknown) {
        init_params->common.log(SCS_LOG_TYPE_error, "ETS2 Job Logger: Unsupported game");
        return SCS_RESULT_unsupported;
    }

    logger = new Logger(game);
    if (!logger) {
        return SCS_RESULT_generic_error;
    }
    logger->start();

    odometer_cb = cb::bind::channel(&Logger::odometer, logger);
    init_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_odometer, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_no_value, callback_channel, (void *)&odometer_cb);

    fuel_cb = cb::bind::channel(&Logger::fuel, logger);
    init_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_fuel, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_no_value, callback_channel, (void *)&fuel_cb);

    speed_cb = cb::bind::channel(&Logger::speed, logger);
    init_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_speed, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_no_value, callback_channel, (void *)&speed_cb);

    truckPlacement_cb = cb::bind::channel(&Logger::truckPlacement, logger);
    init_params->register_for_channel(SCS_TELEMETRY_TRUCK_CHANNEL_world_placement, SCS_U32_NIL, SCS_VALUE_TYPE_dplacement, SCS_TELEMETRY_CHANNEL_FLAG_no_value, callback_channel, (void *)&truckPlacement_cb);

    cargoDamage_cb = cb::bind::channel(&Logger::cargoDamage, logger);
    init_params->register_for_channel(SCS_TELEMETRY_JOB_CHANNEL_cargo_damage, SCS_U32_NIL, SCS_VALUE_TYPE_float, SCS_TELEMETRY_CHANNEL_FLAG_no_value, callback_channel, (void *)&cargoDamage_cb);

    frameEnd_cb = cb::bind::event(&Logger::frameEnd, logger);
    init_params->register_for_event(SCS_TELEMETRY_EVENT_frame_end, callback_event, (void *)&frameEnd_cb);

    eventPaused_cb = cb::bind::event(&Logger::eventPaused, logger);
    init_params->register_for_event(SCS_TELEMETRY_EVENT_paused, callback_event, (void *)&eventPaused_cb);

    eventStarted_cb = cb::bind::event(&Logger::eventStarted, logger);
    init_params->register_for_event(SCS_TELEMETRY_EVENT_started, callback_event, (void *)&eventStarted_cb);

    config_cb = cb::bind::event(&Logger::configuration, logger);
    init_params->register_for_event(SCS_TELEMETRY_EVENT_configuration, callback_event, (void *)&config_cb);

    gameplay_cb = cb::bind::event(&Logger::gameplay, logger);
    init_params->register_for_event(SCS_TELEMETRY_EVENT_gameplay, callback_event, (void *)&gameplay_cb);

    return SCS_RESULT_ok;
}

SCSAPI_VOID scs_telemetry_shutdown(void) {
    if (logger) {
        logger->stop();
    }
    delete logger;
}

#ifdef _WIN32
BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD fdwReason, LPVOID /*lpvReserved*/) {
    if (fdwReason == DLL_PROCESS_DETACH) {
        if (logger) {
			logger->stop();
			delete logger;
		}
	}

	return TRUE;
}
#endif
