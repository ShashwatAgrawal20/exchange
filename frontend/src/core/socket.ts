import { SOCKET_URL } from "../consts/env";
import { stateManager } from "./state";

export class ExchangeSocket {
  #wssUrl: string;
  #socket: WebSocket;

  constructor(wssUrl: string) {
    this.#wssUrl = wssUrl;
    this.#socket = new WebSocket(this.#wssUrl);
    this.init();
  }

  private onOpen() {
    this.#socket.onopen = function (e) {
      this.send("Nero bhai, love you");
    };
  }

  private onClose() {
    this.#socket.onclose = function (e) {};
  }

  private onMessage() {
    this.#socket.onmessage = function (e) {
      const snapshot = JSON.parse(e.data)
      console.log(snapshot)
      stateManager.updateSnapshot(snapshot)
    };
  }

  private onError() {
    this.#socket.onerror = function (err) {
      console.error(err);
    };
  }

  private init() {
    this.onOpen();
    this.onMessage();
    this.onClose();
    this.onError();
  }

  public sendMessage(data: any) {
    this.#socket.send(data);
  }

  get isReady(): boolean {
    return this.#socket.readyState === 1;
  }

  addEventListener(event: string, cb: () => void) {
    this.#socket.addEventListener(event, cb, { once: true })
  }
}

export const exchangeSocket = new ExchangeSocket(SOCKET_URL);
