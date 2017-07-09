
let component = ReasonReact.statelessComponent "InlinePopover";
let make ::inLang ::inText ::outLang ::outText ::close ::open_ _ => {
  ...component,

  render: fun _ =>
    <div className="root-container">
      <Transition
        before=(ReactDOMRe.Style.make opacity::"0" ())
        after=(ReactDOMRe.Style.make opacity::"1" transition::"opacity 250ms" ())>
        <div className="mask-container" onClick=(fun _ => close ())>
          <InlineListing
            lang=inLang
            text=inText
            slideInFrom="above"
            open_=open_
          />
          <InlineListing
            lang=outLang
            text=outText
            slideInFrom="below"
            open_=open_
          />
        </div>
      </Transition>
    </div>
};
