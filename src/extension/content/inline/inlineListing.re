open Core;

external refToElement : Dom.element => LocalDom.Element.t = "%identity";

type state = {preRef: ref(option(Dom.element))};

let updatePreRef = (r, {ReasonReact.state}) => state.preRef := Js.Nullable.to_opt(r);

let component = ReasonReact.reducerComponent("InlineListing");

let make = (~lang, ~text, ~slideInFrom, ~open_, _) => {
  ...component,
  initialState: () => {preRef: ref(None)},
  didMount: ({state}) => {
    switch state.preRef^ {
    | Some(r) => Hljs.highlightBlock(refToElement(r))
    | None => ()
    };
    ReasonReact.NoUpdate
  },
  reducer: ((), _state) => ReasonReact.NoUpdate,
  render: (self) => {
    let translateY = slideInFrom == "above" ? "-10vh" : "10vh";
    let className =
      Util.classNames([
        ("listing-container", true),
        ("ml", lang == RefmtShared.ML),
        ("re", lang == RefmtShared.RE)
      ]);
    <Transition
      before=(ReactDOMRe.Style.make(~transform={j|translateY($translateY)|j}, ()))
      after=(ReactDOMRe.Style.make(~transform="translateY(0)", ~transition="transform 250ms", ()))>
      <div className onClick=((e) => ReactEventRe.Mouse.stopPropagation(e))>
        <div className="sidebar">
          (ReasonReact.stringToElement(Protocol.stringOfLanguage(lang)))
        </div>
        <div className="main">
          <pre ref=(self.handle(updatePreRef))> (ReasonReact.stringToElement(text)) </pre>
          <footer>
            <CopyButton text style=(ReactDOMRe.Style.make(~cursor="pointer", ())) onCopy=ignore />
            <OpenButton
              style=(
                ReactDOMRe.Style.make(
                  ~cursor="pointer",
                  ~height="1em",
                  ~width="1em",
                  ~marginLeft="1em",
                  ()
                )
              )
              onClick=((_) => open_(text))
            />
          </footer>
        </div>
      </div>
    </Transition>
  }
};
