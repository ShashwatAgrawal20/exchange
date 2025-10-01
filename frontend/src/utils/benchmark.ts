import { exchangeSocket } from "../core/socket";

export function runBenchmark() {
  if (exchangeSocket.isReady) {
    sendMessages();
  } else {
    exchangeSocket.addEventListener("open", sendMessages);
  }
}

function sendMessages() {
  for (let i = 0; i < 10_000; i++) {
    setTimeout(() => {
      exchangeSocket.sendMessage(Date.now());
      console.log("bruh");
    }, 1000 * i);
  }
}
