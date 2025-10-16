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
    You will be prompted to fill in some information. For the "Common Name," using `localhost` is a good practice.

The server is configured to look for `key.pem` and `cert.pem` inside the `certs/` directory.
```bash
# build project
cd build && make
```

## Editor Notes
```bash
# only needed if your editor/clangd is SHIT.
# (optional) editor happy = developer happy
ln -sf build/compile_commands.json compile_commands.json
```
