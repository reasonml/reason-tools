open Core;

external refToElement : Dom.element => LocalDom.Element.t = "%identity";

type state = {preRef: option Dom.element};

let updatePreRef nullableRef _ =>
  switch (Js.Null.to_opt nullableRef) {
  | Some ref => ReasonReact.SilentUpdate {preRef: Some ref};
  | None => ReasonReact.NoUpdate
  };

let component = ReasonReact.statefulComponent "InlineListing";
let make ::lang ::text ::slideInFrom ::open_ _ => {
  ...component,

  initialState: fun () => {
    preRef: None
  },

  didMount: fun {state} => {
    switch state.preRef {
    | Some r => Hljs.highlightBlock (refToElement r)
    | None => ()
    };
    ReasonReact.NoUpdate
  },

  render: fun self => {
    let translateY = slideInFrom == "above" ? "-10vh" : "10vh";
    let className =
      Util.classNames [
        ("listing-container", true),
        ("ml", lang == Refmt2.ML),
        ("re", lang == Refmt2.RE)
      ];

    <Transition
      before=(ReactDOMRe.Style.make transform::({j|translateY($translateY)|j}) ())
      after=(ReactDOMRe.Style.make transform::"translateY(0)" transition::"transform 250ms" ())>
      <div className onClick=(fun e => ReactEventRe.Mouse.stopPropagation e)>
        <div className="sidebar">
          (ReasonReact.stringToElement (Protocol.stringOfLanguage lang))
        </div>
        <div className="main">
          <pre ref=(self.update updatePreRef)> (ReasonReact.stringToElement text) </pre>
          <footer>
            <CopyButton
              text=text
              style=(ReactDOMRe.Style.make cursor::"pointer" ())
              onCopy=ignore
            />
            <OpenButton
              style=(ReactDOMRe.Style.make
                cursor::"pointer"
                height::"1em"
                width::"1em"
                marginLeft::"1em" ())
              onClick=(fun _ => open_ text)
            />
          </footer>
        </div>
      </div>
    </Transition>
  }
};
