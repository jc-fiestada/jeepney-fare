import type { Direction } from "./Direction";
import type { FareMode } from "./FareMode";
import type { PassengerType } from "./PassengerType";

export interface FareRequest {
  mode: FareMode;
  passengerType: PassengerType;
  direction?: Direction;
  getOn?: string;
  getOff?: string;
  distanceKm?: number;
}