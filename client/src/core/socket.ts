import { SOCKET_URL } from "../consts/env";

export class ExchangeSocket {
  #wssUrl: string;
  #socket: WebSocket;

  get status(): string {
    return 'deeznuts are ready'
  }

  constructor(wssUrl: string) {
    this.#wssUrl = wssUrl;
    this.#socket = new WebSocket(this.#wssUrl);
    this.init();
    console.log(this.#socket);
  }

  private onOpen() {
    this.#socket.onopen = function (e) {
      this.send('Nero bhai, love you')
    };
  }

  private onClose() {
    this.#socket.onclose = function (e) {};
  }

  private onMessage() {
    this.#socket.onmessage = function (e) {
      console.log(e.data);
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

  private sendMessage(data: any) {
    this.#socket.send(data)
  }
}

export const exchangeSocket = Object.freeze(new ExchangeSocket(SOCKET_URL));

