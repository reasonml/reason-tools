open Core;

external refToElement : Dom.element => LocalDom.Element.t = "%identity";

type state = {preRef: option Dom.element};

let updatePreRef nullableRef _ _ =>
  switch (Js.Null.to_opt nullableRef) {
  | Some ref => ReasonReact.SilentUpdate {preRef: Some ref};
  | None => ReasonReact.NoUpdate
  };

let make ::lang ::text ::slideInFrom ::open_ _ => {
  ...(ReasonReact.statefulComponent "InlineListing"),

  initialState: fun () => {
    preRef: None
  },

  didMount: fun state _ => {
    switch state.preRef {
    | Some r => Hljs.highlightBlock (refToElement r)
    | None => ()
    };
    ReasonReact.NoUpdate
  },

  render: fun _ self => {
    let translateY = slideInFrom == "above" ? "-10vh" : "10vh";
    let className =
      Util.classNames [
        ("listing-container", true),
        ("ml", lang == Protocol.ML),
        ("re", lang == Protocol.RE)
      ];

    <Transition
      before=(ReactDOMRe.Style.make transform::({j|translateY($translateY)|j}) ())
      after=(ReactDOMRe.Style.make transform::"translateY(0)" transition::"transform 250ms" ())>
      <div className onClick=(fun e => ReactEventRe.Mouse.stopPropagation e)>
        <div className="sidebar">
          (ReactRe.stringToElement (Protocol.stringOfLanguage lang))
        </div>
        <div className="main">
          <pre ref=(self.update updatePreRef)> (ReactRe.stringToElement text) </pre>
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