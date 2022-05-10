import "../CSS/MainPage.css";
import "../components/ReturnButton.js";
import ReturnButton from "../components/ReturnButton.js";
import ButtonList from "../components/ButtonList";
import { useNavigate } from "react-router-dom";
import ButtonAttributes from "../components/ButtonAttributes";

export default function SynthesizeTests() {
  let navigate = useNavigate();
  let Buttons = [
    new ButtonAttributes("CRK SHORT CIRCUIT", true, "Output Signal", true),
    new ButtonAttributes("CAM SHORT CIRCUIT", true, "Output Signal", true),
    new ButtonAttributes("CRK SPK", false, null, false),
    new ButtonAttributes("CRK RUN OUT", true, "Angle", false),
    new ButtonAttributes("CRK TOOTH OFF", true, "Teeth off", false),
    new ButtonAttributes("CRK GAP NOT DET", false, null, false),
    new ButtonAttributes("CRK SEG ADP ERR LIM", true, "Angle", false),
    new ButtonAttributes("CRK PULSE DURATION", true, "Duration", false),
    new ButtonAttributes("CRK POSN ENG STST", true, "Teeth Off", false),
    new ButtonAttributes("CAM DELAY", true, "°CRK", false),
    new ButtonAttributes("CAM SPK", false, null, false),
    new ButtonAttributes("CAM PAT ERR", false, null, false),
  ];
  return (
    <>
      <div className="frame-1">
        <p className="txt-271 flex-hcenter">Synthesize Tests</p>
        {ReturnButton()}
        <div>
          <ButtonList navigate={navigate} buttonList={Buttons} />
        </div>
      </div>
    </>
  );
}