import type { FareRequest } from "../models/FareRequest";
import type { FareResult } from "../models/FareResult";

export class ApiError extends Error {
  constructor(message: string, public readonly status: number) {
    super(message);
    this.name = "ApiError";
  }
}

export class ApiClient {
  constructor(private readonly baseUrl: string = "") {}

  async calculateFare(request: FareRequest): Promise<FareResult> {
    const response = await fetch(`${this.baseUrl}/api/fare`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(request),
    });

    const payload: unknown = await response.json().catch(() => null);

    if (!response.ok) {
      throw new ApiError(ApiClient.extractError(payload), response.status);
    }
    return payload as FareResult;
  }

  private static extractError(payload: unknown): string {
    if (typeof payload === "object" && payload !== null && "error" in payload) {
      const message = (payload as { error: unknown }).error;
      if (typeof message === "string") {
        return message;
      }
    }
    return "The server could not compute the fare.";
  }
}