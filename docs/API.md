# API Guide

Documentations for different API versions are found under [API](API/).

Current version [v1](API/v1.md).

## URL

Application sends requests and payloads to the `API URL` which can be set in
the application settings.

All sent requests will have the current API version appended to the URL. For
example if current version is `1` and URL in the settings is
`https://example.com/api` then the requests are sent to URL:
`https://example.com/api/v1`.

Remember to inform users of what URL they should set in the settings.

**Note:** Version specific documentations refers to `<API_URL>` which equals
to URL with version appended to it.

## Token

Requests sent to the API will contain an header `Authorization-token`. The
value of the token is the value set in the settings to the `API Token` field.

Application doesn't enforce any requirements for the token except that it is
set.
