import express from "express";
import expressWs from "express-ws";

const PORT = 7000;
const app = express();

expressWs(app);

const clients = new Set();

app.ws("/", (ws, req) => {
  clients.add(ws);
  console.log("Total clients: ", clients.size);

  for (let i = 0; i < 10; i++) {
    const snapshot = {
      ask: {
        level_1: Math.random(),
        level_2: Math.random(),
        level_3: Math.random(),
        level_4: Math.random(),
        level_5: Math.random(),
      },
      bid: {
        level_1: Math.random(),
        level_2: Math.random(),
        level_3: Math.random(),
        level_4: Math.random(),
        level_5: Math.random(),
      },
    };
    setTimeout(() => {
      ws.send(JSON.stringify(snapshot));
    }, 1000 * i);
  }

  ws.on("message", (msg) => {
    const serverTimestamp = Date.now();
    console.log(
      "[LOG]:",
      "server: ",
      serverTimestamp,
      "client: ",
      msg,
      "diff: ",
      serverTimestamp - msg,
    );
    //ws.send(JSON.stringify({ status: "received", data: msg }));
  });

  ws.on("close", () => {
    clients.delete(ws);
  });
});

app.listen(PORT, () => {
  console.log("Simulate deeznuts");
});
