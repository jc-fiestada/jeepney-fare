import type { FareResult } from "../models/FareResult";
import { PassengerType } from "../models/PassengerType";

const PASSENGER_LABELS: Record<PassengerType, string> = {
  [PassengerType.Regular]: "Regular",
  [PassengerType.Student]: "Student",
  [PassengerType.SeniorCitizen]: "Senior Citizen",
  [PassengerType.Pwd]: "PWD",
};

export class ResultPanel {
  constructor(private readonly container: HTMLElement) {
    this.container.setAttribute("aria-live", "polite");
  }

  showFare(result: FareResult): void {
    const details: string[] = [];

    if (result.getOn !== undefined && result.getOff !== undefined) {
      details.push(`${result.getOn} to ${result.getOff} \u00B7 ${result.distanceKm} km`);
    } else {
      details.push(`${result.distanceKm} km`);
    }

    if (result.distanceKm === 0) {
      details.push("Same stop: the minimum fare applies.");
    }

    if (result.discountApplied) {
      const label = PASSENGER_LABELS[result.passengerType];
      details.push(
        `${label} fare, 20% discount applied (regular ${ResultPanel.formatPeso(result.regularFare)})`,
      );
    } else {
      details.push("Regular fare");
    }

    this.render("\u20B1", "Your fare", ResultPanel.formatPeso(result.fare), details, false);
  }

  showError(message: string): void {
    this.render("!", "Please check your details", null, [message], true);
  }

  private render(
    icon: string,
    title: string,
    amount: string | null,
    details: string[],
    isError: boolean,
  ): void {
    this.container.classList.toggle("error", isError);

    const children: HTMLElement[] = [
      ResultPanel.createElement("div", "ic", icon),
      ResultPanel.createElement("h4", "", title),
    ];
    if (amount !== null) {
      children.push(ResultPanel.createElement("div", "amount", amount));
    }
    for (const detail of details) {
      children.push(ResultPanel.createElement("p", "", detail));
    }
    this.container.replaceChildren(...children);
  }

  private static createElement(tag: "div" | "h4" | "p", className: string, text: string): HTMLElement {
    const element = document.createElement(tag);
    if (className !== "") {
      element.className = className;
    }
    element.textContent = text;
    return element;
  }

  private static formatPeso(value: number): string {
    return `\u20B1${value.toFixed(2)}`;
  }
}