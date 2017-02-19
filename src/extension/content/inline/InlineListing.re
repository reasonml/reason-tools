open Core;

external refToElement : ReactRe.reactRef => Dom.Element.t = "%identity";

module InlineListing = {
  include ReactRe.Component.InstanceVars;
  let name = "InlineListing";
  type props = {lang: string, text: string, slideInFrom: string, open_: string => unit};
  type instanceVars = {mutable preRef: option ReactRe.reactRef};
  let getInstanceVars () => {preRef: None};
  let componentDidMount {instanceVars} => {
    switch instanceVars.preRef {
    | Some r => Hljs.highlightBlock (refToElement r)
    | None => ()
    };
    None
  };
  let render {props, refSetter} => {
    let translateY = props.slideInFrom == "above" ? "-10vh" : "10vh";
    let className =
      Util.classNames [
        ("listing-container", true),
        ("ml", props.lang == "ML"),
        ("re", props.lang == "RE")
      ];
    <Transition
      before=(Transition.makeStyle transform::("translateY(" ^ translateY ^ ")") ())
      after=(Transition.makeStyle transform::"translateY(0)" transition::"transform 250ms" ())>
      <div className onClick=(fun e => e##stopPropagation ())>
        <div className="sidebar"> (ReactRe.stringToElement props.lang) </div>
        <div className="main">
          <pre ref=(refSetter (fun {instanceVars} r => instanceVars.preRef = Some r))>
            (ReactRe.stringToElement props.text)
          </pre>
          <footer>
            <CopyButton text=props.text style={"cursor": "pointer"} onCopy=(fun () => ()) />
            <OpenButton
              style={"cursor": "pointer", "height": "1em", "width": "1em", "margin-left": "1em"}
              onClick=(fun _ => props.open_ props.text)
            />
          </footer>
        </div>
      </div>
    </Transition>
  };
};

include ReactRe.CreateComponent InlineListing;

let createElement ::lang ::text ::slideInFrom ::open_ =>
  wrapProps {lang, text, slideInFrom, open_};
