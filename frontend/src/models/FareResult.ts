import type { PassengerType } from "./PassengerType";

export interface FareResult {
  distanceKm: number;
  passengerType: PassengerType;
  regularFare: number;
  fare: number;
  discountApplied: boolean;
  getOn?: string;
  getOff?: string;
}