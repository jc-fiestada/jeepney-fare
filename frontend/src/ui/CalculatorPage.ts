import { Direction } from "../models/Direction";
import { FareMode } from "../models/FareMode";
import type { FareRequest } from "../models/FareRequest";
import type { PassengerType } from "../models/PassengerType";
import { ApiClient, ApiError } from "../services/ApiClient";
import type { RadioGroup } from "./RadioGroup";
import type { ResultPanel } from "./ResultPanel";
import type { RouteSelector } from "./RouteSelector";

export interface CalculatorPageDependencies {
  api: ApiClient;
  resultPanel: ResultPanel;
  modeGroup: RadioGroup<FareMode>;
  directionGroup: RadioGroup<Direction>;
  passengerGroup: RadioGroup<PassengerType>;
  routeSelectors: Record<Direction, RouteSelector>;
  swapButtons: Record<Direction, HTMLElement>;
  distanceInput: HTMLInputElement;
  computeButton: HTMLButtonElement;
}

export class CalculatorPage {
  constructor(private readonly deps: CalculatorPageDependencies) {}

  init(): void {
    const { computeButton, distanceInput, swapButtons } = this.deps;

    computeButton.addEventListener("click", () => {
      void this.handleCompute();
    });

    distanceInput.addEventListener("keydown", (event) => {
      if (event.key === "Enter") {
        void this.handleCompute();
      }
    });

    for (const direction of Object.values(Direction)) {
      this.bindSwap(swapButtons[direction], direction);
    }
  }

  private bindSwap(element: HTMLElement, from: Direction): void {
    element.setAttribute("role", "button");
    element.setAttribute("aria-label", "Swap stops and reverse direction");
    element.tabIndex = 0;

    element.addEventListener("click", () => this.swapTrip(from));
    element.addEventListener("keydown", (event) => {
      if (event.key === "Enter" || event.key === " ") {
        event.preventDefault();
        this.swapTrip(from);
      }
    });
  }

  private swapTrip(from: Direction): void {
    const { routeSelectors, directionGroup } = this.deps;
    const to = from === Direction.GmaToBinan ? Direction.BinanToGma : Direction.GmaToBinan;

    const getOn = routeSelectors[from].getOnStop.name;
    const getOff = routeSelectors[from].getOffStop.name;

    routeSelectors[to].selectTrip(getOff, getOn);
    directionGroup.select(to);
  }

  private buildRequest(): FareRequest {
    const { modeGroup, directionGroup, passengerGroup, routeSelectors, distanceInput } = this.deps;
    const passengerType = passengerGroup.value;

    if (modeGroup.value === FareMode.ByDistance) {
      const raw = distanceInput.value.trim();
      if (raw === "") {
        throw new Error("Enter a distance between 1 and 16 km.");
      }
      return { mode: FareMode.ByDistance, passengerType, distanceKm: Number(raw) };
    }

    const direction = directionGroup.value;
    const selector = routeSelectors[direction];
    return {
      mode: FareMode.ByRoute,
      passengerType,
      direction,
      getOn: selector.getOnStop.name,
      getOff: selector.getOffStop.name,
    };
  }

  private async handleCompute(): Promise<void> {
    const { api, resultPanel } = this.deps;

    let request: FareRequest;
    try {
      request = this.buildRequest();
    } catch (error) {
      resultPanel.showError(error instanceof Error ? error.message : "Invalid input.");
      return;
    }

    this.setBusy(true);
    try {
      const result = await api.calculateFare(request);
      resultPanel.showFare(result);
    } catch (error) {
      if (error instanceof ApiError) {
        resultPanel.showError(error.message);
      } else {
        resultPanel.showError("Could not reach the fare server. Please try again.");
      }
    } finally {
      this.setBusy(false);
    }
  }

  private setBusy(isBusy: boolean): void {
    this.deps.computeButton.disabled = isBusy;
  }
}