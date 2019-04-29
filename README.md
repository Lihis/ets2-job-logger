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
