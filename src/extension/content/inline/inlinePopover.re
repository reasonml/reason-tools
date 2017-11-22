let component = ReasonReact.statelessComponent("InlinePopover");

let make = (~inLang, ~inText, ~outLang, ~outText, ~close, ~open_, _) => {
  ...component,
  render: (_) =>
    <div className="root-container">
      <Transition
        before=(ReactDOMRe.Style.make(~opacity="0", ()))
        after=(ReactDOMRe.Style.make(~opacity="1", ~transition="opacity 250ms", ()))>
        <div className="mask-container" onClick=((_) => close())>
          <InlineListing lang=inLang text=inText slideInFrom="above" open_ />
          <InlineListing lang=outLang text=outText slideInFrom="below" open_ />
        </div>
      </Transition>
    </div>
};
