import { stateManager } from "../core/state";

export class SnapshotUI extends HTMLElement {
  constructor() {
    super();
  }

  connectedCallback() {
    stateManager.addEventListener("snapshotUpdate", () => {
      this.render();
    });

    this.render();
  }

  render() {
    const { ask, bid } = stateManager.state.snapshot;

    this.innerHTML = `
    <div style="
      font-family: monospace;
      background: #1a1a1a;
      padding: 20px;
      border-radius: 8px;
      max-width: 400px;
    ">
      <h3 style="margin: 0 0 16px 0; color: #888;">Order Book</h3>
      
      <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 24px;">
        <!-- ASK side -->
        <div>
          <div style="color: #ff4444; font-weight: bold; margin-bottom: 8px;">
            ASK (SELL)
          </div>
          ${[5, 4, 3, 2, 1]
            .map(
              (level) => `
            <div style="
              display: flex;
              justify-content: space-between;
              padding: 4px 8px;
              background: rgba(255, 68, 68, 0.1);
              margin-bottom: 2px;
              border-radius: 4px;
            ">
              <span style="color: #666;">L${level}</span>
              <span style="color: #ff4444;">${ask[`level_${level}`]}</span>
            </div>
          `,
            )
            .join("")}
        </div>
        
        <!-- BID side -->
        <div>
          <div style="color: #44ff44; font-weight: bold; margin-bottom: 8px;">
            BID (BUY)
          </div>
          ${[1, 2, 3, 4, 5]
            .map(
              (level) => `
            <div style="
              display: flex;
              justify-content: space-between;
              padding: 4px 8px;
              background: rgba(68, 255, 68, 0.1);
              margin-bottom: 2px;
              border-radius: 4px;
            ">
              <span style="color: #666;">L${level}</span>
              <span style="color: #44ff44;">${bid[`level_${level}` ]}</span>
            </div>
          `,
            )
            .join("")}
        </div>
      </div>
    </div>
  `;
  }
}

customElements.define("snapshot-ui", SnapshotUI);
