# ETS2 Job Logger

Cross-platform Euro Truck Simulator 2 and American Truck Simulator job logger
written in C++.

| Linux | Windows | MacOS |
| ----- | ------- | ----- |
| [![Build Status](https://cloud.drone.io/api/badges/Lihis/ets2-job-logger/status.svg?ref=refs/heads/master)](https://cloud.drone.io/Lihis/ets2-job-logger) | [![Build status](https://ci.appveyor.com/api/projects/status/ev7ijqvsoclm84v4/branch/master?svg=true)](https://ci.appveyor.com/project/Lihis/ets2-job-logger/branch/master) | see [#16](https://github.com/Lihis/ets2-job-logger/issues/16) |

## Functionality

ETS2 Job Logger consists of wxWidgets based application and ETS2 plugin.

Plugin sends job related data to the application via websocket. Application then
sends a POST request to the specified API URL when a new job is taken or a
current job is delivered. See [API Documentation](#api-documentation) paragraph
for details of the format sent to your API endpoint.

Applications stores job information in memory until it's sent to your API. If
sending failed, eg. your API did not return expected HTTP code, applications
tries to send it again later until it's sent successfully.

## Chat

[![IRC freenode #ets2joblogger](https://raster.shields.io/badge/IRC%20freenode-%23ets2joblogger-blue)](https://webchat.freenode.net/?channels=%23ets2joblogger)
[![Discord](https://img.shields.io/badge/Discord-join-blue)](https://discord.gg/wyABv29)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## API Documentation

Current version `v1`.

### URL

Payloads will be sent with `POST` request to URL in form of
`<API_URL>/<API_VERSION>/<MSG_TYPE>`. For example `https://example.com/api/v1/job`.

`<API_URL>` is set by an user in the application settings (i.e. `API URL`).
Users should be guided to set `API URL` to point to your API and it should
not include the version, for example `https://example.com/api/`.

`<API_VERSION>` is automatically appended to the configured `API URL`, see
current version from above.

`<MSG_TYPE>` refers to the different messages documented below.

In the rest of this document `<API_URL>` refers to `<API_URL>/<API_VERSION>`.

### Capabilities

This endpoint, `<API_URL>/capabilities`, is used to query the server of it's
capabilities, i.e. which endpoints it supports. This is queried when program
starts or if user changes settings.

Expected HTTP response code `200`. Program will not send anything to the API
until the capabilities has been successfully queried. Query can fail because
of server did not return HTTP code `200` or the received JSON was invalid.

Configurable endpoints and example of JSON which server is expected to return:

```
{
    "truck": true                 // Supports Truck -endpoint (default: false)
    "fine": true                  // Supports Fine -endpoint (default: false)
}
```

JSON can be minified and keys with default values can be omitted if you want
to use the default value.

### Job

Jobs will be sent to `<API_URL>/job` when user takes a job or cancels or
delivers it.

Expected HTTP response code `200`. Otherwise payload sending will be retried
in 30 seconds intervals until API responds with HTTP code `200`.

Below is an example of JSON format for a job.

**Notes:**

- If `isSpecial` is `true` then `company` object in `source` and
  `destination` will not exist.
- Truck _can be_ wrong when status is `Delivered` or `Cancelled`
    - e.g. when user owns a truck but decides to do a quick job; truck is
      correct when `OnJob` but when `Delivered`/`Cancelled` the truck is
      reported as the truck the user owns instead of the one used in the job.

```
{
    "game": "ets2",               // Game type (ets2, ats)
    "status": 1,                  // See "Status" below
    "type": 1                     // See "Types" below
    "isSpecial": false            // Is this special transport job
    "income": 6878,               // In game specific units (ETS2: €, ATS: $)
    "revenue": 6878,              // In game specific units (ETS2: €, ATS: $)
    "xp": 120                     // XP received
    "time": 234                   // Time spend on the job in game minutes
    "maxSpeed": 0.0,              // Maximum  speed during the delivery (can be negative)
    "fuelConsumed": 0.0,          // Liters
    "autoPark": false,            // Was auto parking used
    "autoLoad": false,            // Was auto loading used (always true for non cargo market jobs)
    "distance": {
        "driven": 0.0,            // Kilometers
        "planned": 248            // Planned trip distance kilometers
    },
    "trailer": {
        "id": "scs_box.moving_floor.chassis_stwx2esii"
        "accessoryId": ""
    },
    "truck": {
        "id": "vehicle.scania.streamline",
        "name": "Streamline",
        "wheels": 6,
        "brand": {
            "id": "scania",
            "name": "Scania"
        }
    },
    "cargo": {
        "id": "paper",
        "name": "Office Paper",
        "mass": 18000.0           // Kilograms
        "damage": 0.0,            // Percentage (e.g. 0.01 = 1%)
    },
    "source": {
        "city": {
            "id": "tampere",
            "name": "Tampere"
        },
        "company": {
            "id": "viljo_paper",
            "name": "Viljo Paperitehdas Oy"
        }
    },
    "destination": {
        "city": {
            "id": "helsinki",
            "name": "Helsinki"
        },
        "company": {
            "id": "cont_port",
            "name": "Container Port"
        }
    }
}
```

#### Status

Description of job status:

```
0 = FreeAsWind
1 = OnJob
2 = Cancelled
3 = Delivered
```

**Note:** Job with a status `0` should not ever be sent to the API, if that
happens it's a bug.

#### Types

Description of job types:

```
0 = Unknown
1 = CargoMarket
2 = QuickJob
3 = FreightMarket
4 = ExternalContract
5 = ExternalMarket
```

**Note:** If job has status of `0` it's most likely bug and an Issue should
be opened if there is no open issue of it already.

### Truck

Trucks positional data will be sent to `<API_URL>/truck` once in a second only
if game is not paused.

Response code is ignored; payload is only sent once.

Below is an example of JSON format for a truck positional data.

```
{
    "speed": 0.0,
    "heading": 0.723,
    "x": 34496.559,
    "y": 11.938,
    "z": -61094.948
}
```

### Fine

When player receives a fine it will be sent to `<API_URL>/fine`. Fines will be
sent to the API even when player does not have an active job.

Expected response code `200`.

Below is an example of JSON format for fine:

```
{
    "type": 1,                    // Explanation below
    "amount": 370                 // Fine offence amount in native game currency
}
```

Valid `Types` are:

```
0  = Unknown
1  = Crash
2  = AvoidSleeping
3  = WrongWay
4  = SpeedingCamera
5  = NoLights
6  = RedSignal
7  = Speeding
8  = AvoidWeighing
9  = IllegalTrailer
10 = AvoidInspection
11 = IllegalBorderCrossing
12 = HardShoulderViolation
13 = DamagedVehicleUsage
14 = Generic
```

If API receives fine with type `Unknown` then create an issue (if does not
exist yet) as it could indicate game has a new fine type.

## Building

### Dependencies

- `cmake`
- `msgpack`
- `websocketpp`
- `curl`
- `openssl`
- `jsoncpp`
- `wxWidgets`

### Windows

Install `conan`. Open `PowerShell` and add remotes for `conan`:

```
conan remote add public-conan https://api.bintray.com/conan/bincrafters/public-conan
conan remote add theirix https://api.bintray.com/conan/bincrafters/theirix
```

- Open project in Visual Studio via `File -> Open -> CMake...` by choosing the
  `CMakeListst.txt`.
- Edit CMake options via `CMake -> Change CMake settings -> CMakelists.txt`
  - Change `generator` value to `Visual Studio 15 2017 Win64`.
  - Remove `-v` from `buildCommandArgs` or specify valid verbosity level.
- CMake will produce error, note down the path from the error:
  > include could not find load file:
  >
  > C:/Users/Test/CMakeBuilds/d76aad98-e5ad-0738-bbe9-bc20c6d93f9c/build/x64-Debug/conanbuildinfo.cmake`
- Open PowerShell in the source directory and run:
  - `conan install . -s build_type=Debug -if='<PATH_FROM_PREVIOUS_STEP>'`
- After installation finished go back to Visual Studio click `Generate` in the
  notification.
- After CMake generation finished you're ready to build.

When you're ready to build a release just replace `Debug` with `Release` in:
- CMakeSettings.json (`CMake -> Change CMake settings -> CMakelists.txt`)
- Replace all `Debug` parts of the `conan` install command.

## License

See [LICENSE.md](LICENSE.md).

### Icon

![logo](gui/resources/icon/128x128.png)

"[Box, delivery, package, shipping, transport, truck icon](https://www.iconfinder.com/icons/4072100/box_delivery_package_shipping_transport_truck_icon)"
 by [icon lauk](https://www.iconfinder.com/andhikairfani) is licensed under [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).
