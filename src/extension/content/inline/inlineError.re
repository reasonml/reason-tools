
let component = ReasonReact.statelessComponent "InlineError";
let make ::message ::close _ => {
  ...component,

  render: fun _ =>
    <div className="root-container">
      <Transition
        before=(ReactDOMRe.Style.make opacity::"0" ())
        after=(ReactDOMRe.Style.make opacity::"1" transition::"opacity 250ms" ())>
        <div className="mask-container" onClick=(fun _ => close ())>
          <div className="error-message-container">
            (ReasonReact.stringToElement message)
          </div>
        </div>
      </Transition>
    </div>
};
