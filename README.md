# ETS2 Job Logger

Cross-platform Euro Truck Simulator 2 job logger written in C++.

## Functionality

ETS2 Job Logger consists of wxWidgets based application and ETS2 plugin.

Plugin sends job related data to the application via websocket. Application then
sends a POST request to the specified API URL when a new job is taken or a
current job is delivered. See `JSON Format` paragraph for details of the format
sent to your API endpoint.

## JSON Format

Below is an example of JSON format sent to API when user takes a job or when
user finishes the job.

```
{
    "onJob": true,
    "delivered": false,
    "distanceDriven": 10.0,       // Kilometers
    "fuelConsumed": 8.2,          // Liters
    "trailerDamage": 0.0,         // Percentage (eg. 10.0 = 10%)
    "cargoName": "Office Paper",
    "cargoMass": 18000.0,         // Kilograms
    "sourceCity": "Tampere",
    "sourceCompany": "Viljo Paperitehdas Oy",
    "destinationCity": "Helsinki",
    "destinationCompany": "Container Port"
}
```

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
