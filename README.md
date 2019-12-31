# ETS2 Job Logger

Cross-platform Euro Truck Simulator 2 and American Truck Simulator job logger
written in C++.

## Functionality

ETS2 Job Logger consists of wxWidgets based application and ETS2 plugin.

Plugin sends job related data to the application via websocket. Application then
sends a POST request to the specified API URL when a new job is taken or a
current job is delivered. See `JSON Format` paragraph for details of the format
sent to your API endpoint.

Applications stores job information in memory until it's sent to your API. If
sending failed, eg. your API did not return expected HTTP code, applications
tries to send it again later until it's sent successfully.

## JSON Format

Below is an example of JSON format sent to API when user takes a job or when
user finishes the job.

```
{
    "game": "ets2",               // Game type (ets2, ats)
    "onJob": true,
    "delivered": false,
    "distanceDriven": 10.0,       // Kilometers
    "fuelConsumed": 8.2,          // Liters
    "income": 6878                // In game specific units (€, $)
    "trailerDamage": 0.0,         // Percentage (eg. 10.0 = 10%)
    "cargoName": "Office Paper",
    "cargoId": "paper",
    "cargoMass": 18000.0,         // Kilograms
    "sourceCity": "Tampere",
    "sourceCityId": "tampere",
    "sourceCompany": "Viljo Paperitehdas Oy",
    "sourceCompanyId": "viljo_paper",
    "destinationCity": "Helsinki",
    "destinationCityId": "helsinki",
    "destinationCompany": "Container Port",
    "destinationCompanyId": "cont_port"
}
```

**API:**

Payload will be sent with `POST` request to `<API_URL>/job`. Expected response
code is `200`.

**User takes a job:**

`onJob` will be `true` and `delivered` will be `false`.

**User finished the job:**

`onJob` will be `false` and `delivered` will be `true`.

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
