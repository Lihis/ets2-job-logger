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
current job is delivered. See `JSON Format` paragraph for details of the format
sent to your API endpoint.

Applications stores job information in memory until it's sent to your API. If
sending failed, eg. your API did not return expected HTTP code, applications
tries to send it again later until it's sent successfully.

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

Expected HTTP response code `200`. Program refuses to run if on the start
server can't be queried of it's capabilities either because of wrong HTTP
response code or the received JSON is invalid.

Configurable endpoints and example of JSON which server is expected to return:

```
{
    "truck": true                 // Supports Truck -endpoint (default: false)
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

**Note:** If `isSpecial` is `true` then `company` object in `source` and
`destination` will not exist.

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
        "id": ""scs_box.moving_floor.chassis_stwx2esii""
        "accessoryId": ""
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
0 = Unknow
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
  `CMakeListst.xt`.
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
