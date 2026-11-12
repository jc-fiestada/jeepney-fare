import type { Stop } from "../models/Stop";

export class RouteSelector {
  private readonly stops: readonly Stop[];

  constructor(
    private readonly getOnSelect: HTMLSelectElement,
    private readonly getOffSelect: HTMLSelectElement,
  ) {
    this.stops = Array.from(getOnSelect.options).map((option) =>
      RouteSelector.parseStop(option.text),
    );

    this.stops.forEach((stop, index) => {
      getOnSelect.options[index].value = stop.name;
    });
    Array.from(getOffSelect.options).forEach((option) => {
      option.value = RouteSelector.parseStop(option.text).name;
    });

    getOnSelect.addEventListener("change", () => this.refreshGetOff());
    this.refreshGetOff();
  }

  get getOnStop(): Stop {
    return this.requireStop(this.getOnSelect.value);
  }

  get getOffStop(): Stop {
    return this.requireStop(this.getOffSelect.value);
  }

  selectTrip(getOnName: string, getOffName: string): void {
    this.getOnSelect.value = getOnName;
    this.refreshGetOff();
    this.getOffSelect.value = getOffName;
  }

  private refreshGetOff(): void {
    const previous = this.getOffSelect.value;
    const getOn = this.getOnStop;
    const available = this.stops.filter((stop) => stop.km >= getOn.km);

    this.getOffSelect.replaceChildren(
      ...available.map((stop) => new Option(RouteSelector.formatLabel(stop), stop.name)),
    );

    const stillValid = available.some((stop) => stop.name === previous);
    this.getOffSelect.value = stillValid ? previous : getOn.name;
  }

  private requireStop(name: string): Stop {
    const stop = this.stops.find((candidate) => candidate.name === name);
    if (stop === undefined) {
      throw new Error("The selected stop is not on this route.");
    }
    return stop;
  }

  private static parseStop(label: string): Stop {
    const match = /^(.+?)\s*\u00B7\s*(\d+)\s*km$/.exec(label.trim());
    if (match === null) {
      throw new Error(`Unexpected stop label: ${label}`);
    }
    return { name: match[1], km: Number(match[2]) };
  }

  private static formatLabel(stop: Stop): string {
    return `${stop.name} \u00B7 ${stop.km} km`;
  }
}