open Core;

external refToElement : Dom.element => LocalDom.Element.t = "%identity";

module InlineListing = {
  include ReactRe.Component.Stateful;
  let name = "InlineListing";
  type props = {lang: Protocol.language, text: string, slideInFrom: string, open_: string => unit};
  type state = {mutable preRef: option Dom.element};
  let getInitialState _ => {preRef: None};
  let componentDidMount {state} => {
    switch state.preRef {
    | Some r => Hljs.highlightBlock (refToElement r)
    | None => ()
    };
    None
  };
  let updatePreRef {state} r => state.preRef = Some r;
  let render {props, handler} => {
    let translateY = props.slideInFrom == "above" ? "-10vh" : "10vh";
    let className =
      Util.classNames [
        ("listing-container", true),
        ("ml", props.lang == ML),
        ("re", props.lang == RE)
      ];
    <Transition
      before=(ReactDOMRe.Style.make transform::("translateY(" ^ translateY ^ ")") ())
      after=(ReactDOMRe.Style.make transform::"translateY(0)" transition::"transform 250ms" ())>
      <div className onClick=(fun e => ReactEventRe.Mouse.stopPropagation e)>
        <div className="sidebar">
          (ReactRe.stringToElement (Protocol.stringOfLanguage props.lang))
        </div>
        <div className="main">
          <pre ref=(handler updatePreRef)> (ReactRe.stringToElement props.text) </pre>
          <footer>
            <CopyButton
              text=props.text
              style=(ReactDOMRe.Style.make cursor::"pointer" ())
              onCopy=ignore
            />
            <OpenButton
              style=(
                      ReactDOMRe.Style.make
                        cursor::"pointer" height::"1em" width::"1em" marginLeft::"1em" ()
                    )
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
