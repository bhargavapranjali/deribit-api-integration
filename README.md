# Deribit API Integration

This project is designed to provide a comprehensive suite of APIs that interact with the Deribit test endpoints. The APIs support key trading operations with a focus on low-latency performance, data integrity, and robust error handling.

## Table of Contents

- [Features](#features)
- [Architecture](#architecture)
  - [Public APIs](#public-apis)
  - [Private APIs](#private-apis)
- [Functionalities](#functionalities)
  - [Order Operations](#order-operations)
  - [Position Retrieval](#position-retrieval)
  - [Order Book Retrieval](#order-book-retrieval)
- [Validation & Error Handling](#validation--error-handling)
- [Auth Service Workflow](#auth-service-workflow)
- [Order/Position Service Workflow](#orderposition-service-workflow)

## Features

- **Place, Modify, and Cancel Orders**: Supports essential trading operations on the Deribit platform.
- **Get Order Book**: Retrieve real-time market data for informed decision-making.
- **View Current Positions**: Check open positions quickly for account management.
- **Low-Latency**: Optimized for high performance with minimized response times.
- **Comprehensive Error Handling**: Detailed error responses ensure a smooth user experience.
- **Early Validation**: Enhances data integrity and prevents unnecessary API calls by validating requests early.

## Architecture

The APIs are divided into two main scopes: **Public** and **Private**.

### Public APIs

Public API requests interact with Deribit's public endpoints. Each request is validated locally before being forwarded to Deribit, ensuring data integrity.

- **Validation**: Checks are performed on incoming requests to validate the request structure.
- **Forwarding**: If valid, the request is forwarded to Deribit's test endpoints, and the response is sent back to the client.

### Private APIs

Private API requests involve additional authentication steps. These requests must obtain a valid access token before accessing Deribit's private endpoints.

- **Authentication Service**: Requests an access token by checking a cached token or retrieving a new one if necessary.
- **Token Management**: Tokens are stored in a cache and updated lazily when expired or missing.

## Functionalities

### Order Operations

- **Place Order**: Initiates a new order on the Deribit platform.
- **Modify Order**: Adjusts an existing order.
- **Cancel Order**: Cancels an open order.

### Position Retrieval

Allows clients to view current trading positions and track performance.

### Order Book Retrieval

Fetches real-time order book data to provide insight into market depth and trading volume.

## Validation & Error Handling

- **Early Validation**: Ensures that requests meet the required format and parameters before processing.
- **Error Responses**:
  - `400 Bad Request`: Returned if validation fails at any stage.
  - **Auth Errors**: If an access token is missing or expired, the Auth service will request a new one from Deribit, cache it, and return it to the client.
  - `500 Internal Error`: Returned if either json cannot be parsed into a valid object or an exception occurred at backend
- **Structured Responses**: Clear messaging for every outcome, enhancing the overall user experience.

## Auth Service Workflow

The Auth service manages the authentication tokens for accessing Deribit's private endpoints. Here’s how it operates:

1. **Client or Service Request**: A client or service sends a `POST` request to the Auth service with their credentials.
2. **Request Validation**: The Auth service performs validation on the incoming request. If the request is invalid, it returns a `400 Bad Request` error to the client.
3. **Token Lookup**:
   - The Auth service searches for an access token associated with the provided credentials.
   - If a valid token is found, it is returned to the client or service.
4. **Token Expiration**:
   - If the token is missing or expired, the Auth service initiates a request to Deribit’s auth API to obtain a new access token.
5. **Cache Update**: The newly fetched access token is stored in the cache using lazy population, ensuring the token remains available for future requests.
6. **Token Response**: The Auth service then sends the valid access token back to the client or service.

## Order/Position Service Workflow

The Order/Position service facilitates placing, modifying, canceling orders, and retrieving order book data. Here’s the workflow:

1. **Client Request**: A client sends a `POST` request to the Order service with credentials and the specific action they wish to perform (place, cancel, modify an order, or get the order book).
2. **Request Validation**:
   - The Order service validates the incoming request.
   - If the request is invalid, a `400 Bad Request` error is returned to the client, and further processing is halted.
3. **Access Token Retrieval**:
   - The Order service requests an access token from the Auth service by providing the client credentials.
   - Upon receiving the access token, the Order service constructs the endpoint URL for the Deribit API based on the requested action.
4. **Request Execution**:
   - The Order service sends the constructed request to the appropriate Deribit API endpoint.
5. **Response Handling**:
   - The response from Deribit is then forwarded to the client, completing the request.

