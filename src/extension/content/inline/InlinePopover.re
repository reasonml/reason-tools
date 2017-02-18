external toElement : ReactRe.reactRef => ReasonJs.Document.element = "%identity";

module InlinePopover = {
  include ReactRe.Component;
  let name = "InlinePopover";

  type props = {
    inLang: string,
    inText: string,
    outLang: string,
    outText: string,
    close: unit => unit,
    open_: string => unit
  };

  let render {props} =>
    <div className="root-container">
      <Transition
        before=(Transition.makeStyle opacity::"0" ())
        after=(Transition.makeStyle opacity::"1" transition::"opacity 250ms" ())
      >
        <div className="mask-container" onClick=(fun _ => props.close ())>
          <InlineListing lang=props.inLang text=props.inText slideInFrom="above" open_=props.open_ />
          <InlineListing lang=props.outLang text=props.outText slideInFrom="below" open_=props.open_ />
        </div>
      </Transition>
    </div>;
};

include ReactRe.CreateComponent InlinePopover;
let createElement ::inLang ::inText ::outLang ::outText ::close ::open_ =>
  wrapProps  { inLang, inText, outLang, outText, close, open_ };
