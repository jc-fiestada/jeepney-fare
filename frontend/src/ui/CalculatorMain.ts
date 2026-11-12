import { Direction } from "../models/Direction";
import { FareMode } from "../models/FareMode";
import { PassengerType } from "../models/PassengerType";
import { ApiClient } from "../services/ApiClient";
import { CalculatorPage } from "./CalculatorPage";
import { requireElement } from "./dom";
import { RadioGroup } from "./RadioGroup";
import { ResultPanel } from "./ResultPanel";
import { RouteSelector } from "./RouteSelector";

function bootstrap(): void {
  const page = new CalculatorPage({
    api: new ApiClient(),
    resultPanel: new ResultPanel(requireElement<HTMLElement>("result")),
    modeGroup: new RadioGroup<FareMode>([
      { id: "m1", value: FareMode.ByRoute },
      { id: "m2", value: FareMode.ByDistance },
    ]),
    directionGroup: new RadioGroup<Direction>([
      { id: "d1", value: Direction.GmaToBinan },
      { id: "d2", value: Direction.BinanToGma },
    ]),
    passengerGroup: new RadioGroup<PassengerType>([
      { id: "p1", value: PassengerType.Regular },
      { id: "p2", value: PassengerType.Student },
      { id: "p3", value: PassengerType.SeniorCitizen },
      { id: "p4", value: PassengerType.Pwd },
    ]),
    routeSelectors: {
      [Direction.GmaToBinan]: new RouteSelector(
        requireElement<HTMLSelectElement>("dir1-on"),
        requireElement<HTMLSelectElement>("dir1-off"),
      ),
      [Direction.BinanToGma]: new RouteSelector(
        requireElement<HTMLSelectElement>("dir2-on"),
        requireElement<HTMLSelectElement>("dir2-off"),
      ),
    },
    swapButtons: {
      [Direction.GmaToBinan]: requireElement<HTMLElement>("swap-dir1"),
      [Direction.BinanToGma]: requireElement<HTMLElement>("swap-dir2"),
    },
    distanceInput: requireElement<HTMLInputElement>("distance"),
    computeButton: requireElement<HTMLButtonElement>("compute"),
  });

  page.init();
}

bootstrap();