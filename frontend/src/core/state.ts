export type ExchangeState = {
  snapshot: Snapshot;
};

export interface Snapshot {
  ask: PriceLevels;
  bid: PriceLevels;
}

export type PriceLevels = {
  level_1: number;
  level_2: number;
  level_3: number;
  level_4: number;
  level_5: number;
};

export class StateManager extends EventTarget {
  state: ExchangeState;

  constructor() {
    super();
    this.state = {
      snapshot: {
        ask: {
          level_1: 0,
          level_2: 0,
          level_3: 0,
          level_4: 0,
          level_5: 0,
        },
        bid: {
          level_1: 0,
          level_2: 0,
          level_3: 0,
          level_4: 0,
          level_5: 0,
        },
      },
    };
  }

  updateSnapshot(newSnapshot: Snapshot) {
    this.state.snapshot = structuredClone(newSnapshot);
    this.dispatchEvent(new CustomEvent("snapshotUpdate"));
  }
}

export const stateManager = new StateManager();
