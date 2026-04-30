# Exchange

A real-time trading exchange system built with C++, React, and Node.js.

## What is this?

This project lets you run your own trading exchange on your computer. It has three parts:

- **Backend** - The matching engine written in C++ that processes buy/sell orders
- **Frontend** - A web interface built with React to view and interact with the exchange
- **Simulator** - A tool that generates fake market data to test the exchange

## Project Structure

```
exchange/
├── backend/          # C++ matching engine (uWebSockets)
├── frontend/         # React + TypeScript web app (Vite)
└── simulator/        # Node.js market data simulator
```

## Prerequisites

Before you begin, make sure you have these installed:

- **C++ compiler** (g++ or clang++ with C++23 support)
- **CMake** (version 3.20 or higher)
- **OpenSSL** development libraries
- **Node.js** (version 18 or higher recommended)
- **npm** (comes with Node.js)


## Getting Started

### Step 1: Clone and setup

```bash
git clone https://github.com/builder-inc/exchange.git
cd exchange
git submodule update --init --recursive
```

This downloads the uWebSockets library that the backend needs.

### Step 2: Build the Backend

The backend uses secure WebSocket connections (WSS), so you need SSL certificates for local development.

**Generate SSL certificates:**

```bash
cd backend
mkdir -p certs
openssl req -x509 -sha256 -newkey rsa:2048 -keyout certs/key.pem -out certs/cert.pem -days 365 -nodes \
    -subj "/C=IN/O=Builder Inc./CN=localhost" \
    -addext "subjectAltName = DNS:localhost"
```

**Build the C++ engine:**

```bash
cmake -S . -B build
cd build && make
```

**Run the test suite (optional):**
```bash
make check
```

**Start the backend server:**
```bash
./websocket_server
```

### Step 3: Install Frontend Dependencies

```bash
cd frontend
npm install
```

**Start the frontend dev server:**

```bash
npm run dev
```

This starts a local server. Open the URL shown in your terminal (usually http://localhost:5173).

### Step 4: Start the Simulator

In a new terminal:

```bash
cd simulator
npm install
npm run dev
```

The simulator will start sending mock market data to test the exchange.

## Available Commands

### Backend

**API Details:** See [backend/README.md](backend/README.md) for WebSocket API documentation, JSON message formats, and terminal testing tools (websocat, wscat).

| Command                                  | Description                              |
|------------------------------------------|------------------------------------------|
| `cmake -S backend -B backend/build`      | Generate build files (from project root) |
| `cd backend && cmake -S . -B build`      | Generate build files (from backend dir)  |
| `cd backend/build && make`               | Compile the engine                       |
| `cd backend/build && make tests`         | Build the test suite                     |
| `cd backend/build && ./tests`            | Run the test suite                       |
| `cd backend/build && make check`         | Build and run tests in one step          |
| `cd backend/build && ./websocket_server` | Start the backend WebSocket server       |

### Frontend

| Command           | Description                              |
|-------------------|------------------------------------------|
| `npm run dev`     | Start development server with hot reload |
| `npm run build`   | Create production build in `dist/`       |
| `npm run lint`    | Check code for style issues              |
| `npm run preview` | Preview the production build locally     |

### Simulator

| Command       | Description                     |
|---------------|---------------------------------|
| `npm run dev` | Start the market data simulator |

## Development Workflow

1. Start the backend first: `cd backend/build && ./websocket_server`
2. Start the simulator: `cd simulator && npm run dev`
3. Start the frontend: `cd frontend && npm run dev`
4. Open the frontend URL in your browser

## Troubleshooting

**CMake can't find OpenSSL:**
Make sure libssl-dev (Debian/Ubuntu) or openssl (macOS) is installed.

**Submodule error:**
Run `git submodule update --init --recursive` from the project root.

**OpenSSL certificate warnings:**
The browser will show a warning for self-signed certificates. This is normal for local development. Click "Advanced" and proceed.
