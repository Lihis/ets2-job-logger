# ETS2 Job Logger

Cross-platform Euro Truck Simulator 2 and American Truck Simulator job logger
written in C++.

| Linux | Windows | MacOS |
| ----- | ------- | ----- |
| [![Linux Build Status](https://circleci.com/gh/Lihis/ets2-job-logger.svg?style=shield)](https://circleci.com/gh/Lihis/ets2-job-logger/?branch=master) | see [#63](https://github.com/Lihis/ets2-job-logger/issues/63) | see [#16](https://github.com/Lihis/ets2-job-logger/issues/16) |

Currently only Linux build is supported. A maintainer for Windows and MacOS builds is needed.

## Functionality

ETS2 Job Logger consists of wxWidgets based application and ETS2 plugin.

Plugin sends job related data to the application via websocket. Application then
sends a POST request to the specified API URL when a new job is taken or a
current job is delivered. If you're interested of implementing the API on your
website then refer to [API Guide](docs/API.md).

Application stores job information in memory until it's sent to your API. If
sending failed, e.g. your API did not return expected HTTP code, applications
tries to send it again later until it's sent successfully.

## Chat

[![IRC libera.chat #ets2joblogger](https://raster.shields.io/badge/IRC%20libera.chat-%23ets2joblogger-blue)](https://web.libera.chat/?channels=#ets2joblogger)
[![Discord](https://img.shields.io/badge/Discord-join-blue)](https://discord.gg/wyABv29)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

## Documentation

Documentation has been split into different documents.

[User Guide](docs/USER.md) covers necessary information for an application
users.

[API Guide](docs/API.md) covers necessary information for API implementors.

[Developer Guide](docs/DEVELOPER.md) covers necessary information for developers.

## License

See [LICENSE.md](LICENSE.md).

### Icon

![logo](gui/resources/icon/128x128.png)

"[Box, delivery, package, shipping, transport, truck icon](https://www.iconfinder.com/icons/4072100/box_delivery_package_shipping_transport_truck_icon)"
 by [icon lauk](https://www.iconfinder.com/andhikairfani) is licensed under [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/).
