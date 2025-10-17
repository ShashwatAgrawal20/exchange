## Build

```bash
# uwebsocket submodule
git submodule update --init --recursive

# generate build files
cmake -S . -B build
```

This server uses `wss://` (Secure WebSockets) and requires an SSL certificate to run.
For local development, you can generate a self-signed certificate.
**Note:** These certificates are for development purposes only and will cause browser warnings.
1.  **Create a directory for your certificates:**
    ```bash
    mkdir certs
    ```
2.  **Generate the self-signed certificate and private key:**
    ```bash
    openssl req -x509 -sha256 -newkey rsa:2048 -keyout certs/key.pem -out certs/cert.pem -days 365 -nodes \
        -subj "/C=IN/O=Builder Inc./CN=localhost" \
        -addext "subjectAltName = DNS:localhost"
    ```

The server is configured to look for `key.pem` and `cert.pem` inside the `certs/` directory.
```bash
# Using make (default generator)
cd build && make

# Or a generator-agnostic alternative
cd build && cmake --build .
```

## Tests
```bash
# Build test executable only
cd build && make tests

# Run tests manually
./tests

# Build and run all tests in one step
make check

# Or using CMake generator-agnostic syntax
cmake --build . --target check
```

## Editor Notes
```bash
# only needed if your editor/clangd is SHIT.
# (optional) editor happy = developer happy
ln -sf build/compile_commands.json compile_commands.json
```
