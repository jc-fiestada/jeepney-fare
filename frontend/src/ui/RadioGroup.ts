import { requireElement } from "./dom";

export interface RadioOption<T> {
  id: string;
  value: T;
}

export class RadioGroup<T> {
  private readonly entries: ReadonlyArray<{ input: HTMLInputElement; value: T }>;

  constructor(options: ReadonlyArray<RadioOption<T>>) {
    this.entries = options.map((option) => ({
      input: requireElement<HTMLInputElement>(option.id),
      value: option.value,
    }));
  }

  get value(): T {
    const checked = this.entries.find((entry) => entry.input.checked);
    if (checked === undefined) {
      throw new Error("No option is selected.");
    }
    return checked.value;
  }

  select(value: T): void {
    const match = this.entries.find((entry) => entry.value === value);
    if (match !== undefined) {
      match.input.checked = true;
    }
  }
}